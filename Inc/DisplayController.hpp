#pragma once

#include <variant>
#include "Terminal/Terminal.hpp"
#include "Screens/TerminalScreen.hpp"
#include "Screens/MainScreen.hpp"

namespace cmd = Command;

extern cmd::Terminal terminal0;

class DisplayController {
private:
    std::variant<MainScreen, TerminalScreen> currentScreen;
public:
    DisplayController();
    ~DisplayController();

    void init();

    void draw();

    template<typename screen, typename... Args>
    void setScreen(Args&&... args);
};
