#include <ugdk/graphic/shader/shaderprogram.h>

#include <cstdio>
#include <cassert>
#include "GL/glew.h"
#define NO_SDL_GLEXT
#include "SDL_opengl.h"

#include <ugdk/graphic/shader/shader.h>

namespace ugdk {
namespace graphic {
namespace shader {

ShaderProgram& MYSHADER() {
    static ShaderProgram myprogram;
    static bool configured = false;
    if(!configured) {
        configured = true;

        Shader vertex_shader(GL_VERTEX_SHADER), fragment_shader(GL_FRAGMENT_SHADER);

#define NEW_LINE "\n"
        vertex_shader.CompileSource(
        "#version 120" NEW_LINE

        "varying vec2 texture_coordinate;" NEW_LINE
        "varying vec4 colors;" NEW_LINE

        "void main(void) { " NEW_LINE
            "gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;" NEW_LINE
            "texture_coordinate = vec2(gl_MultiTexCoord0);" NEW_LINE
            "colors = gl_Color;" NEW_LINE
        "}");

        fragment_shader.CompileSource(
        "#version 120" NEW_LINE

        "varying vec2 texture_coordinate;" NEW_LINE
        "varying vec4 colors;" NEW_LINE
        "uniform sampler2D texture_id;" NEW_LINE

        "void main(void) {" NEW_LINE
        "   gl_FragColor = texture2D(texture_id, texture_coordinate) * colors;" NEW_LINE
        "}");

        myprogram.AttachShader(vertex_shader);
        myprogram.AttachShader(fragment_shader);

        bool status = myprogram.SetupProgram();
        assert(status);
    }
    return myprogram;
}

ShaderProgram::ShaderProgram() : id_(0) {
    id_ = glCreateProgram();
}
ShaderProgram::~ShaderProgram() {
    glDeleteProgram(id_);
}

bool ShaderProgram::IsValid() const {
    return glIsProgram(id_) == GL_TRUE;
}

void ShaderProgram::AttachShader(const Shader& shader) {
    glAttachShader(id_, shader.id());
}

bool ShaderProgram::SetupProgram() {
    glLinkProgram(id_);

    GLint status;
    glGetProgramiv (id_, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) 	{
        GLint infoLogLength;
        glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar *strInfoLog = new GLchar[infoLogLength + 1];
        glGetProgramInfoLog(id_, infoLogLength, NULL, strInfoLog);
        fprintf(stderr, "Linker failure: %s\n", strInfoLog);
        delete[] strInfoLog;
    }
    return status == GL_TRUE;
}


} // namespace ugdk
} // namespace graphic
} // namespace shader
