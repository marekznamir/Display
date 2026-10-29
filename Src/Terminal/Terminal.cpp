#include "Terminal/Terminal.hpp"
#include <cstddef>
#include <cstring>

#include <charconv>
#include <system_error>
#include <cstdint>


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

    template<typename Arg>
    void write(Arg& arg, Terminal& terminalInstance)
    {
        char buf[LINE_LEN]{};
        std::to_chars_result result = std::to_chars(buf, buf + LINE_LEN, arg);

        if (result.ec != std::errc())
        {

        }
        else
        {
            write(buf, BUFF_SIZE, terminalInstance);
        }
    }

    template void write<uint16_t>(uint16_t&, Terminal&);
    template void write<uint8_t>(uint8_t&, Terminal&);
    template void write<uint32_t>(uint32_t&, Terminal&);
    template void write<uint64_t>(uint64_t&, Terminal&);

    const std::tuple<Line*, std::size_t, std::size_t> Terminal::data()
    {
        return { buff, newestLineIndex, lineCount };
    }
};