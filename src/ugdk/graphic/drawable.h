#ifndef HORUSEYE_FRAMEWORK_DRAWABLE_H_
#define HORUSEYE_FRAMEWORK_DRAWABLE_H_

#include <ugdk/math/vector2D.h>
#include <ugdk/base/types.h>
#include <ugdk/graphic/modifier.h>

namespace ugdk {

class Drawable {
  public:
	virtual ~Drawable() {}
	virtual bool DrawTo(const Vector2D& position, int frame_number, uint8 mirror, 
		const Color& color, float alpha, const Vector2D& draw_size) {
			return false;
	}
	virtual bool DrawTo(const Vector2D& position, int frame_number, Modifier *modifier,
        const Vector2D& draw_size) {
	    if (modifier != NULL)
            return DrawTo(position - modifier->offset(), frame_number, modifier->mirror(),
                      modifier->color(), modifier->alpha(), draw_size);
	    else
	        return DrawTo(position, frame_number, MIRROR_NONE,
                      WHITE, 1.0f, draw_size);
    }

	virtual Vector2D render_size() const { return Vector2D(); }

	// DEPRECATED DO NOT USE OR DIE LOLOL
	virtual int width() const { return (int)(render_size().x); }
	virtual int height() const { return (int)(render_size().y); }
	virtual bool Destroy() { return false; }

  protected:
	Drawable() {}
};

}  // namespace framework

#endif
