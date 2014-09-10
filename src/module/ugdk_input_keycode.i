%module ugdk_input_keycode

%{

#include <ugdk/input/scancode.h>
#include <ugdk/input/keycode.h>

%}

// SWIG generates code that doesn't compile
%ignore CreateScancodeFromKeycode(const Keycode&);
%ignore CreateKeycodeFromScancode(const Scancode&);

%import(module="ugdk_input_scancode") <ugdk/input/scancode.h>
%include <ugdk/input/keycode.h>

