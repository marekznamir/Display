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

        if(lineCount < BUFF_SIZE)
        {
            lineCount++;
        }

        std::size_t i = 0;
        for(; i < size; i++)
        {
            if(buffer[i] == '\n')
            {
                // Close current line and continue with the rest of the text in a new one
                data[i] = '\0';
                newestLineIndex = (newestLineIndex + 1) & FINAL_INDEX_MASK;

                if((i + 1 < size) && (buffer[i + 1] != '\0'))
                {
                    write(&buffer[i + 1], (size - i - 1), terminalInstance);
                }
                return;
            }
            else if(buffer[i] == '\0')
            {
                break;
            }
            else if(i == LINE_LEN - 1)
            {
                // Line full - wrap the remaining text into the next line
                data[i] = '\0';
                newestLineIndex = (newestLineIndex + 1) & FINAL_INDEX_MASK;
                write(&buffer[i], (size - i), terminalInstance);
                return;
            }
            else
            {
                data[i] = buffer[i];
            }
        }
        data[i] = '\0';
        newestLineIndex = (newestLineIndex + 1) & FINAL_INDEX_MASK;
    }

    template<typename Arg>
    void write(Arg& arg, Terminal& terminalInstance)
    {
        char buf[LINE_LEN]{};
        std::to_chars_result result = std::to_chars(buf, buf + LINE_LEN, arg);

        if (result.ec != std::errc())
        {
            char err[] = "ERR";
            write(err, sizeof(err) - 1, terminalInstance);
        }
        else
        {
            write(buf, static_cast<std::size_t>(result.ptr - buf), terminalInstance);
        }
    }

    template void write<uint8_t>(uint8_t&, Terminal&);
    template void write<uint16_t>(uint16_t&, Terminal&);
    template void write<uint32_t>(uint32_t&, Terminal&);
    template void write<uint64_t>(uint64_t&, Terminal&);

    const std::tuple<Line*, std::size_t, std::size_t> Terminal::data()
    {
        return { buff, newestLineIndex, lineCount };
    }
};
