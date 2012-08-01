#include <string>
#include <algorithm>
#include <vector>
#include <cassert>
#include <ugdk/util/gdd/descriptionprotocolbase.h>
#include <cstdio>

namespace ugdk {
namespace gdd {

using std::tr1::function;
static bool error(LoadError::Type error_type, const std::string &msg) {
    switch (error_type) {
      case LoadError::TYPE_MISMATCH:
        fprintf(stderr, "Load Error: type mismatch. %s\n", msg.c_str());
        break;
      case LoadError::INVALID_VALUE:
        fprintf(stderr, "Load Error: invalid value. %s\n", msg.c_str());
        break;
      default:
        fprintf(stderr, "Load Error: unknown. %s\n", msg.c_str()); // \comofas
        break;
    }
    return false;
}

static bool is_a_double(const std::string& arg) { return true; } // TODO: implement this

class VoidArgsConverter : public ArgsConverter {
  public:
    VoidArgsConverter(function<bool (void)> function) : function_(function) {}

    bool Process(const GDDArgs& args) const {
        if(args.size() != 0) {
            return error(LoadError::INVALID_VALUE, "Expected no value at all.");
        }
        return function_();
    }
  private:
    function<bool (void)> function_;
};
    
class DoubleArgsConverter : public ArgsConverter {
  public:
    DoubleArgsConverter(function<bool (double)> function) : function_(function) {}

    bool Process(const GDDArgs& args) const {
        if(args.size() != 1 || !is_a_double(args[0])) {
            return error(LoadError::INVALID_VALUE, "Expected value is a single double.");
        }
        double val = atof(args[0].c_str());
        return function_(val);
    }
  private:
    function<bool (double)> function_;
};
    
DescriptionProtocolBase::~DescriptionProtocolBase() {
    std::map<std::string, ArgsConverter*>::iterator it;
    for(it = properties_schema_.begin(); it != properties_schema_.end(); ++it) 
        delete it->second;
    for(it = rings_schema_.begin(); it != rings_schema_.end(); ++it) 
        delete it->second;
    for(it = entries_schema_.begin(); it != entries_schema_.end(); ++it) 
        delete it->second;
}
    
void DescriptionProtocolBase::Register(ProtocolField field, const GDDString& name, function<bool (double)> function) {
    std::string lower_name = name;
    std::transform(lower_name.begin(), lower_name.end(), lower_name.begin(), ::tolower);
    find_schema(field)[lower_name] = new DoubleArgsConverter(function);
}

void DescriptionProtocolBase::Register(ProtocolField field, const GDDString& name, function<bool (void)> function) {
    std::string lower_name = name;
    std::transform(lower_name.begin(), lower_name.end(), lower_name.begin(), ::tolower);
    find_schema(field)[lower_name] = new VoidArgsConverter(function);
}
    
bool DescriptionProtocolBase::NewProperty(const GDDString& property_name, const GDDArgs& property_args) {
    return genericSchemaSearch(PROPERTY, "property", property_name, property_args);
}

bool DescriptionProtocolBase::NewRing(const GDDString& ring_typename) {
    return genericSchemaSearch(RING, "ring", ring_typename, GDDArgs());
}

bool DescriptionProtocolBase::NewEntry(const GDDString& entry_name, const GDDArgs& entry_args) {
    return genericSchemaSearch(ENTRY, "entry", entry_name, entry_args);
}

bool DescriptionProtocolBase::NewSimpleChain(const GDDString& ring_typename, const GDDArgs& ring_args) {
    for(GDDArgs::const_iterator it = ring_args.begin(); it != ring_args.end(); ++it) {
        GDDArgs entry_arg;
        entry_arg.push_back(*it);

        if (!NewRing(simple_chain_ringname_)) return false;
        if (!NewEntry(ring_typename, entry_arg)) return false;
    }
    return true;
}
    
std::map<std::string, ArgsConverter*>& DescriptionProtocolBase::find_schema(ProtocolField field) {
    switch(field) {
    case PROPERTY:  return properties_schema_;
    case RING:      return rings_schema_;
    case ENTRY:     return entries_schema_;
    default:
        assert(false); 
        static std::map<std::string, ArgsConverter*> invalid;
        return invalid;
    }
}

bool DescriptionProtocolBase::genericSchemaSearch(ProtocolField field, const std::string& error_msg, 
                                                  const GDDString& key_name, const GDDArgs& args) {
    std::map<std::string, ArgsConverter*>& schemas = find_schema(field);
    std::string lower_name = key_name;
    std::transform(lower_name.begin(), lower_name.end(), lower_name.begin(), ::tolower);
    
    std::map<std::string, ArgsConverter*>::iterator converter = schemas.find(lower_name);
    if(converter == schemas.end()) {
        std::string msg = "Unknown " + error_msg + " name '" + lower_name + "'.";
        return error(LoadError::INVALID_VALUE, msg);
    } else {
        return converter->second->Process(args);
    }
}

bool DescriptionProtocolBase::error(LoadError::Type error_type, const std::string &msg) {
    return ::ugdk::gdd::error(error_type, msg);
}

} /* namespace gdd */
} /* namespace ugdk */
