#ifndef UGDK_GRAPHIC_DRAWABLE_SHAPE_H_
#define UGDK_GRAPHIC_DRAWABLE_SHAPE_H_

#include <ugdk/graphic/drawable.h>

namespace ugdk {

class Shape : public Drawable {
  public:
    virtual ~Shape() {};
    
  protected:
    Shape() {}
};

}  // namespace framework

#endif
