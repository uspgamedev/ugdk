#ifndef UGDK_UTIL_DESCRIPTIONPROTOCOLBASE_H_
#define UGDK_UTIL_DESCRIPTIONPROTOCOLBASE_H_

#include <string>
#include <vector>
#include <cstdio>
#include <map>

#include <ugdk/portable/tr1.h>
#include FROM_TR1(functional)

#include <ugdk/util/gdd/abstractloader.h>

namespace ugdk {

namespace gdd {

typedef std::string               GDDString;
typedef std::vector<GDDString>    GDDArgs;

class LoadError {
  public:
    enum Type {
        TYPE_MISMATCH,
        INVALID_VALUE
    };
};

class DescriptionProtocolBase;
class ArgsConverter {
public:
    virtual ~ArgsConverter() {}
    virtual bool Process(const GDDArgs& args) const = 0;

protected:
    ArgsConverter() {}
};

enum ProtocolField {
    PROPERTY, RING, ENTRY
};

class DescriptionProtocolBase {
  public:
    virtual ~DescriptionProtocolBase();

    virtual bool NewDescription() = 0;
    virtual bool NewData(const GDDString& data_name) = 0;

    void Register(ProtocolField, const GDDString& name, std::tr1::function<bool (double)> function);
    void Register(ProtocolField, const GDDString& name, std::tr1::function<bool (void)> function);
    
    template<class P, typename T>
    void RegisterBind(ProtocolField field, const GDDString& name, bool (P::*function) (T), P* obj) {
        std::tr1::function<bool (T)> result = std::tr1::bind(function, obj, std::tr1::placeholders::_1);
        Register(field, name, result);
    }
    
    template<class P>
    void RegisterBind(ProtocolField field, const GDDString& name, bool (P::*function) (void), P* obj) {
        std::tr1::function<bool ()> result = std::tr1::bind(function, obj);
        Register(field, name, result);
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

    std::map<std::string, ArgsConverter*> properties_schema_, rings_schema_, entries_schema_;
    GDDString simple_chain_ringname_;

    friend class ArgsConverter;

};

} /* namespace gdd */

} /* namespace ugdk */

#endif /* UGDK_UTIL_DESCRIPTIONPROTOCOL_H_ */
