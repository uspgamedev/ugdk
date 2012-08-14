#ifndef UGDK_UTIL_DESCRIPTIONPROTOCOLBASE_H_
#define UGDK_UTIL_DESCRIPTIONPROTOCOLBASE_H_

#include <string>
#include <vector>
#include <cstdio>
#include <map>

#include <ugdk/portable/tr1.h>
#include FROM_TR1(functional)

#include <ugdk/util/gdd/abstractloader.h>
#include <ugdk/util/gdd/types.h>
#include <ugdk/util/gdd/typedargsconverter.h>

namespace ugdk {
namespace gdd {


enum ProtocolField {
    PROPERTY, RING, ENTRY
};

class DescriptionProtocolBase {
  public:
    virtual ~DescriptionProtocolBase();

    virtual bool NewDescription() = 0;
    virtual bool NewData(const GDDString& data_name) = 0;

    template<class P>
    void RegisterBind(ProtocolField field, const GDDString& name, bool (P::*function) (void), P* obj) {
        std::tr1::function<bool ()> result = std::tr1::bind(function, obj);
        find_schema(field)[to_lower(name)] = new TypedArgsConverter<void, void, void, void, void>(result);
    }
    template<class P, typename T1>
    void RegisterBind(ProtocolField field, const GDDString& name, bool (P::*function) (T1), P* obj) {
        using namespace std::tr1::placeholders;
        std::tr1::function<bool (T1)> result = std::tr1::bind(function, obj, _1);
        registerFunc(field, name, result);
    }
    template<class P, typename T1, typename T2>
    void RegisterBind(ProtocolField field, const GDDString& name, bool (P::*function) (T1, T2), P* obj) {
        using namespace std::tr1::placeholders;
        std::tr1::function<bool (T1, T2)> result = std::tr1::bind(function, obj, _1, _2);
        registerFunc(field, name, result);
    }
    template<class P, typename T1, typename T2, typename T3>
    void RegisterBind(ProtocolField field, const GDDString& name, bool (P::*function) (T1, T2, T3), P* obj) {
        using namespace std::tr1::placeholders;
        std::tr1::function<bool (T1, T2, T3)> result = std::tr1::bind(function, obj, _1, _2, _3);
        registerFunc(field, name, result);
    }
    template<class P, typename T1, typename T2, typename T3, typename T4>
    void RegisterBind(ProtocolField field, const GDDString& name, bool (P::*function) (T1, T2, T3, T4), P* obj) {
        using namespace std::tr1::placeholders;
        std::tr1::function<bool (T1, T2, T3, T4)> result = std::tr1::bind(function, obj, _1, _2, _3, _4);
        registerFunc(field, name, result);
    }
    template<class P, typename T1, typename T2, typename T3, typename T4, typename T5>
    void RegisterBind(ProtocolField field, const GDDString& name, bool (P::*function) (T1, T2, T3, T4, T5), P* obj) {
        using namespace std::tr1::placeholders;
        std::tr1::function<bool (T1, T2, T3, T4, T5)> result = std::tr1::bind(function, obj, _1, _2, _3, _4, _5);
        registerFunc(field, name, result);
    }

    bool NewProperty(const GDDString& property_name, const GDDArgs& property_args);
    bool NewRing(const GDDString& ring_typename);
    bool NewEntry(const GDDString& entry_name, const GDDArgs& entry_args);
    bool NewSimpleChain(const GDDString& ring_typename, const GDDArgs& ring_args);

  protected:
    DescriptionProtocolBase() {}

    bool genericSchemaSearch(ProtocolField field, const std::string& error_msg, 
        const GDDString& key_name, const GDDArgs& args);

    bool error(LoadError::Type error_type, const std::string &msg);
    bool error(LoadError::Type error_type) { return error(error_type, ""); }
    void set_simple_chain_ringname(const GDDString& ringname) { simple_chain_ringname_ = ringname; }

  private:
    std::map<std::string, ArgsConverter*>& find_schema(ProtocolField);
    static std::string to_lower(const std::string& str);

    std::map<std::string, ArgsConverter*> properties_schema_, rings_schema_, entries_schema_;
    GDDString simple_chain_ringname_;

    friend class ArgsConverter;
    
    template<typename T1, typename T2, typename T3, typename T4, typename T5>
    void registerFunc(ProtocolField field, const GDDString& name, function<bool (T1, T2, T3, T4, T5)> function) {
        find_schema(field)[to_lower(name)] = new TypedArgsConverter<T1, T2, T3, T4, T5>(function);
    }
    template<typename T1, typename T2, typename T3, typename T4>
    void registerFunc(ProtocolField field, const GDDString& name, function<bool (T1, T2, T3, T4)> function) {
        find_schema(field)[to_lower(name)] = new TypedArgsConverter<T1, T2, T3, T4, void>(function);
    }
    template<typename T1, typename T2, typename T3>
    void registerFunc(ProtocolField field, const GDDString& name, function<bool (T1, T2, T3)> function) {
        find_schema(field)[to_lower(name)] = new TypedArgsConverter<T1, T2, T3, void, void>(function);
    }
    template<typename T1, typename T2>
    void registerFunc(ProtocolField field, const GDDString& name, function<bool (T1, T2)> function) {
        find_schema(field)[to_lower(name)] = new TypedArgsConverter<T1, T2, void, void, void>(function);
    }
    template<typename T1>
    void registerFunc(ProtocolField field, const GDDString& name, function<bool (T1)> function) {
        find_schema(field)[to_lower(name)] = new TypedArgsConverter<T1, void, void, void, void>(function);
    }
};

} /* namespace gdd */

} /* namespace ugdk */

#endif /* UGDK_UTIL_DESCRIPTIONPROTOCOL_H_ */
