/*
  Header adapted from SDL_scancode.h
  Original license is the following:

  Simple DirectMedia Layer
  Copyright (C) 1997-2013 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

/**
 *  Defines keyboard scancodes.
 */

#ifndef UGDK_INPUT_SCANCODE_H_
#define UGDK_INPUT_SCANCODE_H_

namespace ugdk {
namespace input {

/**
 *  \brief The SDL keyboard scancode representation.
 *
 *  Values of this type are used to represent keyboard keys, among other places
 *  in the \link SDL_Keysym::scancode key.keysym.scancode \endlink field of the
 *  SDL_Event structure.
 *
 *  The values in this enumeration are based on the USB usage page standard:
 *  http://www.usb.org/developers/devclass_docs/Hut1_12v2.pdf
 */
#ifndef SWIG
enum class Scancode
{
    UNKNOWN = 0,

    /**
     *  \name Usage page 0x07
     *
     *  These values are from usage page 0x07 (USB keyboard page).
     */
    /*@{*/

    A = 4,
    B = 5,
    C = 6,
    D = 7,
    E = 8,
    F = 9,
    G = 10,
    H = 11,
    I = 12,
    J = 13,
    K = 14,
    L = 15,
    M = 16,
    N = 17,
    O = 18,
    P = 19,
    Q = 20,
    R = 21,
    S = 22,
    T = 23,
    U = 24,
    V = 25,
    W = 26,
    X = 27,
    Y = 28,
    Z = 29,


    ALPHANUMERIC_1 = 30, 
    ALPHANUMERIC_2 = 31,
    ALPHANUMERIC_3 = 32,
    ALPHANUMERIC_4 = 33,
    ALPHANUMERIC_5 = 34,
    ALPHANUMERIC_6 = 35,
    ALPHANUMERIC_7 = 36,
    ALPHANUMERIC_8 = 37,
    ALPHANUMERIC_9 = 38,
    ALPHANUMERIC_0 = 39,

    RETURN = 40,
    ESCAPE = 41,
    BACKSPACE = 42,
    TAB = 43,
    SPACE = 44,

    MINUS = 45,
    EQUALS = 46,
    LEFTBRACKET = 47,
    RIGHTBRACKET = 48,
    BACKSLASH = 49, /**< Located at the lower left of the return
                     *   key on ISO keyboards and at the right end
                     *   of the QWERTY row on ANSI keyboards.
                     *   Produces REVERSE SOLIDUS (backslash) and
                     *   VERTICAL LINE in a US layout, REVERSE
                     *   SOLIDUS and VERTICAL LINE in a UK Mac
                     *   layout, NUMBER SIGN and TILDE in a UK
                     *   Windows layout, DOLLAR SIGN and POUND SIGN
                     *   in a Swiss German layout, NUMBER SIGN and
                     *   APOSTROPHE in a German layout, GRAVE
                     *   ACCENT and POUND SIGN in a French Mac
                     *   layout, and ASTERISK and MICRO SIGN in a
                     *   French Windows layout.
                     */
    NONUSHASH = 50, /**< ISO USB keyboards actually use this code
                     *   instead of 49 for the same key, but all
                     *   OSes I've seen treat the two codes
                     *   identically. So, as an implementor, unless
                     *   your keyboard generates both of those
                     *   codes and your OS treats them differently,
                     *   you should generate BACKSLASH
                     *   instead of this code. As a user, you
                     *   should not rely on this code because SDL
                     *   will never generate it with most (all?)
                     *   keyboards.
                     */
    SEMICOLON = 51,
    APOSTROPHE = 52,
    GRAVE = 53, /**< Located in the top left corner (on both ANSI
                 *   and ISO keyboards). Produces GRAVE ACCENT and
                 *   TILDE in a US Windows layout and in US and UK
                 *   Mac layouts on ANSI keyboards, GRAVE ACCENT
                 *   and NOT SIGN in a UK Windows layout, SECTION
                 *   SIGN and PLUS-MINUS SIGN in US and UK Mac
                 *   layouts on ISO keyboards, SECTION SIGN and
                 *   DEGREE SIGN in a Swiss German layout (Mac:
                 *   only on ISO keyboards), CIRCUMFLEX ACCENT and
                 *   DEGREE SIGN in a German layout (Mac: only on
                 *   ISO keyboards), SUPERSCRIPT TWO and TILDE in a
                 *   French Windows layout, COMMERCIAL AT and
                 *   NUMBER SIGN in a French Mac layout on ISO
                 *   keyboards, and LESS-THAN SIGN and GREATER-THAN
                 *   SIGN in a Swiss German, German, or French Mac
                 *   layout on ANSI keyboards.
                 */
    COMMA = 54,
    PERIOD = 55,
    SLASH = 56,

