#ifndef UGDK_GRAPHIC_MANAGER_H_
#define UGDK_GRAPHIC_MANAGER_H_

#include <ugdk/structure/types.h>
#include <ugdk/math/vector2D.h>

#include <ugdk/action.h>
#include <ugdk/graphic.h>
#include <ugdk/desktop.h>
#include <ugdk/util.h>

#include <ugdk/graphic/textureunit.h>

#include <string>
#include <bitset>
#include <functional>
#include <memory>

struct SDL_Window;
typedef void* SDL_GLContext;

namespace ugdk {
namespace graphic {

class RenderScreen;
class Renderer;
action::Scene* CreateLightrenderingScene(std::function<void (Canvas&)> render_light_function);

enum class VertexType {
    VERTEX, TEXTURE, COLOR, CUSTOM1
};

class Manager {
  public:
    Manager();
    ~Manager();
    
    bool Initialize(const std::vector<std::weak_ptr<desktop::Window>>& windows_, 
                    const math::Vector2D& canvas_size);
    void Release();

    uint32_t RegisterScreen(std::weak_ptr<desktop::Window>);
    /*WE NEED TO ADD A METHOD FOR RENDERTEXTURES*/

    void UnregisterTarget(uint32_t index);
    /*
    void UseCanvas(graphic::Canvas &);
    void FreeCanvas(graphic::Canvas &);
    */
    void ResizeTarget(uint32_t index, const math::Vector2D& canvas_size);

    void SetUserNearestNeighborTextures(bool enabled);

    void SetActiveRenderer(std::weak_ptr<Renderer>);
    std::weak_ptr<Renderer> ActiveRenderer();

    TextureUnit ReserveTextureUnit(const graphic::GLTexture* texture = nullptr);
    void DisableVertexType(VertexType);
    unsigned int LocationForVertexType(VertexType);

    class Shaders {
      public:
        enum Flag {
            USE_LIGHT_BUFFER = 0,
            IGNORE_TEXTURE_COLOR,

            NUM_FLAGS // Value generated by the compiler
        };
    
        const ShaderProgram* current_shader() const;
        
        const ShaderProgram* GetSpecificShader(const std::bitset<NUM_FLAGS>& flags) const;

        bool IsFlagSet(Flag) const;
        void ChangeFlag(Flag, bool);

        /// Replace the ShaderProgram UGDK uses for the given combination of flags.
        /** @arg flags A STL bitset containing the flags.
            @arg program The ShaderProgram to use. This assumes you're passing the ownership of the object.
        */
        void ReplaceShader(const std::bitset<NUM_FLAGS>& flags, ShaderProgram* program);
        
        void ReplaceShader(unsigned long flags, ShaderProgram* program) {
            ReplaceShader(std::bitset<NUM_FLAGS>(flags), program);
        }

        void ReplaceShader(const std::string& flags, ShaderProgram* program) {
            ReplaceShader(std::bitset<NUM_FLAGS>(flags), program);
        }

      private:
        Shaders();
        ~Shaders();

        ShaderProgram* shaders_[1 << NUM_FLAGS];
        std::bitset<NUM_FLAGS> flags_;

        friend class Manager;
    };

    RenderTarget* target(uint32_t index) const;
    uint32_t num_targets();
    RenderTexture* light_buffer() const { return light_buffer_.get(); }

    graphic::GLTexture* white_texture() { return white_texture_; }
    Shaders& shaders() { return shaders_; }
    const Shaders& shaders() const { return shaders_; }
    ShaderProgram* light_shader() { return light_shader_; }

  private:
    void CreateLightBuffer(const math::Vector2D& size);
    void ReleaseTextureUnitID(int id);

    SDL_GLContext context_;
    std::vector<std::unique_ptr<RenderTarget> > targets_;
    std::unique_ptr<RenderTexture> light_buffer_;
    std::unique_ptr<util::IDGenerator> textureunit_ids_;
    graphic::GLTexture* white_texture_;
    
    Shaders shaders_;
    ShaderProgram* light_shader_;
    uint32_t active_index_;

    std::weak_ptr<Renderer> current_renderer_;

    friend class ::ugdk::graphic::TextureUnit;
};

}  // namespace graphic
}  // namespace ugdk

#endif // UGDK_GRAPHIC_MANAGER_H_
