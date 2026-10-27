#pragma once

#include <variant>
#include "Terminal/Terminal.hpp"
#include "Screens/TerminalScreen.hpp"

namespace cmd = Command;

extern cmd::Terminal terminal0;

class DisplayController {
private:
    std::variant<std::monostate, TerminalScreen> currentScreen;
public:
    DisplayController();
    ~DisplayController();

    void init();

    void draw();

    template <typename ScreenType, typename... Args>
    void setScreen(Args&&... args) {
        currentScreen.emplace<ScreenType>(std::forward<Args>(args)...);
    }
};
