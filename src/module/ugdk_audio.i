%module ugdk_audio

%include <module/export.swig>
%include "std_map.i"
%include "std_string.i"

%{

#include <ugdk/audio/sample.h>
#include <ugdk/audio/music.h>
#include <ugdk/audio/manager.h>
#include <ugdk/audio/module.h>

%}

/*  TAKE CARE
audio::Manager has 2 methods that may return a new object
But we're not using the %newobject here because they only
return a new object sometimes, and the audio::Manager manages these objects.
*/

%include <ugdk/audio/sample.h>
%include <ugdk/audio/music.h>

//%template(SampleMap) std::map<std::string, ugdk::audio::Sample*>;
//%template(MusicMap) std::map<std::string, ugdk::audio::Music*>;

%include <ugdk/audio/manager.h>
%include <ugdk/audio/module.h>

namespace ugdk {
namespace audio {
    export_class(Sample)
    export_class(Music)
    export_class(Manager)
}
}
 
confirm_exports(ugdk_audio)
