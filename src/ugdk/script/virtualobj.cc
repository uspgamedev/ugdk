
#include <ugdk/script/virtualobj.h>

#include <algorithm>

namespace ugdk {
namespace script {

using std::vector;
using std::list;

VirtualObj VirtualObj::operator() (const list<VirtualObj>& args) const {
    vector<VirtualData::Ptr> arglist;
    for(auto it : args) {
        // Wrappers of executed VObj (we) and of the VObjs passed as
        // arguments must be the same.
        if (!it || wrapper() != it.wrapper()) {
            fprintf(stderr, "Calling a VirtualObj with non-matching wrappers in arguments.\n");
            return VirtualObj();
        }
        arglist.push_back(it.data_);
    }
    VirtualObj ret(data_->Execute(arglist));
    return ret;
}

VirtualObj VirtualObj::operator() (const VirtualObj& arg) const {
    return (*this)(list<VirtualObj>(1, arg));
}

VirtualObj VirtualObj::Create (const char* obj, LangWrapper* wrapper) {
    if (!wrapper) return VirtualObj();
    VirtualData::Ptr new_data = wrapper->NewData();
    new_data->WrapString(obj);
    return VirtualObj(new_data);
}

Bind::Bind(VirtualObj& obj, const std::string& method_name)
        :   obj_(obj),
            method_name_(obj.wrapper()) {
    method_name_.set_value(method_name.c_str());
}

VirtualObj Bind::operator() () const {
    std::list<VirtualObj> args;
    return obj_[method_name_]((obj_, args)); 
}
    
VirtualObj Bind::operator() (const VirtualObj& arg) const {
    std::list<VirtualObj> args(1, arg);
    return obj_[method_name_]((obj_, args)); 
}

VirtualObj Bind::operator() (std::list<VirtualObj>& args) const {
    return obj_[method_name_]((obj_, args));
}

} /* namespace script */
} /* namespace ugdk */
