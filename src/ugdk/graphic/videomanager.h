#ifndef UGDK_GRAPHIC_VIDEOMANAGER_H_
#define UGDK_GRAPHIC_VIDEOMANAGER_H_

#include <string>
#include <list>
#include <ugdk/base/types.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/math/frame.h>
#include <ugdk/action.h>
#include <ugdk/graphic.h>

#define VIDEO_MANAGER() (ugdk::Engine::reference()->video_manager())

namespace ugdk {
namespace graphic {

class VideoManager {
  public:
    static const int COLOR_DEPTH = 32;

    VideoManager() : settings_(false, false, false), light_buffer_(NULL), light_texture_(NULL) {}
    ~VideoManager() {}

    bool Initialize(const std::string& title, const ugdk::math::Vector2D& size, bool fullscreen, const std::string& icon);
    bool Release();
    void Render(const std::list<action::Scene*>&);

    // Configuration
    bool ChangeResolution(const ugdk::math::Vector2D& size, bool fullscreen);
    void SetVSync(const bool active);
    void SetLightSystem(const bool active) { settings_.light_system = active; }

    // Getters
    ugdk::math::Vector2D video_size() const { return video_size_; }
    bool fullscreen() const { return settings_.fullscreen; }
    const std::string& title() const { return title_; }
    const Texture* light_texture() const { return light_texture_; }
    math::Frame virtual_bounds() const { return virtual_bounds_; }

  private:
    ugdk::math::Vector2D video_size_;
    math::Frame virtual_bounds_;
    std::string title_;

    struct Settings {
        bool fullscreen;
        bool vsync;
        bool light_system;

        Settings(bool fs, bool vs, bool light) : fullscreen(fs), vsync(vs), light_system(light) {}
    } settings_;

    Texture* light_buffer_;
    Texture* light_texture_;

    void InitializeLight();

    void mergeLights(const std::list<action::Scene*>& scene_list);
    void BlendLightIntoBuffer();
};

}  // namespace graphic
}  // namespace ugdk

#endif
