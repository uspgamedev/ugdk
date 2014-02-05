
#include <ugdk/script/virtualobj.h>

#include <algorithm>

namespace ugdk {
namespace script {

using std::vector;
using std::list;

Bind::Bind(VirtualObj& obj, const std::string& method_name)
    :   obj_(obj)
    ,   method_name_(obj.wrapper())
{
    method_name_.set_value(method_name.c_str());
}

} /* namespace script */
} /* namespace ugdk */
