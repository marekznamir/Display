#include <pybind11/pybind11.h>

#include <iostream>

namespace py = pybind11;

class Test {
public:
    int a = 67;

    int getA() {
        return a;
    }
};

#ifdef _WIN32
PYBIND11_MODULE(Display, m) {
    // Eksportujemy klasę Test
    py::class_<Test>(m, "Test")
        .def(py::init<>())          // Udostępniamy domyślny konstruktor
        .def("getA", &Test::getA);  // Udostępniamy metodę getA
}
#endif
