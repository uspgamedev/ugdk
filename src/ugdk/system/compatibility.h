#ifndef UGDK_SYSTEM_COMPATIBILITY_H_
#define UGDK_SYSTEM_COMPATIBILITY_H_

#ifdef __GNUC__
#if __GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 7)
#define override 
#endif
#endif

#endif // UGDK_SYSTEM_COMPATIBILITY_H_
