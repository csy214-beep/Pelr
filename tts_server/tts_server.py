#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import hashlib
import sys
import os
import threading
import logging
import signal
from functools import wraps

# Flask & gevent
from flask import Flask, request, jsonify, send_file
from gevent.pywsgi import WSGIServer

# PySide6 (用于托盘)
from PySide6.QtWidgets import QApplication, QSystemTrayIcon, QMenu
from PySide6.QtGui import QIcon, QAction
from PySide6.QtCore import QThread, Signal, QObject

# ======================
# 导入 Edge TTS 模块的视图函数
# ======================
# 注意：需要确保 server.py 中的视图函数可以独立导入
# 原 server.py 中的 app 我们不用，只导入需要的函数
from openai_edge_tts.server import (
    text_to_speech,  # /v1/audio/speech 等
    list_models,  # /v1/models
    list_voices_formatted,  # /v1/audio/voices
    list_voices,  # /v1/voices
    list_all_voices,  # /v1/voices/all
    elevenlabs_tts,  # /elevenlabs/...
    azure_tts  # /azure/...
)

# 导入 Edge TTS 的辅助函数和配置（确保环境变量加载）
from openai_edge_tts.config import DEFAULT_CONFIGS
from openai_edge_tts.utils import require_api_key, DETAILED_ERROR_LOGGING
from openai_edge_tts.tts_handler import generate_speech
from openai_edge_tts.handle_text import prepare_tts_input_with_context
from openai_edge_tts.config import DEFAULT_CONFIGS
import shutil
# ======================
# 导入 Pelr TTS 核心服务
# ======================
from iFlytek.server import TTSService, logger as pelr_logger

# ======================
# 创建统一 Flask 应用
# ======================
app = Flask(__name__)

# ---------- Edge TTS 路由 ----------
# 原 server.py 中的路由全部注册到新 app
app.add_url_rule('/v1/audio/speech', 'text_to_speech', text_to_speech, methods=['POST'])
app.add_url_rule('/audio/speech', 'text_to_speech_alias', text_to_speech, methods=['POST'])
app.add_url_rule('/v1/models', 'list_models', list_models, methods=['GET', 'POST'])
app.add_url_rule('/models', 'list_models_alias', list_models, methods=['GET', 'POST'])
app.add_url_rule('/v1/audio/models', 'list_audio_models', list_models, methods=['GET', 'POST'])
app.add_url_rule('/audio/models', 'list_audio_models_alias', list_models, methods=['GET', 'POST'])
app.add_url_rule('/v1/audio/voices', 'list_voices_formatted', list_voices_formatted, methods=['GET', 'POST'])
app.add_url_rule('/audio/voices', 'list_voices_formatted_alias', list_voices_formatted, methods=['GET', 'POST'])
app.add_url_rule('/v1/voices', 'list_voices', list_voices, methods=['GET', 'POST'])
app.add_url_rule('/voices', 'list_voices_alias', list_voices, methods=['GET', 'POST'])
app.add_url_rule('/v1/voices/all', 'list_all_voices', list_all_voices, methods=['GET', 'POST'])
app.add_url_rule('/voices/all', 'list_all_voices_alias', list_all_voices, methods=['GET', 'POST'])
app.add_url_rule('/elevenlabs/v1/text-to-speech/<voice_id>', 'elevenlabs_tts', elevenlabs_tts, methods=['POST'])
app.add_url_rule('/azure/cognitiveservices/v1', 'azure_tts', azure_tts, methods=['POST'])

# ---------- Pelr TTS 路由 ----------
# 复用 TTSService 单例
tts_service = TTSService()

# 复用 Pelr 的 voice_files 目录
EDGE_OUTPUT_DIR = tts_service.output_dir


@app.route('/voice_edge/<filename>', methods=['GET'])
def get_edge_voice(filename):
    """下载 Edge TTS 生成的音频文件（支持 mp3, wav, opus, aac, flac）"""
    # 防止路径遍历攻击
    if '..' in filename or filename.startswith('/'):
        return jsonify({"error": "Invalid filename"}), 400

    file_path = os.path.join(EDGE_OUTPUT_DIR, filename)
    if not os.path.exists(file_path):
        return jsonify({"error": "File not found"}), 404

    # 根据扩展名确定 MIME 类型
    ext = os.path.splitext(filename)[1].lower()
    mime_map = {
        '.mp3': 'audio/mpeg',
        '.wav': 'audio/wav',
        '.opus': 'audio/ogg',
        '.aac': 'audio/aac',
        '.flac': 'audio/flac'
    }
    mime_type = mime_map.get(ext, 'application/octet-stream')

    return send_file(file_path, mimetype=mime_type)


