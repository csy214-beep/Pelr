# Pelr - Live2D Virtual Desktop Partner
# https://github.com/csy214-beep/Pelr
# https://sourceforge.net/projects/pfolg-plauncher/
# Copyright (c) 2025 SY Cheng
#
# GPL v3 License
# https://gnu.ac.cn/licenses/gpl-3.0.html

import sys
import os
import json
import base64
import hashlib
import struct
import threading
import websocket
import ssl
from http.server import HTTPServer, BaseHTTPRequestHandler
import logging
from .Ws_Param import Ws_Param

# PySide6 imports
from PySide6.QtWidgets import QApplication, QSystemTrayIcon, QMenu
from PySide6.QtGui import QIcon, QAction
from PySide6.QtCore import QThread, Signal, QObject


# 配置日志
def setup_logging():
    # 获取临时目录
    # temp_dir = tempfile.gettempdir()
    base_dir = "log"
    # 创建日志目录
    if not os.path.exists(base_dir):
        os.makedirs(base_dir)
    log_file = os.path.join(base_dir, "tts_server.log")
    # 如果日志文件已存在，先删除
    if os.path.exists(log_file):
        os.remove(log_file)

    # 创建日志记录器
    logger = logging.getLogger("TTS_Server")
    logger.setLevel(logging.INFO)

    # 创建文件处理器
    file_handler = logging.FileHandler(log_file, encoding='utf-8')
    file_handler.setLevel(logging.INFO)

    # 创建控制台处理器（如果有终端）
    console_handler = logging.StreamHandler()
    console_handler.setLevel(logging.INFO)

    # 创建格式化器
    formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
    file_handler.setFormatter(formatter)
    console_handler.setFormatter(formatter)

    # 添加处理器到日志记录器
    logger.addHandler(file_handler)
    logger.addHandler(console_handler)

    return logger


# 初始化日志
logger = setup_logging()


# 单例装饰器
def singleton(cls):
    instances = {}

    def get_instance(*args, **kwargs):
        if cls not in instances:
            instances[cls] = cls(*args, **kwargs)
        return instances[cls]

    return get_instance


