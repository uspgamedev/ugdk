#include <ugdk/config/config.h>

#include <ugdk/script/module.h>
#include <ugdk/script/languages/lua/luawrapper.h>
#include <ugdk/script/languages/lua/modules.h>
#include <ugdk/script/languages/python/pythonwrapper.h>
#include <ugdk/script/languages/python/modules.h>

#define MODULE_INIT_DECLARTION(LANG, NAME) extern LANG##_INIT_FUNCTION_SIGNATURE(NAME);

#define DECLARE_MODULE_INITS(LANG) extern "C" { UGDK_MODULES_LIST(LANG, MODULE_INIT_DECLARTION) }

#define MODULE_ITEM(LANG, name) \
    LANG##_NAMESPACE::AddModule(::ugdk::script::Module< LANG##_NAMESPACE::inittype >(LANG##_MODULE_NAME(name), LANG##_INIT_FUNCTION_NAME(name)));

#define DECLARE_MODULES(LANG) \
static int build_UGDK_##LANG##_MODULES() { \
    UGDK_MODULES_LIST(LANG, MODULE_ITEM) \
    return 1; \
} \
int UGDK_##LANG##_MODULES = build_UGDK_##LANG##_MODULES();

UGDK_LANGUAGES_LIST(DECLARE_MODULE_INITS)
UGDK_LANGUAGES_LIST(DECLARE_MODULES)

int UGDK_MODULES_HEARTBEAT = 0;
