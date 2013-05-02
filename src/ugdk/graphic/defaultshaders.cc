#include <ugdk/graphic/defaultshaders.h>

#include "GL/glew.h"
#define NO_SDL_GLEXT
#include <cassert>

#include <ugdk/graphic/opengl/shader.h>
#include <ugdk/graphic/opengl/shaderprogram.h>

namespace ugdk {
namespace graphic {

opengl::ShaderProgram* InterfaceShader() {
    static opengl::ShaderProgram* myprogram = NULL;
    if(!myprogram) {
        opengl::Shader vertex_shader(GL_VERTEX_SHADER), fragment_shader(GL_FRAGMENT_SHADER);

        vertex_shader.AddCodeBlock("out vec2 UV;" "\n");
        vertex_shader.AddLineInMain("	gl_Position =  geometry_matrix * vec4(vertexPosition,0,1);" "\n");
        vertex_shader.AddLineInMain("	UV = vertexUV;" "\n");
        vertex_shader.GenerateSource();

        fragment_shader.AddCodeBlock("in vec2 UV;" "\n"
                                     "uniform sampler2D drawable_texture;" "\n"
                                     "uniform vec4 effect_color;" "\n");
        fragment_shader.AddLineInMain("	vec4 color = texture2D( drawable_texture, UV ) * effect_color;" "\n");
        fragment_shader.AddLineInMain(" gl_FragColor = color;" "\n");
        fragment_shader.GenerateSource();


        myprogram = new opengl::ShaderProgram;
        myprogram->AttachShader(vertex_shader);
        myprogram->AttachShader(fragment_shader);

        bool status = myprogram->SetupProgram();
        assert(status);
    }
    return myprogram;
}

opengl::ShaderProgram* LightSystemShader() {
    static opengl::ShaderProgram* myprogram = NULL;
    if(!myprogram) {
        opengl::Shader vertex_shader(GL_VERTEX_SHADER), fragment_shader(GL_FRAGMENT_SHADER);

        vertex_shader.AddCodeBlock("out vec2 UV;" "\n");
        vertex_shader.AddCodeBlock("out vec2 lightUV;" "\n"
                                   "void calculateLightUV() {" "\n"
                                   "   lightUV = (gl_Position.xy + vec2(1, 1)) * 0.5;" "\n"
                                   "}" "\n");
        vertex_shader.AddLineInMain("	gl_Position =  geometry_matrix * vec4(vertexPosition,0,1);" "\n");
        vertex_shader.AddLineInMain("	UV = vertexUV;" "\n");
        vertex_shader.AddLineInMain("   calculateLightUV();" "\n");
        vertex_shader.GenerateSource();

        fragment_shader.AddCodeBlock("in vec2 UV;" "\n"
                                     "uniform sampler2D drawable_texture;" "\n"
                                     "uniform vec4 effect_color;" "\n");
        fragment_shader.AddCodeBlock("in vec2 lightUV;" "\n"
                                     "uniform sampler2D light_texture;" "\n");
        fragment_shader.AddLineInMain("	vec4 color = texture2D( drawable_texture, UV ) * effect_color;" "\n");
        fragment_shader.AddLineInMain("	color *= vec4(texture2D(light_texture, lightUV).rgb, 1.0);" "\n");
        fragment_shader.AddLineInMain(" gl_FragColor = color;" "\n");
        fragment_shader.GenerateSource();

        myprogram = new opengl::ShaderProgram;

        myprogram->AttachShader(vertex_shader);
        myprogram->AttachShader(fragment_shader);

        bool status = myprogram->SetupProgram();
        assert(status);
    }
    return myprogram;
}

opengl::ShaderProgram* InterfaceTextShader() {
    static opengl::ShaderProgram* myprogram = NULL;
    if(!myprogram) {
        opengl::Shader vertex_shader(GL_VERTEX_SHADER), fragment_shader(GL_FRAGMENT_SHADER);

        vertex_shader.AddCodeBlock("out vec2 UV;" "\n");
        vertex_shader.AddLineInMain("	gl_Position =  geometry_matrix * vec4(vertexPosition,0,1);" "\n");
        vertex_shader.AddLineInMain("	UV = vertexUV;" "\n");
        vertex_shader.GenerateSource();

        fragment_shader.AddCodeBlock("in vec2 UV;" "\n"
                                     "uniform sampler2D drawable_texture;" "\n"
                                     "uniform vec4 effect_color;" "\n");
        fragment_shader.AddLineInMain("	vec4 color = vec4(effect_color.rgb, texture2D( drawable_texture, UV ).a * effect_color.a);" "\n");
        fragment_shader.AddLineInMain(" gl_FragColor = color;" "\n");
        fragment_shader.GenerateSource();

        myprogram = new opengl::ShaderProgram;

        myprogram->AttachShader(vertex_shader);
        myprogram->AttachShader(fragment_shader);

        bool status = myprogram->SetupProgram();
        assert(status);
    }
    return myprogram;
}

opengl::ShaderProgram* LightSystemTextShader() {
    static opengl::ShaderProgram* myprogram = NULL;
    if(!myprogram) {
        opengl::Shader vertex_shader(GL_VERTEX_SHADER), fragment_shader(GL_FRAGMENT_SHADER);

        vertex_shader.AddCodeBlock("out vec2 UV;" "\n");
        vertex_shader.AddCodeBlock("out vec2 lightUV;" "\n"
                                   "void calculateLightUV() {" "\n"
                                   "   lightUV = (gl_Position.xy + vec2(1, 1)) * 0.5;" "\n"
                                   "}" "\n");
        vertex_shader.AddLineInMain("	gl_Position =  geometry_matrix * vec4(vertexPosition,0,1);" "\n");
        vertex_shader.AddLineInMain("	UV = vertexUV;" "\n");
        vertex_shader.AddLineInMain("   calculateLightUV();" "\n");
        vertex_shader.GenerateSource();

        fragment_shader.AddCodeBlock("in vec2 UV;" "\n"
                                     "uniform sampler2D drawable_texture;" "\n"
                                     "uniform vec4 effect_color;" "\n");
        fragment_shader.AddCodeBlock("uniform sampler2D light_texture;" "\n");
        fragment_shader.AddLineInMain("	vec4 color = vec4(effect_color.rgb, texture2D( drawable_texture, UV ).a * effect_color.a);" "\n");
        fragment_shader.AddLineInMain("	color *= vec4(light_color, 1.0);" "\n");
        fragment_shader.AddLineInMain(" gl_FragColor = color;" "\n");
        fragment_shader.GenerateSource();

        myprogram = new opengl::ShaderProgram;

        myprogram->AttachShader(vertex_shader);
        myprogram->AttachShader(fragment_shader);

        bool status = myprogram->SetupProgram();
        assert(status);
    }
    return myprogram;
}

opengl::ShaderProgram* LightShader() {
    static opengl::ShaderProgram* myprogram = NULL;
    if(!myprogram) {
        opengl::Shader vertex_shader(GL_VERTEX_SHADER), fragment_shader(GL_FRAGMENT_SHADER);

        vertex_shader.AddCodeBlock("out vec2 lightPosition;" "\n");
        vertex_shader.AddLineInMain("	gl_Position =  geometry_matrix * vec4(vertexPosition*4,0,1);" "\n");
        vertex_shader.AddLineInMain("   lightPosition = vertexPosition*4;" "\n");
        vertex_shader.GenerateSource();

        fragment_shader.AddCodeBlock("in vec2 lightPosition;" "\n"
                                     "uniform vec4 effect_color;" "\n"
                                     "uniform float decayment = 2.4;" "\n"
                                     "uniform float minimum_radius = 0.05;" "\n");
        fragment_shader.AddLineInMain(" gl_FragColor = effect_color * exp(-decayment * (length(lightPosition) - minimum_radius));" "\n");
        fragment_shader.GenerateSource();

        myprogram = new opengl::ShaderProgram;

        myprogram->AttachShader(vertex_shader);
        myprogram->AttachShader(fragment_shader);

        bool status = myprogram->SetupProgram();
        assert(status);
    }
    return myprogram;
}
}  // namespace graphic
}  // namespace ugdk