    CAPSLOCK = 57,

    F1 = 58,
    F2 = 59,
    F3 = 60,
    F4 = 61,
    F5 = 62,
    F6 = 63,
    F7 = 64,
    F8 = 65,
    F9 = 66,
    F10 = 67,
    F11 = 68,
    F12 = 69,

    PRINTSCREEN = 70,
    SCROLLLOCK = 71,
    PAUSE = 72,
    INSERT = 73, /**< insert on PC, help on some Mac keyboards (but
                      does send code 73, not 117) */
    HOME = 74,
    PAGEUP = 75,
    DELETE = 76,
    END = 77,
    PAGEDOWN = 78,
    RIGHT = 79,
    LEFT = 80,
    DOWN = 81,
    UP = 82,

    NUMLOCKCLEAR = 83, /**< num lock on PC, clear on Mac keyboards
                                     */
    NUMPAD_DIVIDE = 84,
    NUMPAD_MULTIPLY = 85,
    NUMPAD_MINUS = 86,
    NUMPAD_PLUS = 87,
    NUMPAD_ENTER = 88,
    NUMPAD_1 = 89,
    NUMPAD_2 = 90,
    NUMPAD_3 = 91,
    NUMPAD_4 = 92,
    NUMPAD_5 = 93,
    NUMPAD_6 = 94,
    NUMPAD_7 = 95,
    NUMPAD_8 = 96,
    NUMPAD_9 = 97,
    NUMPAD_0 = 98,
    NUMPAD_PERIOD = 99,

    NONUSBACKSLASH = 100, /**< This is the additional key that ISO
                           *   keyboards have over ANSI ones,
                           *   located between left shift and Y.
                           *   Produces GRAVE ACCENT and TILDE in a
                           *   US or UK Mac layout, REVERSE SOLIDUS
                           *   (backslash) and VERTICAL LINE in a
                           *   US or UK Windows layout, and
                           *   LESS-THAN SIGN and GREATER-THAN SIGN
                           *   in a Swiss German, German, or French
                           *   layout. */
    APPLICATION = 101, /**< windows contextual menu, compose */
    POWER = 102, /**< The USB document says this is a status flag,
                  *   not a physical key - but some Mac keyboards
                  *   do have a power key. */
    NUMPAD_EQUALS = 103,
    F13 = 104,
    F14 = 105,
    F15 = 106,
    F16 = 107,
    F17 = 108,
    F18 = 109,
    F19 = 110,
    F20 = 111,
    F21 = 112,
    F22 = 113,
    F23 = 114,
    F24 = 115,
    EXECUTE = 116,
    HELP = 117,
    MENU = 118,
    SELECT = 119,
    STOP = 120,
    AGAIN = 121,   /**< redo */
    UNDO = 122,
    CUT = 123,
    COPY = 124,
    PASTE = 125,
    FIND = 126,
    MUTE = 127,
    VOLUMEUP = 128,
    VOLUMEDOWN = 129,
/* not sure whether there's a reason to enable these */
/*     LOCKINGCAPSLOCK = 130,  */
/*     LOCKINGNUMLOCK = 131, */
/*     LOCKINGSCROLLLOCK = 132, */
    NUMPAD_COMMA = 133,
    NUMPAD_EQUALSAS400 = 134,

    INTERNATIONAL1 = 135, /**< used on Asian keyboards, see
                               footnotes in USB doc */
    INTERNATIONAL2 = 136,
    INTERNATIONAL3 = 137, /**< Yen */
    INTERNATIONAL4 = 138,
    INTERNATIONAL5 = 139,
    INTERNATIONAL6 = 140,
    INTERNATIONAL7 = 141,
    INTERNATIONAL8 = 142,
    INTERNATIONAL9 = 143,
    LANG1 = 144, /**< Hangul/English toggle */
    LANG2 = 145, /**< Hanja conversion */
    LANG3 = 146, /**< Katakana */
    LANG4 = 147, /**< Hiragana */
    LANG5 = 148, /**< Zenkaku/Hankaku */
    LANG6 = 149, /**< reserved */
    LANG7 = 150, /**< reserved */
    LANG8 = 151, /**< reserved */
    LANG9 = 152, /**< reserved */

