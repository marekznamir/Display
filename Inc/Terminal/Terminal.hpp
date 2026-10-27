#pragma once

#include <cstddef>
#include <tuple>

namespace Command {

    constexpr std::size_t BUFF_SIZE = 32;    
    constexpr std::size_t LINE_LEN = 32;
    constexpr std::size_t FINAL_INDEX_MASK = BUFF_SIZE - 1;

    struct Line {
        char data[LINE_LEN];
    };

    class Terminal {
    public:
        Line buff[BUFF_SIZE];
        std::size_t newestLineIndex = 0;
        std::size_t lineCount = 0;
    
        Terminal();

        const std::tuple<Line*, std::size_t, std::size_t> data();
    };

    void write(char* buffer, std::size_t size, Terminal& terminalInstance);
};