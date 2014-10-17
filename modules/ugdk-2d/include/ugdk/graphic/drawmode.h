#ifndef UGDK_GRAPHIC_DRAWMODE_H_
#define UGDK_GRAPHIC_DRAWMODE_H_

namespace ugdk {
namespace graphic {

class DrawMode {
  public:
    static DrawMode POINTS();
    static DrawMode LINE_STRIP();
    static DrawMode LINE_LOOP();
    static DrawMode LINES();
    static DrawMode LINE_STRIP_ADJACENCY();
    static DrawMode LINES_ADJACENCY();
    static DrawMode TRIANGLE_STRIP();
    static DrawMode TRIANGLE_FAN();
    static DrawMode TRIANGLES();
    static DrawMode TRIANGLE_STRIP_ADJACENCY();
    static DrawMode TRIANGLES_ADJACENCY();

    int mode() const;

  private:
    DrawMode(int);

    int mode_;
};

}  // namespace graphic
}  // namespace ugdk

#endif // UGDK_GRAPHIC_DRAWMODE_H_
