#ifndef UGDK_GRAPHIC_SPRITESHEET_H_
#define UGDK_GRAPHIC_SPRITESHEET_H_

namespace ugdk {

class Spritesheet {
  public:
    Spritesheet() {}
	virtual ~Spritesheet() {}

    virtual void Draw(int frame_number) = 0; //TODO
	virtual int FrameCount() const = 0;
};

}  // namespace framework

#endif
