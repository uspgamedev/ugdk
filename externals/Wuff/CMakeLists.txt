add_library(Wuff STATIC
                 include/wuff.h
                 src/wuff_config.h
                 src/wuff_convert.h
                 src/wuff_internal.h
                 src/wuff_convert.c
                 src/wuff_internal.c
                 src/wuff_memory.c
                 src/wuff.c)

target_include_directories(Wuff PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
