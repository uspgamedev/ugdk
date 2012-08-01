#ifndef UGDK_UTIL_GDD_TYPECONVERTER_H_
#define UGDK_UTIL_GDD_TYPECONVERTER_H_

#include <string>
#include <cstdio>
#include <cmath>

#include <ugdk/util/gdd/types.h>

namespace ugdk {
namespace gdd {

//====================================================================
// Check if the given args is of the wanted type.
template<typename T>
static bool is_of_type(const GDDString& args) { return false; }

template<>
bool is_of_type<double>(const GDDString& args) { return true; } // TODO: implement this

template<>
bool is_of_type<int>(const GDDString& args) { return true; } // TODO: implement this

template<>
bool is_of_type<std::string>(const GDDString& args) { return args.size() == 1; }

//====================================================================
// Error message for when the given args is not of the wanted type.
template<typename T>
static std::string type_error_message() { return "INVALID TYPE"; }

template<>
std::string type_error_message<void>() { return "Expected no value at all."; }

template<>
std::string type_error_message<double>() { return "Expected value is a single double."; }

template<>
std::string type_error_message<int>() { return "Expected value is a single int."; }

template<>
std::string type_error_message<std::string>() { return "Expected value is a single string."; }

//====================================================================
// Convert the given args to the wanted type.
template<typename T>
T convert_to_type(const GDDString& args);

template<>
double convert_to_type(const GDDString& args) { return atof(args.c_str()); }

template<>
int convert_to_type(const GDDString& args) { return atoi(args.c_str()); }

template<>
std::string convert_to_type(const GDDString& args) { return args; }

} /* namespace gdd */
} /* namespace ugdk */

#endif // UGDK_UTIL_GDD_TYPECONVERTER_H_