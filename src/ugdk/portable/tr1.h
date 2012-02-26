
#ifndef UGDK_PORTABLE_TR1_H_
#define UGDK_PORTABLE_TR1_H_

#include <ugdk/config/config.h>

#ifdef ECLIPSE_TR1_WORKAROUND_INCLUDE
#define FROM_TR1(header) <boost/tr1/header.hpp>
#else
#define FROM_TR1(header) <header>
#endif

#endif /* UGDK_PORTABLE_TR1_H_ */
