%module ugdk_audio

%include <module/export.swig>
%include "std_map.i"
%include "std_string.i"

%{

#include <ugdk/audio/sample.h>
#include <ugdk/audio/music.h>
#include <ugdk/audio/audiomanager.h>

%}

/*  TAKE CARE
AudioManager has 2 methods that may return a new object
But we're not using the %newobject here because they only
return a new object sometimes, and the AudioManager
(at least in theory) should manage these objects.
*/

%include <ugdk/audio/sample.h>
%include <ugdk/audio/music.h>

//%template(SampleMap) std::map<std::string, ugdk::Sample*>;
//%template(MusicMap) std::map<std::string, ugdk::Music*>;

%include <ugdk/audio/audiomanager.h>

namespace ugdk {
    export_class(Sample)
    export_class(Music)
    export_class(AudioManager)
}
 
confirm_exports(ugdk_audio)
