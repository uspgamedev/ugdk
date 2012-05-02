from ugdk.ugdk_base import Engine_reference

def RegisterLanguages():
    Engine_reference().language_manager().RegisterLanguage("en_US", "text/lang_en.txt")
    Engine_reference().language_manager().RegisterLanguage("pt_BR", "text/lang_pt_br.txt")
