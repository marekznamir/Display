#include "Terminal/Terminal.hpp"
#include <cstddef>
#include <cstring>

namespace Command {

    Terminal::Terminal()
    {
        for(auto& line : buff)
        {
            std::memset(line.data, 0, LINE_LEN);
        }
    }

    void write(char* buffer, std::size_t size, Terminal& terminalInstance)
    {
        auto& lineCount = terminalInstance.lineCount;
        auto& newestLineIndex = terminalInstance.newestLineIndex;
        auto& data = terminalInstance.buff[newestLineIndex].data;

        if(size >= LINE_LEN)
        {
            size = LINE_LEN - 1;
        }

        if(lineCount < BUFF_SIZE)
        {
            lineCount++;
        }

        std::memcpy(data, buffer, size);
        data[size] = '\0';
        newestLineIndex = (newestLineIndex + 1) & FINAL_INDEX_MASK;
    }

    const std::tuple<Line*, std::size_t, std::size_t> Terminal::data()
    {
        return { buff, newestLineIndex, lineCount };
    }
};