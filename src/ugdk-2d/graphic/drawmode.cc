#include <ugdk/graphic/drawmode.h>

#include <ugdk/internal/opengl.h>

namespace ugdk {
namespace graphic {

DrawMode DrawMode::POINTS() { 
    return DrawMode(GL_POINTS);
}

DrawMode DrawMode::LINE_STRIP() {
    return DrawMode(GL_LINE_STRIP);
}

DrawMode DrawMode::LINE_LOOP() {
    return DrawMode(GL_LINE_LOOP);
}

DrawMode DrawMode::LINES() {
    return DrawMode(GL_LINES);
}

DrawMode DrawMode::LINE_STRIP_ADJACENCY() {
    return DrawMode(GL_LINE_STRIP_ADJACENCY);
}

DrawMode DrawMode::LINES_ADJACENCY() {
    return DrawMode(GL_LINES_ADJACENCY);
}

DrawMode DrawMode::TRIANGLE_STRIP() {
    return DrawMode(GL_TRIANGLE_STRIP);
}

DrawMode DrawMode::TRIANGLE_FAN() {
    return DrawMode(GL_TRIANGLE_FAN);
}

DrawMode DrawMode::TRIANGLES() {
    return DrawMode(GL_TRIANGLES);
}

DrawMode DrawMode::TRIANGLE_STRIP_ADJACENCY() {
    return DrawMode(GL_TRIANGLE_STRIP_ADJACENCY);
}

DrawMode DrawMode::TRIANGLES_ADJACENCY() {
    return DrawMode(GL_TRIANGLES_ADJACENCY);
}

int DrawMode::mode() const {
    return mode_;
}

DrawMode::DrawMode(int mode)
: mode_(mode)
{}

}  // namespace graphic
}  // namespace ugdk
