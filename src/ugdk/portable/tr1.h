
#ifndef UGDK_PORTABLE_TR1_H_
#define UGDK_PORTABLE_TR1_H_

#include <ugdk/config/config.h>

#ifdef WIN32
#define FROM_TR1(header) <header>
#else
#define FROM_TR1(header) <tr1/header>
#endif

#endif /* UGDK_PORTABLE_TR1_H_ */
