import translators as ts


def translate_text(text: str, target_lang: str, translator: str) -> str:
    """
    使用指定供应商将文本翻译成目标语言。

    参数:
        text:       待翻译文本
        target_lang: 目标语言代码（如 'en', 'zh-Hans'）
        translator: 供应商名称（如 'google', 'baidu'）

    返回:
        翻译后的字符串，失败时返回空字符串
    """
    try:
        result = ts.translate_text(text, to_language=target_lang, translator=translator)
        return str(result) if result else ""
    except Exception:
        return ""
