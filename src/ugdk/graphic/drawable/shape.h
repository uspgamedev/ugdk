#ifndef UGDK_GRAPHIC_DRAWABLE_SHAPE_H_
#define UGDK_GRAPHIC_DRAWABLE_SHAPE_H_

#include <ugdk/graphic/drawable.h>

namespace ugdk {

class Shape : public Drawable {
  public:
    virtual ~Shape() = 0;

    virtual void Draw() = 0;
    virtual Vector2D render_size() const = 0;
    
  protected:
    Shape() {}
};

}  // namespace framework

#endif
