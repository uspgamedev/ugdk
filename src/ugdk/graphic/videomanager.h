#ifndef HORUSEYE_FRAMEWORK_VIDEOMANAGER_H_
#define HORUSEYE_FRAMEWORK_VIDEOMANAGER_H_

#include <string>
#include <map>
#include <vector>
#include <list>
#include <stack>
#include <ugdk/base/types.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/math/frame.h>
#include <ugdk/graphic/modifier.h>

using std::string;
using std::map;

#define VIDEO_MANAGER() (ugdk::Engine::reference()->video_manager())

namespace ugdk {
class Image;
class Scene;
class Layer;
class Modifier;

class VideoManager {
  public:
    static const int COLOR_DEPTH = 32;

    VideoManager() : light_image_(NULL), fullscreen_(false), light_texture_(0) {}
    ~VideoManager() {}

    bool Initialize(const string& title, const Vector2D& size, bool fullscreen, const string& icon);
    bool ChangeResolution(const Vector2D& size, bool fullscreen);
    bool Release();
    void Render(std::vector<Scene*>, std::list<Layer*>);

    Image* LoadImageFile(const string& filepath);
    Image* LoadImage(const string& filepath) {
        return LoadImageFile(filepath);
    }

    Vector2D video_size() const { return video_size_; }
    bool fullscreen() const { return fullscreen_; }
    string title() const { return title_; }
    Image* backbuffer() const { return NULL; }
    Image* screen() const { return NULL; }
    Image* blank_image() const { return blank_image_; }
    Image* light_image() const { return light_image_; }
    Vector2D light_size() const { return light_size_; }
    Frame virtual_bounds() const { return virtual_bounds_; }

    void TranslateTo (Vector2D& offset);

    void PushAndApplyModifier(Modifier*);
    bool PopModifier();

  private:
    Image *blank_image_, *light_image_;
    Vector2D video_size_, light_size_;
    Frame virtual_bounds_;
    bool fullscreen_;
    string title_;
    map<string, Image*> image_memory_;
    std::stack<Modifier> modifiers_;

    unsigned int light_texture_, light_buffer_id_;

    void InitializeLight();


    void MergeLights(std::vector<Scene*> scene_list);
    void BlendLightIntoBuffer();

};

}  // namespace framework

#endif