    ALTERASE = 153, /**< Erase-Eaze */
    SYSREQ = 154,
    CANCEL = 155,
    CLEAR = 156,
    PRIOR = 157,
    RETURN2 = 158,
    SEPARATOR = 159,
    OUT = 160,
    OPER = 161,
    CLEARAGAIN = 162,
    CRSEL = 163,
    EXSEL = 164,

    NUMPAD_00 = 176,
    NUMPAD_000 = 177,
    THOUSANDSSEPARATOR = 178,
    DECIMALSEPARATOR = 179,
    CURRENCYUNIT = 180,
    CURRENCYSUBUNIT = 181,
    NUMPAD_LEFTPAREN = 182,
    NUMPAD_RIGHTPAREN = 183,
    NUMPAD_LEFTBRACE = 184,
    NUMPAD_RIGHTBRACE = 185,
    NUMPAD_TAB = 186,
    NUMPAD_BACKSPACE = 187,
    NUMPAD_A = 188,
    NUMPAD_B = 189,
    NUMPAD_C = 190,
    NUMPAD_D = 191,
    NUMPAD_E = 192,
    NUMPAD_F = 193,
    NUMPAD_XOR = 194,
    NUMPAD_POWER = 195,
    NUMPAD_PERCENT = 196,
    NUMPAD_LESS = 197,
    NUMPAD_GREATER = 198,
    NUMPAD_AMPERSAND = 199,
    NUMPAD_DBLAMPERSAND = 200,
    NUMPAD_VERTICALBAR = 201,
    NUMPAD_DBLVERTICALBAR = 202,
    NUMPAD_COLON = 203,
    NUMPAD_HASH = 204,
    NUMPAD_SPACE = 205,
    NUMPAD_AT = 206,
    NUMPAD_EXCLAM = 207,
    NUMPAD_MEMSTORE = 208,
    NUMPAD_MEMRECALL = 209,
    NUMPAD_MEMCLEAR = 210,
    NUMPAD_MEMADD = 211,
    NUMPAD_MEMSUBTRACT = 212,
    NUMPAD_MEMMULTIPLY = 213,
    NUMPAD_MEMDIVIDE = 214,
    NUMPAD_PLUSMINUS = 215,
    NUMPAD_CLEAR = 216,
    NUMPAD_CLEARENTRY = 217,
    NUMPAD_BINARY = 218,
    NUMPAD_OCTAL = 219,
    NUMPAD_DECIMAL = 220,
    NUMPAD_HEXADECIMAL = 221,

    LCTRL = 224,
    LSHIFT = 225,
    LALT = 226, /**< alt, option */
    LGUI = 227, /**< windows, command (apple), meta */
    RCTRL = 228,
    RSHIFT = 229,
    RALT = 230, /**< alt gr, option */
    RGUI = 231, /**< windows, command (apple), meta */

    MODE = 257,    /**< I'm not sure if this is really not covered
                    *   by any of the above, but since there's a
                    *   special KMOD_MODE for it I'm adding it here
                    */

    /*@}*//*Usage page 0x07*/

    /**
     *  \name Usage page 0x0C
     *
     *  These values are mapped from usage page 0x0C (USB consumer page).
     */
    /*@{*/

    AUDIONEXT = 258,
    AUDIOPREV = 259,
    AUDIOSTOP = 260,
    AUDIOPLAY = 261,
    AUDIOMUTE = 262,
    MEDIASELECT = 263,
    WWW = 264,
    MAIL = 265,
    CALCULATOR = 266,
    COMPUTER = 267,
    AC_SEARCH = 268,
    AC_HOME = 269,
    AC_BACK = 270,
    AC_FORWARD = 271,
    AC_STOP = 272,
    AC_REFRESH = 273,
    AC_BOOKMARKS = 274,

    /*@}*//*Usage page 0x0C*/

    /**
     *  \name Walther keys
     *
     *  These are values that Christian Walther added (for mac keyboard?).
     */
    /*@{*/

    BRIGHTNESSDOWN = 275,
    BRIGHTNESSUP = 276,
    DISPLAYSWITCH = 277, /**< display mirroring/dual display
                                           switch, video mode switch */
    KBDILLUMTOGGLE = 278,
    KBDILLUMDOWN = 279,
    KBDILLUMUP = 280,
    EJECT = 281,
    SLEEP = 282,

    APP1 = 283,
    APP2 = 284,

    /*@}*//*Walther keys*/

    /* Add any other keys here. */

    SDL_NUM_SCANCODES = 512 /**< not a key, just marks the number of scancodes
                                 for array bounds */
};
#else
struct Scancode {};
#endif


} // namespace input
} // namespace ugdk

#endif // UGDK_INPUT_SCANCODE_H_
