
#ifndef UGDK_SCRIPT_LANGWRAPPER_H_
#define UGDK_SCRIPT_LANGWRAPPER_H_

#include <string>

#include <ugdk/script/defs.h>
#include <ugdk/script/type.h>
#include <ugdk/script/virtualdata.h>

namespace ugdk {
namespace script {

class VirtualObj;

/// Wraps a scripting language.
/** Classes derived from this should implement it's methods to wrap a given
 ** language in the system. 
 ** 
 ** Besides this interface, derived classes should implement a method
 ** 
 ** <code>
 ** bool RegisterModule(const string& moduleName, T initFunction)
 ** </code>
 **
 ** where moduleName is the name of the module, as to be imported from the language;
 ** T is a language specific module init function type;
 ** initFunction is the pointer to a function of the said type to initialize
 ** the module in the language (SWIG generates these functions in the wrappers)
 ** and the bool return value tells if the module was registered correctly.
 **
 ** Also, in order to properly add a scripting language to the system,
 ** besides implementing this class, you need to derive the VirtualData
 ** class (like this, implementing it specific to your language),
 ** modify whatever compiling mechanisms you use to make SWIG generate the wrappers
 ** to your language; and finally, when using the scripting system, properly
 ** registering your specific wrapper in the ScriptManager and the
 ** wrapper modules you want in your LangWrapper.
 */
class LangWrapper {

  public:

    virtual ~LangWrapper() {}

    const std::string& file_extension() { return file_extension_; }


    /// Initializes the LangWrapper.
    /** This is used to initialize the script language's API, if needed.
     ** @return bool : informing whether the initialization was successful.
     */
    virtual bool Initialize() = 0;

    /// Finalizes the LangWrapper, finalizing any language specific stuff.
    virtual void Finalize() = 0;

    virtual VirtualData::Ptr NewData() = 0;

	virtual void ExecuteCode(const std::string& code) = 0;

    virtual VirtualObj LoadModule(const std::string& name) = 0;

    const LangID lang_id () { return lang_id_; }

  protected:

    LangWrapper(const std::string& file_extension, const LangID id) :
        file_extension_(file_extension),
        lang_id_(id) {}

  private:

    const std::string file_extension_;
    const LangID      lang_id_;

};

}
}

#endif /* UGDK_SCRIPT_LANGWRAPPER_H_ */
