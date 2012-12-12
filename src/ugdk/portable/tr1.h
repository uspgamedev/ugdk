#ifndef UGDK_PORTABLE_TR1_H_
#define UGDK_PORTABLE_TR1_H_

#ifdef __GNUC__
#define FROM_TR1(header) <tr1/header>
#else
#define FROM_TR1(header) <header>
#endif

#endif /* UGDK_PORTABLE_TR1_H_ */
