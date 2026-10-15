#include "DisplayController.hpp"

#ifdef _WIN32

#include <pybind11/pybind11.h>

enum class Button {
    UP,
    DOWN,
    OK,
    BACK,
    VIEW,
    FN
};

void onButtonPressed(Button button) {
    switch(button) {
        case Button::UP:
            // Handle UP button press
            break;
        case Button::DOWN:
            // Handle DOWN button press
            break;
        case Button::OK:
            // Handle OK button press
            break;
        case Button::BACK:
            // Handle BACK button press
            break;
        case Button::VIEW:
            // Handle VIEW button press
            break;
        case Button::FN:
            // Handle FN button press
            break;
    }
}



 

#endif
