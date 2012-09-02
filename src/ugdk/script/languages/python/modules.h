#ifndef UGDK_SCRIPT_PYTHON_MODULES_H_
#define UGDK_SCRIPT_PYTHON_MODULES_H_

#include <ugdk/script.h>
#include <ugdk/script/languages/python/pythonwrapper.h>

namespace ugdk {
namespace script {
namespace python {

void AddModule(const Module<PYTHON_inittype> &);

void RegisterModules(PythonWrapper* wrapper);

} /* namespace python */
} /* namespace script */
} /* namespace ugdk */


#endif /* UGDK_SCRIPT_PYTHON_MODULES_H_ */
