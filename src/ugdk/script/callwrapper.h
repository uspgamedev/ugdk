
#ifndef UGDK_SCRIPT_CALLWRAPPER_H_
#define UGDK_SCRIPT_CALLWRAPPER_H_

#include <ugdk/script/defs.h>

namespace ugdk {

namespace script {

class CallWrapper {

  public:

    virtual ~CallWrapper() {}

    virtual bool AddArg (VirtualObjPtr arg) = 0;

    virtual bool Call () = 0;

    virtual VirtualObjPtr CollectResult() = 0;

  protected:

    CallWrapper() {}

};

} /* namespace script */

} /* namespace ugdk */

#endif /* UGDK_SCRIPT_CALLWRAPPER_H_ */
