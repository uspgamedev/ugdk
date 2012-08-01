#ifndef UGDK_UTIL_DESCRIPTIONPROTOCOLBASE_H_
#define UGDK_UTIL_DESCRIPTIONPROTOCOLBASE_H_

#include <string>
#include <vector>
#include <ugdk/util/gdd/abstractloader.h>
#include <cstdio>
#include <map>

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
    virtual bool Process(DescriptionProtocolBase* target, const GDDArgs& args) const = 0;

protected:
    ArgsConverter() {}
    bool error(DescriptionProtocolBase* target, LoadError::Type error_type, const std::string& msg) const;
};

enum ProtocolField {
    PROPERTY, RING, ENTRY
};

class DescriptionProtocolBase {
  public:
    virtual ~DescriptionProtocolBase();

    virtual bool NewDescription() = 0;
    virtual bool NewData(const GDDString& data_name) = 0;

    void Register(ProtocolField, const GDDString& name, bool (DescriptionProtocolBase::*function) (double));
    void Register(ProtocolField, const GDDString& name, bool (DescriptionProtocolBase::*function) (void));

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

  private:
    std::map<std::string, ArgsConverter*>& find_schema(ProtocolField);

    std::map<std::string, ArgsConverter*> properties_schema_, rings_schema_, entries_schema_;
    GDDString simple_chain_ringname_;

    friend class ArgsConverter;

};

} /* namespace gdd */

} /* namespace ugdk */

#endif /* UGDK_UTIL_DESCRIPTIONPROTOCOL_H_ */
