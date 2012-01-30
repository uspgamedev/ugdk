
#ifndef UGDK_SCRIPT_LANGWRAPPER_H_
#define UGDK_SCRIPT_LANGWRAPPER_H_

#include <string>
#include <cstdio>

#include <ugdk/script/defs.h>
#include <ugdk/script/type.h>
#include <ugdk/script/virtualdata.h>

namespace ugdk {
namespace script {

class VirtualObj;

class LangWrapper {

  public:

    virtual ~LangWrapper();

    const std::string& file_extension() { return file_extension_; }


    /// Initializes the LangWrapper.
    /** This is used to initialize the script language's API, if needed.
     ** @return bool : informing whether the initialization was successful.
     */
    virtual bool Initialize() = 0;

    /// Finalizes the LangWrapper, finalizing any language specific stuff.
    virtual void Finalize() = 0;

    bool CheckIfModuleExists(std::string filepath) {
        FILE* file = fopen((filepath + file_extension()).c_str(), "r");
        if (file) {
            fclose(file);
            return true;
        }
        return false;
    }

    virtual VirtualData::Ptr WrapData(void* data, const VirtualType& type) = 0;

    virtual VirtualObj LoadModule(std::string name) = 0;


    const LangID id () { return id_; }

  protected:

    LangWrapper(const std::string& file_extension, const LangID id) :
        file_extension_(file_extension),
        id_(id) {}

  private:

    const std::string file_extension_;
    const LangID      id_;

};

}
}

#endif /* UGDK_SCRIPT_LANGWRAPPER_H_ */
