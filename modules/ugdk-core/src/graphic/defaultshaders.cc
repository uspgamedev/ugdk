#include <ugdk/graphic/defaultshaders.h>

#include <cassert>

#include <ugdk/graphic/opengl.h>
#include <ugdk/graphic/shader.h>
#include <ugdk/graphic/shaderprogram.h>

#include <ugdk/system/config.h>

namespace ugdk {
namespace graphic {

ShaderProgram* DEFAULT_SHADERS[4] = { nullptr, nullptr, nullptr, nullptr };

ShaderProgram* CreateShader(bool light_system, bool color_text_mode) {
    Shader vertex_shader(GL_VERTEX_SHADER), fragment_shader(GL_FRAGMENT_SHADER);

    // VERTEX
    vertex_shader.AddCodeBlock("out highp vec2 UV;" "\n");
    if(light_system)
        vertex_shader.AddCodeBlock("out highp vec2 lightUV;" "\n"
                                   "void calculateLightUV() {" "\n"
                                   "   lightUV = (gl_Position.xy + vec2(1, 1)) * 0.5;" "\n"
                                   "}" "\n");

    vertex_shader.AddLineInMain("	gl_Position =  geometry_matrix * vec4(vertexPosition,0,1);" "\n");
    vertex_shader.AddLineInMain("	UV = vertexUV;" "\n");
    if(light_system)
        vertex_shader.AddLineInMain("   calculateLightUV();" "\n");
    vertex_shader.GenerateSource();

    // FRAGMENT
    fragment_shader.AddCodeBlock("in highp vec2 UV;" "\n"
                                 "uniform highp sampler2D drawable_texture;" "\n"
                                 "uniform highp vec4 effect_color;" "\n");

    if(light_system)
        fragment_shader.AddCodeBlock("in highp vec2 lightUV;" "\n"
                                     "uniform highp sampler2D light_texture;" "\n");

    if(color_text_mode)
        fragment_shader.AddLineInMain("	highp vec4 color = vec4(effect_color.rgb, texture2D( drawable_texture, UV ).a * effect_color.a);" "\n");
    else
        fragment_shader.AddLineInMain("	highp vec4 color = texture2D( drawable_texture, UV ) * effect_color;" "\n");
    if(light_system)
        fragment_shader.AddLineInMain("	color *= vec4(texture2D(light_texture, lightUV).rgb, 1.0);" "\n");
    fragment_shader.AddLineInMain(" gl_FragColor = color;" "\n");
    fragment_shader.GenerateSource();

    ShaderProgram* myprogram = new ShaderProgram;

    myprogram->AttachShader(vertex_shader);
    myprogram->AttachShader(fragment_shader);

    bool status = myprogram->SetupProgram();
    assert(status);
    return myprogram;
}

ShaderProgram* InterfaceShader() {
    if(DEFAULT_SHADERS[0*2 + 0]) return DEFAULT_SHADERS[0*2 + 0];
    return DEFAULT_SHADERS[0 + 0] = CreateShader(false, false);
}

ShaderProgram* LightSystemShader() {
    if(DEFAULT_SHADERS[0*2 + 1]) return DEFAULT_SHADERS[0*2 + 1];
    return DEFAULT_SHADERS[0 + 1] = CreateShader(true, false);
}

ShaderProgram* InterfaceTextShader() {
    if(DEFAULT_SHADERS[1*2 + 0]) return DEFAULT_SHADERS[1*2 + 0];
    return DEFAULT_SHADERS[1*2 + 0] = CreateShader(false, true);
}

ShaderProgram* LightSystemTextShader() {
    if(DEFAULT_SHADERS[1*2 + 1]) return DEFAULT_SHADERS[1*2 + 1];
    return DEFAULT_SHADERS[1*2 + 1] = CreateShader(true, true);
}

ShaderProgram* LightShader() {
    static ShaderProgram* myprogram = nullptr;
    if(!myprogram) {
        Shader vertex_shader(GL_VERTEX_SHADER), fragment_shader(GL_FRAGMENT_SHADER);

        vertex_shader.AddCodeBlock("out highp vec2 lightPosition;" "\n"
                                   "uniform highp vec2 dimension;" "\n"
                                   );
        vertex_shader.AddLineInMain("	gl_Position =  geometry_matrix * vec4(vertexPosition * dimension,0,1);" "\n");
        vertex_shader.AddLineInMain("   lightPosition = vertexPosition;" "\n");
        vertex_shader.GenerateSource();

        fragment_shader.AddCodeBlock("in highp vec2 lightPosition;" "\n"
                                     "uniform highp vec4 effect_color;" "\n"
                                     "uniform highp float decayment;" "\n"
                                     "uniform highp float minimum_radius;" "\n");
        fragment_shader.AddLineInMain(" gl_FragColor = effect_color * exp(-decayment * (length(lightPosition) - minimum_radius));" "\n");
        fragment_shader.GenerateSource();

        myprogram = new ShaderProgram;

        myprogram->AttachShader(vertex_shader);
        myprogram->AttachShader(fragment_shader);

        bool status = myprogram->SetupProgram();
        assert(status);
    }
    return myprogram;
}
}  // namespace graphic
}  // namespace ugdk
