#include <ugdk/script/python/pythonlangwrapper.h>

namespace ugdk {
namespace script {
namespace python {

VirtualObj PythonLangWrapper::LoadModule(std::string name) {
}

/// Initializes the LangWrapper (that is, the language's API. Returns bool telling if (true=) no problems occured.
bool PythonLangWrapper::Initialize() {
}

/// Finalizes the LangWrapper, finalizing any language specific stuff.
void PythonLangWrapper::Finalize() {
}

}
}
}
