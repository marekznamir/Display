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

    constexpr std::size_t LINE_HEIGHT = 16; // essentialy font size
    constexpr std::size_t VISIBLE_LINES = SCREEN_H / LINE_HEIGHT; // how many lines fit on screen

    std::size_t linesToDraw = std::min(lineCount, VISIBLE_LINES); // if line count exceedes 10, it makes sure to always print 10 lines
    std::size_t startIndex = (newestLineIndex + cmd::BUFF_SIZE - linesToDraw) & cmd::FINAL_INDEX_MASK;
    /*
        Lets say newestLineIndex is 4 (5th element), to print 10 lines, I would have to reach -5th element, which coresponds to 26th if it wrapped around itself,
        So there is a trick, 4 + 32 - 10 = 26. FINAL_INDEX_MASK assures we dont acces data over 32nd element
    */
    for (std::size_t i = 0; i < linesToDraw; i++)
    {
        std::size_t index = (startIndex + i) & cmd::FINAL_INDEX_MASK;
        u8g2_uint_t y = static_cast<u8g2_uint_t>((i + 1) * LINE_HEIGHT);
        u8g2_DrawStr(&u8g2, 0, y, buff[index]);
    }

    u8g2_SendBuffer(&u8g2);
}