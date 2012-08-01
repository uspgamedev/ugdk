#include <string>
#include <algorithm>
#include <vector>
#include <cassert>
#include <cstdio>

#include <ugdk/util/gdd/descriptionprotocolbase.h>
#include <ugdk/util/gdd/typeconverter.h>

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

// 
template<typename T>
class TypedArgsConverter : public ArgsConverter {
public:
    TypedArgsConverter(function<bool (T)> function) : function_(function) {}
    
    bool Process(const GDDArgs& args) const {
        if(!size_matches(args)) return error(LoadError::INVALID_VALUE, "Expected a single value.");
        if(!type_matches(args)) return error(LoadError::TYPE_MISMATCH, type_error_message<T>());
        return call(args);
    }
  private:
    bool call(const GDDArgs& args) const { return function_(convert_to_type<T>(args[0])); }
    bool size_matches(const GDDArgs& args) const { return args.size() == 1; }
    bool type_matches(const GDDArgs& args) const { return is_of_type<T>(args[0]); }

    function<bool (T)> function_;
};

// T=void
template<> 
bool TypedArgsConverter<void>::call(const GDDArgs& args) const { return function_(); }
template<>
bool TypedArgsConverter<void>::size_matches(const GDDArgs& args) const { return args.size() == 0; }
template<>
bool TypedArgsConverter<void>::type_matches(const GDDArgs& args) const { return true; }

DescriptionProtocolBase::~DescriptionProtocolBase() {
    std::map<std::string, ArgsConverter*>::iterator it;
    for(it = properties_schema_.begin(); it != properties_schema_.end(); ++it) 
        delete it->second;
    for(it = rings_schema_.begin(); it != rings_schema_.end(); ++it) 
        delete it->second;
    for(it = entries_schema_.begin(); it != entries_schema_.end(); ++it) 
        delete it->second;
}
    
static std::string to_lower(const std::string& str) {
    std::string lower_name = str;
    std::transform(lower_name.begin(), lower_name.end(), lower_name.begin(), ::tolower);
    return lower_name;
}

#define DECLARE_REGISTER(TYPE) \
void DescriptionProtocolBase::Register(ProtocolField field, const GDDString& name, function<bool (TYPE)> function) { \
    find_schema(field)[to_lower(name)] = new TypedArgsConverter<TYPE>(function); \
}

DECLARE_REGISTER(double)
DECLARE_REGISTER(int)
DECLARE_REGISTER(std::string)
DECLARE_REGISTER(void)

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
    std::string lower_name = to_lower(key_name);
    
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
