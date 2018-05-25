
#ifndef UGDK_AUDIO_OPENAL_H_
#define UGDK_AUDIO_OPENAL_H_

#include <ugdk/system/config.h>

#ifdef CHECKMAC
# include <OpenAL/al.h>
# include <OpenAL/alc.h>
#else
# include <AL/al.h>
# include <AL/alc.h>
#endif

#endif // UGDK_AUDIO_OPENAL_H_
