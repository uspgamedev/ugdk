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

//====================================================================
// Check if the given args is of the wanted type.
template<typename T>
static bool is_of_type(const GDDArgs& args) { return false; }

template<>
static bool is_of_type<void>(const GDDArgs& args) { return args.size() == 0; }

template<>
static bool is_of_type<double>(const GDDArgs& args) { return args.size() == 1; } // TODO: implement this

template<>
static bool is_of_type<int>(const GDDArgs& args) { return args.size() == 1; } // TODO: implement this

template<>
static bool is_of_type<std::string>(const GDDArgs& args) { return args.size() == 1; }

//====================================================================
// Error message for when the given args is not of the wanted type.
template<typename T>
static std::string type_error_message() { return "INVALID TYPE"; }

template<>
static std::string type_error_message<void>() { return "Expected no value at all."; }

template<>
static std::string type_error_message<double>() { return "Expected value is a single double."; }

template<>
static std::string type_error_message<int>() { return "Expected value is a single int."; }

template<>
static std::string type_error_message<std::string>() { return "Expected value is a single string."; }

//====================================================================
// Convert the given args to the wanted type.
template<typename T>
T convert_to_type(const GDDArgs& args);

template<>
double convert_to_type(const GDDArgs& args) { return atof(args[0].c_str()); }

template<>
int convert_to_type(const GDDArgs& args) { return atoi(args[0].c_str()); }

template<>
std::string convert_to_type(const GDDArgs& args) { return args[0]; }

//====================================================================
// Wrapper for function that converts the given argument to the necessary type.
template<typename T>
struct ConvertFunction {
    function<bool (T)> function_;

    ConvertFunction(function<bool (T)> func) : function_(func) {}

    bool operator()(const GDDArgs& args) const { 
        return function_(convert_to_type<T>(args));
    }
};

template<> // T=void calls the function another way.
bool ConvertFunction<void>::operator()(const GDDArgs& args) const { return function_(); }

// 
template<typename T>
class TypedArgsConverter : public ArgsConverter {
public:
    TypedArgsConverter(function<bool (T)> function) : function_(function) {}
    
    bool Process(const GDDArgs& args) const {
        if(!is_of_type<T>(args)) {
            return error(LoadError::INVALID_VALUE, type_error_message<T>());
        }
        return function_(args);
    }
  private:
    ConvertFunction<T> function_;
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
