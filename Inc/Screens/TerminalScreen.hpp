#pragma once

#include "Screens/IScreen.hpp"
#include "Terminal/Terminal.hpp"

namespace cmd = Command;

class TerminalScreen : IScreen<TerminalScreen> {
private:
    cmd::Terminal& terminalInstance;
public:
    TerminalScreen(cmd::Terminal &terminal);
    void draw();
};