#pragma once

#include <cstddef>
#include <tuple>

namespace Command {

    constexpr std::size_t BUFF_SIZE = 32;    
    constexpr std::size_t LINE_LEN = 32;
    constexpr std::size_t FINAL_INDEX_MASK = BUFF_SIZE - 1;

    class Terminal {
    private:
        void write(char* buffer);
    public:
        char buff[BUFF_SIZE][LINE_LEN];
        std::size_t newestLineIndex = 0;
        std::size_t lineCount = 0;
    
        Terminal();

        template<typename... Args>
        void printf(const char* buf, Args... args);

        const std::tuple<const char(*)[LINE_LEN], std::size_t, std::size_t> data();
    };
}