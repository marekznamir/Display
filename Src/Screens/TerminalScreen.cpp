#include "Screens/TerminalScreen.hpp"
#include <cstddef>
#include <iterator>

#include "u8g2.h"

TerminalScreen::TerminalScreen(cmd::Terminal &terminal) : terminalInstance(terminal)
{
}

extern u8g2_t u8g2;

void TerminalScreen::draw()
{
    const auto& [buff, newestLineIndex, lineCount] = terminalInstance.data();

    u8g2_ClearBuffer(&u8g2);
    u8g2_SetBitmapMode(&u8g2, 1);
    u8g2_SetFontMode(&u8g2, 1);
    u8g2_SetFont(&u8g2, u8g2_font_t0_16_tr);

    constexpr std::size_t LINE_HEIGHT = 16;
    constexpr std::size_t VISIBLE_LINES = 160 / LINE_HEIGHT; 

    std::size_t linesToDraw = std::min(lineCount, VISIBLE_LINES);
    std::size_t startIdx = (newestLineIndex + cmd::BUFF_SIZE - linesToDraw) & cmd::FINAL_INDEX_MASK;
    for (std::size_t i = 0; i < linesToDraw; i++)
    {
        std::size_t idx = (startIdx + i) & cmd::FINAL_INDEX_MASK;
        u8g2_uint_t y = static_cast<u8g2_uint_t>((i + 1) * LINE_HEIGHT);
        u8g2_DrawStr(&u8g2, 0, y, buff[idx].data);
    }

    u8g2_SendBuffer(&u8g2);
}