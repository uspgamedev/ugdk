
#include <ugdk/script/virtualobj.h>

#include <algorithm>

namespace ugdk {
namespace script {

using std::vector;
using std::list;

VirtualObj VirtualObj::operator() (const list<VirtualObj>& args) const {
    vector<VirtualData::Ptr> arglist;
    list<VirtualObj>::const_iterator it;
    for (it = args.begin(); it != args.end(); ++it) {
        // Wrappers of executed VObj (we) and of the VObjs passed as
        // arguments must be the same.
        if (!*it || wrapper() != it->wrapper()) {
            puts("FAIL");
            return VirtualObj();
        }
        arglist.push_back(it->data_);
    }
    VirtualObj ret(data_->Execute(arglist));
    return ret;
}

VirtualObj VirtualObj::Create (const char* obj, LangWrapper* wrapper) {
    if (!wrapper) return VirtualObj();
    VirtualData::Ptr new_data = wrapper->NewData();
    new_data->WrapString(obj);
    return VirtualObj(new_data);
}


} /* namespace script */
} /* namespace ugdk */
