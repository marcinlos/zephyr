
#include <zephyr/input/Key.hpp>

namespace {

const char* strings[] = {
    "A",
    "B",
    "C",
    "D",
    "E",
    "F",
    "G",
    "H",
    "I",
    "J",
    "K",
    "L",
    "M",
    "N",
    "O",
    "P",
    "Q",
    "R",
    "S",
    "T",
    "U",
    "V",
    "W",
    "X",
    "Y",
    "Z",
    "K0",
    "K1",
    "K2",
    "K3",
    "K4",
    "K5",
    "K6",
    "K7",
    "K8",
    "K9",
    "F1",
    "F2",
    "F3",
    "F4",
    "F5",
    "F6",
    "F7",
    "F8",
    "F9",
    "F10",
    "F11",
    "F12",
    "LEFT_CTRL",
    "RIGHT_CTRL",
    "LEFT_ALT",
    "RIGHT_ALT",
    "LEFT_SHIFT",
    "RIGHT_SHIFT",
    "MENU",
    "SPACE",
    "TAB",
    "CAPS_LOCK",
    "ESCAPE",
    "RETURN",
    "BACKSPACE",
    "COMMA",
    "PERIOD",
    "APOSTROPHE",
    "SLASH",
    "BACKSLASH",
    "GRAVE",
    "SEMICOLON",
    "RIGHT_BRACKET",
    "LEFT_BRACKET",
    "MINUS",
    "EQUALS",
    "INSERT",
    "DELETE",
    "HOME",
    "END",
    "PAGE_UP",
    "PAGE_DOWN",
    "PRINT_SCREEN",
    "SCROLL_LOCK",
    "NUM_LOCK",
    "UP",
    "DOWN",
    "LEFT",
    "RIGHT",
    "PAD0",
    "PAD1",
    "PAD2",
    "PAD3",
    "PAD4",
    "PAD5",
    "PAD6",
    "PAD7",
    "PAD8",
    "PAD9",
    "PAD_PLUS",
    "PAD_MINUS",
    "PAD_TIMES",
    "PAD_DIV",
    "PAD_DECIMAL",
    "PAD_RETURN",
    "UNKNOWN",
};

template <typename T, std::size_t N>
constexpr std::size_t size(const T (&)[N]) {
    return N;
}

constexpr const char* helper(std::uint16_t n) {
    return n < size(strings) ? strings[n] : nullptr;
}

}

namespace zephyr {
namespace input {

const char* to_string(Key key) {
    return helper(static_cast<std::uint16_t>(key));
}

} /* namespace input */
} /* namespace zephyr */


