#ifndef UGDK_GRAPHIC_VIDEOMANAGER_H_
#define UGDK_GRAPHIC_VIDEOMANAGER_H_

#include <string>
#include <map>
#include <list>
#include <stack>
#include <ugdk/base/types.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/math/frame.h>

#include <ugdk/graphic.h>
#include <ugdk/graphic/modifier.h>

using std::string;
using std::map;

#define VIDEO_MANAGER() (ugdk::Engine::reference()->video_manager())

namespace ugdk {

class Scene;

class VideoManager {
  public:
    static const int COLOR_DEPTH = 32;

    VideoManager() : light_image_(NULL), fullscreen_(false), vsync_(false), light_system_(true), light_texture_(0) {}
    ~VideoManager() {}

    bool Initialize(const string& title, const Vector2D& size, bool fullscreen, const string& icon);
    bool ChangeResolution(const Vector2D& size, bool fullscreen);
    bool Release();
    
    void SetVSync(const bool active);
    void SetLightSystem(const bool active) { light_system_ = active; }
    
    void Render(std::list<Scene*>&, std::list<Node*>&, float dt);

    Texture* LoadTexture(const string& filepath);
    FlexibleSpritesheet* LoadSpritesheet(const std::string& filepath);

    Vector2D video_size() const { return video_size_; }
    bool fullscreen() const { return fullscreen_; }

    const std::string& title() const { return title_; }

    Image* light_image() const { return light_image_; }
    Vector2D light_size() const { return light_size_; }
    Frame virtual_bounds() const { return virtual_bounds_; }

    void PushAndApplyModifier(const Modifier*);
    void PushAndApplyModifier(const Modifier& apply) { PushAndApplyModifier(&apply); }
    bool PopModifier();
    const Modifier& CurrentModifier() const {
        return (modifiers_.empty()) ? Modifier::IDENTITY : modifiers_.top(); 
    }

  private:
    Image *light_image_;
    Vector2D video_size_, light_size_;
    Frame virtual_bounds_;
    bool fullscreen_;
    bool vsync_;
    bool light_system_;
    std::string title_;


    std::map<std::string, Texture*> image_memory_;
    std::map<std::string, FlexibleSpritesheet*> spritesheet_memory_;
    std::stack<Modifier> modifiers_;

    unsigned int light_texture_, light_buffer_id_;

    void InitializeLight();

    void MergeLights(std::list<Scene*>& scene_list);
    void BlendLightIntoBuffer();

    void ClearModiferStack();

};

}  // namespace framework

#endif
