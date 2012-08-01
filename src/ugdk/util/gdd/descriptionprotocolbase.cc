#include <string>
#include <algorithm>
#include <vector>
#include <utility>
#include <cassert>
#include <cstdio>
#include <ugdk/portable/tr1.h>
#include FROM_TR1(tuple)

#include <ugdk/util/gdd/descriptionprotocolbase.h>
#include <ugdk/util/gdd/typeconverter.h>

namespace ugdk {
namespace gdd {

using std::tr1::function;
using std::tr1::tuple;
using std::tr1::get;

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

template<typename T1, typename T2, typename T3, typename T4, typename T5>
bool call_function(const function<bool (T1, T2, T3, T4, T5)>& func, const tuple<T1, T2, T3, T4, T5>& t) {
    return func(get<0>(t), get<1>(t), get<2>(t), get<3>(t), get<4>(t));
}
template<typename T1, typename T2, typename T3, typename T4>
bool call_function(const function<bool (T1, T2, T3, T4)>& func, const tuple<T1, T2, T3, T4>& t) {
    return func(get<0>(t), get<1>(t), get<2>(t), get<3>(t));
}
template<typename T1, typename T2, typename T3>
bool call_function(const function<bool (T1, T2, T3)>& func, const tuple<T1, T2, T3>& t) {
    return func(get<0>(t), get<1>(t), get<2>(t));
}
template<typename T1, typename T2>
bool call_function(const function<bool (T1, T2)>& func, const tuple<T1, T2>& t) {
    return func(get<0>(t), get<1>(t));
}
template<typename T1>
bool call_function(const function<bool (T1)>& func, const tuple<T1>& t) {
    return func(get<0>(t));
}

template<typename T1, typename T2, typename T3, typename T4, typename T5>
tuple<T1, T2, T3, T4, T5> convert_to_tuple(const GDDArgs& args) {
    return tuple<T1, T2, T3, T4, T5>(convert_to_type<T1>(args[0]), convert_to_type<T2>(args[1]), convert_to_type<T3>(args[2]), convert_to_type<T4>(args[3]), convert_to_type<T5>(args[4]));
}
template<typename T1, typename T2, typename T3, typename T4>
tuple<T1, T2, T3, T4> convert_to_tuple(const GDDArgs& args) {
    return tuple<T1, T2, T3, T4>(convert_to_type<T1>(args[0]), convert_to_type<T2>(args[1]), convert_to_type<T3>(args[2]), convert_to_type<T4>(args[3]));
}
template<typename T1, typename T2, typename T3>
tuple<T1, T2, T3> convert_to_tuple(const GDDArgs& args) {
    return tuple<T1, T2, T3>(convert_to_type<T1>(args[0]), convert_to_type<T2>(args[1]), convert_to_type<T3>(args[2]));
}
template<typename T1, typename T2>
tuple<T1, T2> convert_to_tuple(const GDDArgs& args) {
    return tuple<T1, T2>(convert_to_type<T1>(args[0]), convert_to_type<T2>(args[1]));
}
template<typename T1>
tuple<T1> convert_to_tuple(const GDDArgs& args) {
    return tuple<T1>(convert_to_type<T1>(args[0]));
}

#define CHECK_TYPE(TYPE, i) if(!is_of_type<TYPE>(args[i])) return error(LoadError::TYPE_MISMATCH, type_error_message<TYPE>())
template<typename T1>
bool validate_types(const GDDArgs& args) {
    CHECK_TYPE(T1, 0);
    return true;
}
template<typename T1, typename T2>
bool validate_types(const GDDArgs& args) {
    CHECK_TYPE(T2, 1);
    return validate_types<T1>(args);
}
template<typename T1, typename T2, typename T3>
bool validate_types(const GDDArgs& args) {
    CHECK_TYPE(T3, 2);
    return validate_types<T1, T2>(args);
}
template<typename T1, typename T2, typename T3, typename T4>
bool validate_types(const GDDArgs& args) {
    CHECK_TYPE(T4, 3);
    return validate_types<T1, T2, T3>(args);
}
template<typename T1, typename T2, typename T3, typename T4, typename T5>
bool validate_types(const GDDArgs& args) {
    CHECK_TYPE(T5, 4);
    return validate_types<T1, T2, T3, T4>(args);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5>
class TypedArgsConverter : public ArgsConverter {
    typedef function<bool (T1, T2, T3, T4, T5)> Func;
    static const int num_args = 5;
  public:
    TypedArgsConverter(Func function) : function_(function) {}
    
    bool Process(const GDDArgs& args) const {
        if(!size_matches(args)) return error(LoadError::INVALID_VALUE, "Unexpected number of values.");
        if(!type_matches(args)) return false;
        return call(args);
    }
  private:
    bool size_matches(const GDDArgs& args) const { return args.size() == num_args; }
    bool type_matches(const GDDArgs& args) const { return validate_types<T1, T2, T3, T4, T5>(args); }
    bool call(const GDDArgs& args) const { return call_function(function_, convert_to_tuple<T1, T2, T3, T4, T5>(args)); }
    Func function_;
};

template <typename T1, typename T2, typename T3, typename T4>
class TypedArgsConverter<T1, T2, T3, T4, void> : public ArgsConverter {
    typedef function<bool (T1, T2, T3, T4)> Func;
    static const int num_args = 4;
  public:
    TypedArgsConverter(Func function) : function_(function) {}
    
    bool Process(const GDDArgs& args) const {
        if(!size_matches(args)) return error(LoadError::INVALID_VALUE, "Unexpected number of values.");
        if(!type_matches(args)) return false;
        return call(args);
    }
  private:
    bool size_matches(const GDDArgs& args) const { return args.size() == num_args; }
    bool type_matches(const GDDArgs& args) const { return validate_types<T1, T2, T3, T4>(args); }
    bool call(const GDDArgs& args) const { return call_function(function_, convert_to_tuple<T1, T2, T3, T4>(args)); }
    Func function_;
};

template <typename T1, typename T2, typename T3>
class TypedArgsConverter<T1, T2, T3, void, void> : public ArgsConverter {
    typedef function<bool (T1, T2, T3)> Func;
    static const int num_args = 3;
  public:
    TypedArgsConverter(Func function) : function_(function) {}
    
    bool Process(const GDDArgs& args) const {
        if(!size_matches(args)) return error(LoadError::INVALID_VALUE, "Unexpected number of values.");
        if(!type_matches(args)) return false;
        return call(args);
    }
  private:
    bool size_matches(const GDDArgs& args) const { return args.size() == num_args; }
    bool type_matches(const GDDArgs& args) const { return validate_types<T1, T2, T3>(args); }
    bool call(const GDDArgs& args) const { return call_function(function_, convert_to_tuple<T1, T2, T3>(args)); }
    Func function_;
};

template <typename T1, typename T2>
class TypedArgsConverter<T1, T2, void, void, void> : public ArgsConverter {
    typedef function<bool (T1, T2)> Func;
    static const int num_args = 2;
  public:
    TypedArgsConverter(Func function) : function_(function) {}
    
    bool Process(const GDDArgs& args) const {
        if(!size_matches(args)) return error(LoadError::INVALID_VALUE, "Unexpected number of values.");
        if(!type_matches(args)) return false;
        return call(args);
    }
  private:
    bool size_matches(const GDDArgs& args) const { return args.size() == num_args; }
    bool type_matches(const GDDArgs& args) const { return validate_types<T1, T2>(args); }
    bool call(const GDDArgs& args) const { return call_function(function_, convert_to_tuple<T1, T2>(args)); }
    Func function_;
};

template <typename T1>
class TypedArgsConverter<T1, void, void, void, void> : public ArgsConverter {
    typedef function<bool (T1)> Func;
    static const int num_args = 1;
  public:
    TypedArgsConverter(Func function) : function_(function) {}
    
    bool Process(const GDDArgs& args) const {
        if(!size_matches(args)) return error(LoadError::INVALID_VALUE, "Unexpected number of values.");
        if(!type_matches(args)) return false;
        return call(args);
    }
  private:
    bool size_matches(const GDDArgs& args) const { return args.size() == num_args; }
    bool type_matches(const GDDArgs& args) const { return validate_types<T1>(args); }
    bool call(const GDDArgs& args) const { return call_function(function_, convert_to_tuple<T1>(args)); }
    Func function_;
};

template <> // THIS ONE IS DIFFERENT
class TypedArgsConverter<void, void, void, void, void> : public ArgsConverter {
    typedef function<bool (void)> Func;
    static const int num_args = 0;
  public:
    TypedArgsConverter(Func function) : function_(function) {}
    
    bool Process(const GDDArgs& args) const {
        if(!size_matches(args)) return error(LoadError::INVALID_VALUE, "Unexpected number of values.");
        if(!type_matches(args)) return false;
        return call(args);
    }
  private:
    bool size_matches(const GDDArgs& args) const { return args.size() == num_args; }
    bool type_matches(const GDDArgs& args) const { return true; }
    bool call(const GDDArgs& args) const { return function_(); }
    Func function_;
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
    find_schema(field)[to_lower(name)] = new TypedArgsConverter<TYPE, void, void, void, void>(function); \
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
