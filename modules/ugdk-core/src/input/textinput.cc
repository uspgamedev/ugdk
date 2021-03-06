#include <ugdk/input/textinput.h>

#include <ugdk/action/scene.h>
#include <ugdk/input/events.h>
#include <ugdk/system/engine.h>

#include <system/sdleventhandler.h>

#include <cstring>

namespace ugdk {
namespace input {

class TextInputSDLEventHandler : public system::SDLEventHandler {
  public:
      TextInputSDLEventHandler(TextInput& text_input) : text_input_(text_input) {}

    std::unordered_set<Uint32> TypesHandled() const override {
        return { SDL_TEXTEDITING, SDL_TEXTINPUT };
    }

    void Handle(const ::SDL_Event& sdlevent) const override {
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

        system::EventHandler::RaiseGlobalEvent(TextEditingEvent(text_input_));
    }
    
    void TextInputHandler(const ::SDL_Event& sdlevent) const {
        system::EventHandler::RaiseGlobalEvent(TextInputEvent(sdlevent.text.text));
    }


  private:
    void operator=(const TextInputSDLEventHandler&);

    TextInput& text_input_;
};

TextInput::TextInput() : event_handler_(new TextInputSDLEventHandler(*this)) {}

TextInput::~TextInput() {}

const system::SDLEventHandler* TextInput::event_handler() {
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
