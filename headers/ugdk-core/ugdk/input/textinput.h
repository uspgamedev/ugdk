#ifndef UGDK_INPUT_TEXTINPUT_H_
#define UGDK_INPUT_TEXTINPUT_H_

#include <ugdk/system.h>
#include <ugdk/math.h>
#include <string>
#include <memory>

namespace ugdk {
namespace input {

class TextInput {
  public:
    void Start(const math::Integer2D& candidate_list_location, const math::Integer2D& candidate_list_size);
    void Stop();

    const char* current_composition() const { return composition_; }
    int cursor() const { return cursor_; }
    int selection_size() const { return selection_size_; }

  private:
    TextInput();
    ~TextInput();

    const system::SDLEventHandler* event_handler();

    std::unique_ptr<system::SDLEventHandler> event_handler_;
    char composition_[32];
    int cursor_;
    int selection_size_;

    friend class Manager;
    friend class TextInputSDLEventHandler;
};

} // namespace input
} // namespace ugdk

#endif // UGDK_INPUT_TEXTINPUT_H_
