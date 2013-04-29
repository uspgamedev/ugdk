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

        vertex_shader.CompileSource(
"#version 120" "\n"
"#define in attribute" "\n"
"#define out varying" "\n"
// Input vertex data, different for all executions of this shader.
"in vec2 vertexPosition;" "\n"
"in vec2 vertexUV;" "\n"
// Output data ; will be interpolated for each fragment.
"out vec2 UV;" "\n"
// Values that stay constant for the whole mesh.
"uniform mat4 geometry_matrix;" "\n"
"void main() {" "\n"
	// Output position of the vertex, in clip space : MVP * position
"	gl_Position =  geometry_matrix * vec4(vertexPosition,0,1);" "\n"
	// UV of the vertex. No special space for this one.
"	UV = vertexUV;" "\n"
"}");

        fragment_shader.CompileSource(
"#version 120" "\n"
"#define in varying" "\n"
// Interpolated values from the vertex shaders
"in vec2 UV;" "\n"
// Ouput data
// Values that stay constant for the whole mesh.
"uniform sampler2D drawable_texture;" "\n"
"uniform vec4 effect_color;" "\n"
"void main() {" "\n"
	// Output color = color of the texture at the specified UV
"	gl_FragColor = texture2D( drawable_texture, UV ) * effect_color;" "\n"
"}");

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

        vertex_shader.CompileSource(
"#version 120" "\n"
"#define in attribute" "\n"
"#define out varying" "\n"
// Input vertex data, different for all executions of this shader.
"in vec2 vertexPosition;" "\n"
"in vec2 vertexUV;" "\n"
// Output data ; will be interpolated for each fragment.
"out vec2 UV;" "\n"
"out vec2 lightUV;" "\n"
// Values that stay constant for the whole mesh.
"uniform mat4 geometry_matrix;" "\n"
"void main() {" "\n"
	// Output position of the vertex, in clip space : MVP * position
"	gl_Position =  geometry_matrix * vec4(vertexPosition,0,1);" "\n"
"   lightUV = (gl_Position.xy + vec2(1, 1)) * 0.5;" "\n"
	// UV of the vertex. No special space for this one.
"	UV = vertexUV;" "\n"
"}");

        fragment_shader.CompileSource(
"#version 120" "\n"
"#define in varying" "\n"
// Interpolated values from the vertex shaders
"in vec2 UV;" "\n"
"in vec2 lightUV;" "\n"
// Ouput data
// Values that stay constant for the whole mesh.
"uniform sampler2D drawable_texture;" "\n"
"uniform sampler2D light_texture;" "\n"
"uniform vec4 effect_color;" "\n"
"void main() {" "\n"
	// Output color = color of the texture at the specified UV
"   vec3 light_color = texture2D(light_texture, lightUV).rgb;" "\n"
"	gl_FragColor = texture2D(drawable_texture, UV) * effect_color * vec4(light_color, 1.0);" "\n"
"}");

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

        vertex_shader.CompileSource(
"#version 120" "\n"
"#define in attribute" "\n"
"#define out varying" "\n"
// Input vertex data, different for all executions of this shader.
"in vec2 vertexPosition;" "\n"
"in vec2 vertexUV;" "\n"
// Output data ; will be interpolated for each fragment.
"out vec2 UV;" "\n"
// Values that stay constant for the whole mesh.
"uniform mat4 geometry_matrix;" "\n"
"void main() {" "\n"
	// Output position of the vertex, in clip space : MVP * position
"	gl_Position =  geometry_matrix * vec4(vertexPosition,0,1);" "\n"
	// UV of the vertex. No special space for this one.
"	UV = vertexUV;" "\n"
"}");

        fragment_shader.CompileSource(
"#version 120" "\n"
"#define in varying" "\n"
// Interpolated values from the vertex shaders
"in vec2 UV;" "\n"
// Ouput data
// Values that stay constant for the whole mesh.
"uniform sampler2D drawable_texture;" "\n"
"uniform vec4 effect_color;" "\n"
"void main() {" "\n"
	// Output color = color of the texture at the specified UV
"	gl_FragColor = vec4(effect_color.rgb, texture2D( drawable_texture, UV ).a * effect_color.a);" "\n"
"}");

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

        vertex_shader.CompileSource(
"#version 120" "\n"
"#define in attribute" "\n"
"#define out varying" "\n"
// Input vertex data, different for all executions of this shader.
"in vec2 vertexPosition;" "\n"
"in vec2 vertexUV;" "\n"
// Output data ; will be interpolated for each fragment.
"out vec2 UV;" "\n"
"out vec2 lightUV;" "\n"
// Values that stay constant for the whole mesh.
"uniform mat4 geometry_matrix;" "\n"
"void main() {" "\n"
	// Output position of the vertex, in clip space : MVP * position
"	gl_Position =  geometry_matrix * vec4(vertexPosition,0,1);" "\n"
"   lightUV = (gl_Position.xy + vec2(1, 1)) * 0.5;" "\n"
	// UV of the vertex. No special space for this one.
"	UV = vertexUV;" "\n"
"}");

        fragment_shader.CompileSource(
"#version 120" "\n"
"#define in varying" "\n"
// Interpolated values from the vertex shaders
"in vec2 UV;" "\n"
"in vec2 lightUV;" "\n"
// Ouput data
// Values that stay constant for the whole mesh.
"uniform sampler2D drawable_texture;" "\n"
"uniform sampler2D light_texture;" "\n"
"uniform vec4 effect_color;" "\n"
"void main() {" "\n"
	// Output color = color of the texture at the specified UV
"   vec3 light_color = texture2D(light_texture, lightUV).rgb;" "\n"
"	gl_FragColor = vec4(effect_color.rgb * light_color, texture2D( drawable_texture, UV ).a * effect_color.a);" "\n"
"}");

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

        vertex_shader.CompileSource(
"#version 120" "\n"
"#define in attribute" "\n"
"#define out varying" "\n"
// Input vertex data, different for all executions of this shader.
"in vec2 vertexPosition;" "\n"
// Output data ; will be interpolated for each fragment.
"out vec2 lightPosition;" "\n"
// Values that stay constant for the whole mesh.
"uniform mat4 geometry_matrix;" "\n"
"void main() {" "\n"
	// Output position of the vertex, in clip space : MVP * position
"	gl_Position =  geometry_matrix * vec4(vertexPosition*4,0,1);" "\n"
"   lightPosition = vertexPosition*4;" "\n"
"}");

        fragment_shader.CompileSource(
"#version 120" "\n"
"#define in varying" "\n"
// Interpolated values from the vertex shaders
"in vec2 lightPosition;" "\n"
// Ouput data
// Values that stay constant for the whole mesh.
"uniform vec4 effect_color;" "\n"
"uniform float decayment = 2.4;" "\n"
"uniform float minimum_radius = 0.05;" "\n"
"void main() {" "\n"
	// Output color = color of the texture at the specified UV
"	gl_FragColor = effect_color * exp(-decayment * (length(lightPosition) - minimum_radius));" "\n"
"}");

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
