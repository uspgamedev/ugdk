
#include <ugdk/script/virtualobj.h>

namespace ugdk {
namespace script {

using std::vector;

VirtualObj VirtualObj::operator() (const vector<VirtualObj>& args) const {
    vector<VirtualData::Ptr> arglist;
    vector<VirtualObj>::const_iterator it;
    for (it = args.begin(); it != args.end(); ++it) {
        // Wrappers of executed VObj (we) and of the VObjs passed as
        // arguments must be the same.
        if (wrapper() != it->wrapper())
            return VirtualObj();
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
