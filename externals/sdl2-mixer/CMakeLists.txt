cmake_minimum_required(VERSION 2.8)
project(SDL2-mixer C)

set(SDL2-mixer-sources dynamic_flac.c dynamic_fluidsynth.c dynamic_mod.c dynamic_modplug.c dynamic_mp3.c dynamic_ogg.c
                       effect_position.c effect_stereoreverse.c effects_internal.c
                       fluidsynth.c
                       load_aiff.c load_flac.c load_ogg.c load_voc.c
                       mixer.c
                       music.c music_cmd.c music_flac.c music_mad.c music_mod.c  music_modplug.c music_ogg.c
                       wavestream.c
                       
                       dynamic_flac.h dynamic_fluidsynth.h dynamic_mod.h dynamic_modplug.h dynamic_mp3.h dynamic_ogg.h
                       effects_internal.h fluidsynth.h
                       load_aiff.h load_flac.h load_ogg.h load_voc.h
                       music_cmd.h music_flac.h music_mad.h music_mod.h music_modplug.h music_ogg.h
                       wavestream.h

                       include/SDL_mixer.h
)

find_package(Vorbis)
find_package(VorbisFile)

if(MSVC)
    list(APPEND SDL2-mixer-sources version.rc)
endif(MSVC)

add_library(SDL2-mixer ${SDL2-mixer-sources})
target_compile_definitions(SDL2-mixer PRIVATE WAV_MUSIC)
target_link_libraries(     SDL2-mixer LINK_PUBLIC SDL2)
target_include_directories(SDL2-mixer PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include") # TODO: add 'SYSTEM' flag when we have cmake 2.8.12

if(VORBIS_FOUND AND VORBISFILE_FOUND)
    target_compile_definitions(SDL2-mixer PRIVATE     OGG_MUSIC)
    target_link_libraries(     SDL2-mixer LINK_PUBLIC ${VORBIS_LIBRARIES} ${VORBISFILE_LIBRARIES})
    target_include_directories(SDL2-mixer PRIVATE     ${VORBIS_INCLUDE_DIRS} ${VORBISFILE_INCLUDE_DIRS})
endif()

