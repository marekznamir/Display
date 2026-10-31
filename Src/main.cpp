#ifdef _WIN32

#include "DisplayController.hpp"
#include "u8g2.h"
#include <pybind11/pybind11.h>

namespace py = pybind11;

extern u8g2_t u8g2;

py::bytes getBuffer()
{
    const uint8_t* buf = u8g2_GetBufferPtr(&u8g2);
    const size_t size = 8u * u8g2_GetBufferTileWidth(&u8g2) * u8g2_GetBufferTileHeight(&u8g2);
    return py::bytes(reinterpret_cast<const char*>(buf), size);
}

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



PYBIND11_MODULE(Display, m, py::mod_gil_not_used()) {
    m.doc() = "Display module"; 

    m.def("getBuffer", &getBuffer, py::return_value_policy::reference, "A function that returns the buffer pointer");

    py::enum_<Button>(m, "Button")
        .value("UP", Button::UP)
        .value("DOWN", Button::DOWN)
        .value("OK", Button::OK)
        .value("BACK", Button::BACK)
        .value("VIEW", Button::VIEW)
        .value("FN", Button::FN);

    m.def("onButtonPressed", &onButtonPressed, py::arg("button"), "A function that handles button presses");

    py::class_<DisplayController>(m, "DisplayController")
        .def(py::init<>())
        .def("init", &DisplayController::init)
        .def("draw", &DisplayController::draw);
}

 

#endif
