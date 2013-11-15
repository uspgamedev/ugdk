#ifndef UGDK_WINDOW_VIDEOMANAGER_H_
#define UGDK_WINDOW_VIDEOMANAGER_H_

#include <string>
#include <list>
#include <bitset>
#include <functional>
#include <ugdk/structure/types.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/math/integer2D.h>
#include <ugdk/math/frame.h>
#include <ugdk/window.h>

struct SDL_Window;

namespace ugdk {
namespace window {

struct WindowSettings {
    std::string window_title;
    std::string window_icon;
    math::Integer2D resolution;
    bool fullscreen;
    bool vsync;

    WindowSettings();
    WindowSettings(const std::string& title,
                  const std::string& icon,
                  const math::Integer2D& _resolution, 
                  bool _fullscreen,
                  bool _vsync)
        : 
        window_title(title),
        window_icon(icon),
        resolution(_resolution),
        fullscreen(_fullscreen),
        vsync(_vsync) {}
};

class Manager {
  public:
    static const int COLOR_DEPTH = 32;

    Manager(const WindowSettings& settings);
    ~Manager() {}

    bool Initialize();
    void Release();

    /// Presents the current canvas in the screen.
    void Present();

    /// Updates the settings and applies the changes.
    /** Warning: changing the resolution and/or fullscreen is a slow operation. */
    bool ChangeSettings(const WindowSettings& new_settings);

    /// Convenience
    const math::Integer2D& video_size() const { return settings_.resolution; }

    // Getters
    const WindowSettings& settings() const { return settings_; }

  private:
    bool UpdateResolution();
    void UpdateVSync();

    WindowSettings settings_;

    SDL_Window* window_;
};

}  // namespace window
}  // namespace ugdk

#endif