@app.route('/v1/audio/speech_local', methods=['POST'])
@app.route('/audio/speech_local', methods=['POST'])
def text_to_speech_local():
    """OpenAI 风格参数，但将音频保存到 voice_files 目录，并返回 JSON 格式的文件路径"""
    try:
        data = request.json
        if not data or 'input' not in data:
            return jsonify({"error": "Missing 'input' in request body"}), 400

        text = data.get('input')
        # 是否清洗文本（与原 Edge TTS 逻辑一致）
        remove_filter = os.getenv('REMOVE_FILTER', str(DEFAULT_CONFIGS["REMOVE_FILTER"])).lower() == 'true'
        if not remove_filter:
            text = prepare_tts_input_with_context(text)

        voice = data.get('voice', os.getenv('DEFAULT_VOICE', DEFAULT_CONFIGS["DEFAULT_VOICE"]))
        response_format = data.get('response_format', DEFAULT_CONFIGS["DEFAULT_RESPONSE_FORMAT"])
        speed = float(data.get('speed', DEFAULT_CONFIGS["DEFAULT_SPEED"]))

        # 生成临时音频文件
        temp_file_path = generate_speech(text, voice, response_format, speed)
        if not temp_file_path or not os.path.exists(temp_file_path):
            return jsonify({"error": "TTS generation failed"}), 500

        # 生成唯一的文件名（哈希值 + 扩展名）
        content_hash = hashlib.sha256(f"{text}_{voice}_{speed}_{response_format}".encode()).hexdigest()
        ext = response_format if response_format != 'mp3' else 'mp3'
        dest_filename = f"{content_hash}.{ext}"
        dest_path = os.path.join(EDGE_OUTPUT_DIR, dest_filename)

        # 如果文件已存在（相同参数生成过），直接返回，否则移动临时文件
        if not os.path.exists(dest_path):
            shutil.move(temp_file_path, dest_path)
        else:
            # 临时文件不再需要，直接删除
            os.remove(temp_file_path)

        # 返回绝对路径（与 Pelr 的 /generate 返回格式一致）
        abs_path = os.path.abspath(dest_path)
        pelr_logger.info(f"Edge TTS generated file: {abs_path}")
        return jsonify({
            "file_path": abs_path,
            "format": response_format,
            "voice": voice
        })

    except Exception as e:
        pelr_logger.error(f"Error in /v1/audio/speech_local: {e}")
        return jsonify({"error": str(e)}), 500


@app.route('/generate', methods=['POST'])
def generate_tts():
    """生成语音，返回文件路径（与原始 Pelr 接口一致）"""
    try:
        data = request.json
        if not data:
            return jsonify({'error': 'Missing JSON body'}), 400

        appid = data.get('appid', '')
        api_key = data.get('api_key', '')
        api_secret = data.get('api_secret', '')
        text = data.get('text', '')
        voice = data.get('voice', 'x4_yezi')

        if not all([appid, api_key, api_secret, text]):
            return jsonify({'error': 'Missing parameters (appid, api_key, api_secret, text)'}), 400

        pelr_logger.info(f"Received Pelr TTS request: text={text[:50]}..., voice={voice}")

        file_path = tts_service.generate_voice(appid, api_key, api_secret, text, voice)

        if file_path and os.path.exists(file_path):
            return jsonify({'file_path': file_path})
        else:
            return jsonify({'error': 'TTS generation failed'}), 500

    except Exception as e:
        pelr_logger.error(f"Error in /generate: {e}")
        return jsonify({'error': f'Internal server error: {str(e)}'}), 500


@app.route('/voice/<filename>', methods=['GET'])
def get_voice(filename):
    """获取生成的音频文件"""
    file_path = os.path.join(tts_service.output_dir, filename)
    if not os.path.exists(file_path):
        pelr_logger.warning(f"Voice file not found: {filename}")
        return jsonify({'error': 'File not found'}), 404

    try:
        return send_file(file_path, mimetype='audio/wav')
    except Exception as e:
        pelr_logger.error(f"Error sending file {file_path}: {e}")
        return jsonify({'error': 'File read error'}), 500


# ======================
# 启动服务（支持托盘）
# ======================

class FlaskServerThread(QThread):
    """在后台线程中运行 gevent WSGI 服务器"""

    def __init__(self, port):
        super().__init__()
        self.port = port
        self.server = None
        self._stop_event = threading.Event()

    def run(self):
        try:
            # 使用 gevent 服务器（与原 Edge TTS 一致）
            self.server = WSGIServer(('0.0.0.0', self.port), app)
            pelr_logger.info(f"Combined TTS Server started on port {self.port}")
            self.server.serve_forever()
        except Exception as e:
            pelr_logger.error(f"Server failed: {e}")

    def stop(self):
        if self.server:
            self.server.stop()
            pelr_logger.info("Server stopped")


class TrayIcon(QSystemTrayIcon):
    """系统托盘图标（整合后使用）"""

    def __init__(self, server_thread):
        super().__init__()
        self.server_thread = server_thread

        # 尝试加载图标（优先使用 Pelr 图标）
        icon_paths = [
            "assets/image/Pelr.ico",
            "assets/image/Pelr.png",
        ]
        icon = None
        for path in icon_paths:
            if os.path.exists(path):
                try:
                    icon = QIcon(path)
                    if not icon.isNull():
                        break
                except Exception:
                    pass

        if not icon or icon.isNull():
            from PySide6.QtWidgets import QStyle
            icon = QApplication.style().standardIcon(QStyle.SP_ComputerIcon)

        self.setIcon(icon)
        self.setToolTip("Pelr TTS Server")

        # 创建菜单
        menu = QMenu()
        exit_action = QAction("Quit", self)
        exit_action.triggered.connect(self.exit_app)
        menu.addAction(exit_action)
        self.setContextMenu(menu)

        self.show()

    def exit_app(self):
        """退出应用程序"""
        pelr_logger.info("Exiting...")
        self.server_thread.stop()
        QApplication.quit()


def main():
    # 读取端口（默认 5050）
    port = int(os.getenv('COMBINED_PORT', os.getenv('PORT', '9140')))

    # 创建 Qt 应用
    qt_app = QApplication(sys.argv)

    # 启动 Flask 服务器线程
    server_thread = FlaskServerThread(port)
    server_thread.start()

    # 创建托盘图标
    tray_icon = TrayIcon(server_thread)

    # 优雅退出信号处理
    def signal_handler(sig, frame):
        pelr_logger.info("Received interrupt signal, shutting down...")
        server_thread.stop()
        qt_app.quit()

    signal.signal(signal.SIGINT, signal_handler)
    signal.signal(signal.SIGTERM, signal_handler)

    # 进入 Qt 事件循环
    sys.exit(qt_app.exec())


if __name__ == '__main__':
    main()