@singleton
class TTSService:
    def __init__(self):
        self.connected = False
        # 创建保存语音文件的目录
        self.output_dir = "voice_files"
        if not os.path.exists(self.output_dir):
            os.makedirs(self.output_dir)
        logger.info(f"TTS service initialized, output directory: {self.output_dir}")

    def generate_voice(self, appid, api_key, api_secret, text, voice="x4_yezi"):
        try:
            # 生成文件名
            text_hash = hashlib.sha256(text.encode()).hexdigest()
            output_file = os.path.abspath(os.path.join(self.output_dir, f"{text_hash}.wav"))

            # 如果文件已存在，直接返回路径
            if os.path.exists(output_file):
                logger.info(f"use existing voice file: {output_file}")
                return output_file

            logger.info(f"start generating voice: {text[:50]}...")
            wsParam = Ws_Param(appid, api_key, api_secret, text, output_file, voice)
            logger.info(f"final url: {wsParam}")
            websocket.enableTrace(False)
            wsUrl = wsParam.create_url()

            ws = websocket.WebSocketApp(wsUrl,
                                        on_message=self.on_message,
                                        on_error=self.on_error,
                                        on_close=self.on_close)
            ws.output_file = output_file
            ws.wsParam = wsParam
            ws.audio_data = []

            ws.on_open = self.on_open
            ws.run_forever(sslopt={"cert_reqs": ssl.CERT_NONE})

            # 返回文件路径（无论成功与否）
            if os.path.exists(output_file):
                logger.info(f"vioce generated successfully, output file: {output_file}")
                return output_file
            else:
                logger.error("vioce generation failed, no output file created")
                return None
        except Exception as e:
            logger.error(f"unexpected error occurred: {str(e)}")
            return None

    def on_message(self, ws, message):
        try:
            # 添加对 None 消息的检查
            if message is None:
                logger.warning("Received None message")
                return

            message = json.loads(message)
            code = message["code"]
            sid = message["sid"]

            # 检查是否有 data 字段
            if "data" not in message:
                logger.error(f"Message missing data field: {message}")
                return

            audio = message["data"]["audio"]
            audio = base64.b64decode(audio)
            status = message["data"]["status"]

            if status == 2:
                ws.close()
            if code != 0:
                errMsg = message["message"]
                logger.error(f"Error: sid:{sid} call error:{errMsg} code is:{code}")
            else:
                if not hasattr(ws, 'audio_data'):
                    ws.audio_data = []
                ws.audio_data.append(audio)
                if status == 2:
                    total_audio_size = sum(len(data) for data in ws.audio_data)
                    with open(ws.output_file, 'wb') as f:
                        self.write_wav_header(f, total_audio_size)
                        for data in ws.audio_data:
                            f.write(data)
                    logger.info(f"TTS completed. Output file: {ws.output_file}")
        except json.JSONDecodeError as e:
            logger.error(f"JSON parse error: {str(e)}, original message: {message}")
        except KeyError as e:
            logger.error(f"Message missing necessary field {str(e)}: {message}")
        except Exception as e:
            logger.error(f"Parse message error: {str(e)}")
            logger.debug(f"Original message content: {message}")

    def write_wav_header(self, file, audio_data_size, sample_rate=16000, num_channels=1, bits_per_sample=16):
        file.write(b'RIFF')
        file.write(struct.pack('<I', 36 + audio_data_size))
        file.write(b'WAVE')
        file.write(b'fmt ')
        file.write(struct.pack('<I', 16))
        file.write(struct.pack('<H', 1))
        file.write(struct.pack('<H', num_channels))
        file.write(struct.pack('<I', sample_rate))
        file.write(struct.pack('<I', sample_rate * num_channels * bits_per_sample // 8))
        file.write(struct.pack('<H', num_channels * bits_per_sample // 8))
        file.write(struct.pack('<H', bits_per_sample))
        file.write(b'data')
        file.write(struct.pack('<I', audio_data_size))

    def on_error(self, ws, error, *args):
        logger.error(f"WebSocket error: {error}")
        if args:
            logger.debug(f"Additional error args: {args}")

    def on_close(self, ws, close_status_code=None, close_msg=None, *args):
        logger.info(
            f"WebSocket connection closed (code: {close_status_code}, msg: {close_msg})"
        )
        if args:
            logger.debug(f"Additional close args: {args}")

    def on_open(self, ws):
        def run(*args):
            d = {"common": ws.wsParam.CommonArgs,
                 "business": ws.wsParam.BusinessArgs,
                 "data": ws.wsParam.Data}
            ws.send(json.dumps(d))
            logger.info("WebSocket connection established")

        threading.Thread(target=run).start()


class TTSRequestHandler(BaseHTTPRequestHandler):
    def log_message(self, format, *args):
        logger.info("%s - - [%s] %s\n" % (
            self.client_address[0],
            self.log_date_time_string(),
            format % args))

    def do_POST(self):
        if self.path == '/generate':
            try:
                content_length = int(self.headers['Content-Length'])
                post_data = self.rfile.read(content_length)
                data = json.loads(post_data.decode('utf-8'))

                appid = data.get('appid', '')
                api_key = data.get('api_key', '')
                api_secret = data.get('api_secret', '')
                text = data.get('text', '')
                voice = data.get('voice', 'x4_yezi')

                logger.info(f"Received TTS request: text={text[:50]}..., voice={voice}")

                if not all([appid, api_key, api_secret, text]):
                    self.send_response(400)
                    self.end_headers()
                    self.wfile.write(json.dumps({'error': 'Missing parameters'}).encode())
                    logger.warning("Lack of necessary parameters")
                    return

                # 使用单例TTS服务
                tts_service = TTSService()
                file_path = tts_service.generate_voice(appid, api_key, api_secret, text, voice)

                if file_path and os.path.exists(file_path):
                    self.send_response(200)
                    self.send_header('Content-type', 'application/json')
                    self.end_headers()
                    response = {'file_path': file_path}
                    self.wfile.write(json.dumps(response).encode())
                    logger.info(
                        f"Request processed successfully, output file: {file_path}"
                    )
                else:
                    self.send_response(500)
                    self.end_headers()
                    self.wfile.write(json.dumps({'error': 'TTS generation failed'}).encode())
                    logger.error("TTS generation failed")
            except Exception as e:
                self.send_response(500)
                self.end_headers()
                self.wfile.write(json.dumps({'error': f'Internal server error: {str(e)}'}).encode())
                logger.error(
                    f"An exception occurred while processing POST request: {str(e)}"
                )
        else:
            self.send_response(404)
            self.end_headers()
            logger.warning(f"Path not found: {self.path}")

    def do_GET(self):
        if self.path.startswith('/voice/'):
            filename = self.path[7:]  # 移除 '/voice/' 前缀
            file_path = os.path.join(TTSService().output_dir, filename)

            if os.path.exists(file_path):
                self.send_response(200)
                self.send_header('Content-type', 'audio/wav')
                self.send_header('Content-Length', str(os.path.getsize(file_path)))
                self.end_headers()

                with open(file_path, 'rb') as f:
                    self.wfile.write(f.read())
                logger.info(f"Voice file sent: {filename}")
            else:
                self.send_response(404)
                self.end_headers()
                logger.warning(f"Voice file not found: {filename}")
        else:
            self.send_response(404)
            self.end_headers()
            logger.warning(f"Path not found: {self.path}")


# HTTP服务器线程
class HTTPServerThread(QThread):
    def __init__(self, port=9140):
        super().__init__()
        self.port = port
        self.server = None

    def run(self):
        try:
            self.server = HTTPServer(('localhost', self.port), TTSRequestHandler)
            logger.info(f"TTS Server started on port {self.port}")
            self.server.serve_forever()
        except Exception as e:
            logger.error(f"Server start failed: {str(e)}")

    def stop(self):
        if self.server:
            self.server.shutdown()
            logger.info("TTS Server stopped")


# 系统托盘类
class TrayIcon(QSystemTrayIcon):
    def __init__(self):
        super().__init__()

        # 尝试加载图标
        icon_paths = [
            "assets/image/Pelr.ico",  # 优先尝试 .ico 格式
            "assets/image/Pelr.png",  # 其次尝试 .png 格式
        ]

        icon = None
        for path in icon_paths:
            if os.path.exists(path):
                try:
                    icon = QIcon(path)
                    if not icon.isNull():
                        logger.info(f"Icon loaded: {path}")
                        break
                except Exception as e:
                    logger.warning(f"load icon from {path} failed: {str(e)}")

        # 如果所有文件都加载失败，使用内置图标
        if not icon or icon.isNull():
            from PySide6.QtWidgets import QStyle

            try:
                icon = QApplication.style().standardIcon(QStyle.SP_ComputerIcon)
                logger.info("Load default icon")
            except Exception as e:
                logger.warning(f"Load default icon failed: {str(e)}")
                icon = QIcon()

        self.setIcon(icon)
        self.setToolTip("Pelr TTS Server")

        # 创建菜单
        self.menu = QMenu()

        # 添加退出动作
        exit_action = QAction("Quit", self)
        exit_action.triggered.connect(self.exit_app)
        self.menu.addAction(exit_action)

        self.setContextMenu(self.menu)

        # 显示托盘图标
        self.show()

    def exit_app(self):
        logger.info("Exiting...")
        # 停止HTTP服务器线程
        if hasattr(self, 'server_thread'):
            self.server_thread.stop()
        # 退出应用程序
        QApplication.quit()


# 主应用程序
class TTSApp(QApplication):
    def __init__(self, argv):
        super().__init__(argv)

        # 创建托盘图标
        self.tray_icon = TrayIcon()

        # 启动HTTP服务器线程
        self.server_thread = HTTPServerThread(port=9140)
        self.server_thread.start()

        logger.info("TTS Server started, Tray icon created")


def main():
    # 创建应用程序实例
    app = TTSApp(sys.argv)

    # 运行应用程序
    sys.exit(app.exec())


if __name__ == "__main__":
    main()
