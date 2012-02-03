
#ifndef UGDK_PORTABLE_TR1_H_
#define UGDK_PORTABLE_TR1_H_

#ifdef ECLIPSE_BOOST_WORKAROUND_INCLUDE
#define FROM_TR1(header) <boost/tr1/header.hpp>
#else
#define FROM_TR1(header) <header>
#endif

#endif /* UGDK_PORTABLE_TR1_H_ */
