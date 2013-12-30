# =========================================================================
# Freetype GL - A C OpenGL Freetype engine
# Platform:    Any
# WWW:         http://code.google.com/p/freetype-gl/
# -------------------------------------------------------------------------
# Copyright 2011,2012 Nicolas P. Rougier. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
#  1. Redistributions of source code must retain the above copyright notice,
#     this list of conditions and the following disclaimer.
#
#  2. Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in the
#     documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY NICOLAS P. ROUGIER ''AS IS'' AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
# EVENT SHALL NICOLAS P. ROUGIER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
# INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# The views and conclusions contained in the software and documentation are
# those of the authors and should not be interpreted as representing official
# policies, either expressed or implied, of Nicolas P. Rougier.
# =========================================================================
CMAKE_MINIMUM_REQUIRED( VERSION 2.8.11 )

SET( FREETYPE_GL_LIBRARY freetype-gl )
SET( FREETYPE_GL_SRC freetype-gl.h
                     vec234.h
                     opengl.h
                     mat4.c             mat4.h
                     texture-atlas.c    texture-atlas.h
                     texture-font.c     texture-font.h
                     vector.c           vector.h
                     platform.c         platform.h)

add_library( freetype-gl ${FREETYPE_GL_SRC} )

target_link_libraries(freetype-gl LINK_PUBLIC freetype)
if(CMAKE_COMPILER_IS_GNUCC)
    target_link_libraries(freetype-gl LINK_PUBLIC m)
endif()
if(ANDROID)
    target_link_libraries(freetype-gl LINK_PUBLIC GLESv2)
else()
    target_link_libraries(freetype-gl LINK_PUBLIC glew)
endif()

target_include_directories(freetype-gl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

