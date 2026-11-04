#pragma once

#include <cstddef>

constexpr std::size_t SCREEN_W = 240;
constexpr std::size_t SCREEN_H = 160;

template <typename T>
class IScreen {
public:
    IScreen() = default;
    void draw();
};

