#include <ugdk/input/textinput.h>

#include <ugdk/action/scene.h>
#include <ugdk/input/events.h>
#include <ugdk/internal/sdleventhandler.h>
#include <ugdk/system/engine.h>

namespace ugdk {
namespace input {

class TextInputSDLEventHandler : public internal::SDLEventHandler {
  public:
      TextInputSDLEventHandler(TextInput& text_input) : text_input_(text_input) {}

    bool CanHandle(const ::SDL_Event& sdlevent) const {
        return sdlevent.type == SDL_TEXTEDITING || sdlevent.type == SDL_TEXTINPUT;
    }

    void Handle(const ::SDL_Event& sdlevent) const {
        if (sdlevent.type == SDL_TEXTEDITING)
            TextEditingHandler(sdlevent);
        else if (sdlevent.type == SDL_TEXTINPUT)
            TextInputHandler(sdlevent);
        else
            assert(false);
    }

    void TextEditingHandler(const ::SDL_Event& sdlevent) const {
        strncpy(text_input_.composition_, sdlevent.edit.text, 32);
        text_input_.cursor_ = sdlevent.edit.start;
        text_input_.selection_size_ = sdlevent.edit.length;

        system::CurrentScene()->event_handler().RaiseEvent(TextEditingEvent(text_input_));
    }
    
    void TextInputHandler(const ::SDL_Event& sdlevent) const {
        system::CurrentScene()->event_handler().RaiseEvent(TextInputEvent(sdlevent.text.text));
    }


  private:
    void operator=(const TextInputSDLEventHandler&);

    TextInput& text_input_;
};

TextInput::TextInput() : event_handler_(new TextInputSDLEventHandler(*this)) {}

TextInput::~TextInput() {}

const internal::SDLEventHandler* TextInput::event_handler() {
    return event_handler_.get();
}

void TextInput::Start(const math::Integer2D& location, const math::Integer2D& size) {
    composition_[0] = '\0';
    cursor_ = 0;
    selection_size_ = 0;

    SDL_Rect rect = { location.x, location.y, size.x, size.y };
    SDL_SetTextInputRect(&rect);
    SDL_StartTextInput();
}

void TextInput::Stop() {
    SDL_StopTextInput();
}

} // namespace input
} // namespace ugdk
