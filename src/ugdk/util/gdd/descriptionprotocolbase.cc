#include <string>
#include <algorithm>
#include <utility>
#include <cassert>
#include <cstdio>

#include <ugdk/util/gdd/descriptionprotocolbase.h>
#include <ugdk/util/gdd/typeconverter.h>

namespace ugdk {
namespace gdd {

DescriptionProtocolBase::~DescriptionProtocolBase() {
    std::map<std::string, ArgsConverter*>::iterator it;
    for(it = properties_schema_.begin(); it != properties_schema_.end(); ++it) 
        delete it->second;
    for(it = rings_schema_.begin(); it != rings_schema_.end(); ++it) 
        delete it->second;
    for(it = entries_schema_.begin(); it != entries_schema_.end(); ++it) 
        delete it->second;
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

std::string DescriptionProtocolBase::to_lower(const std::string& str) {
    std::string lower_name = str;
    std::transform(lower_name.begin(), lower_name.end(), lower_name.begin(), ::tolower);
    return lower_name;
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
