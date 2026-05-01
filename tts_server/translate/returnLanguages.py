import translators as ts


def get_languages() -> list:
    return ts.get_languages().get("ace")


def get_providers() -> list:
    return ts.translators_pool


def test_translation(text: str, target_lang: str, translator: str) -> bool:
    """
    测试指定供应商能否完成翻译。
    - text: 待翻译文本
    - target_lang: 目标语言代码（如 'en', 'zh-Hans'）
    - translator: 供应商名称（如 'google', 'baidu'，必须是 get_providers() 中的值）
    返回: 成功返回 True，失败返回 False
    """
    try:
        result = ts.translate_text(text, to_language=target_lang, translator=translator)
        return bool(result)
    except Exception:
        return False
