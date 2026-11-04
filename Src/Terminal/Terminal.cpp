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
            std::memset(line, 0, LINE_LEN);
        }
    }

    void Terminal::write(char* buffer)
    {
        constexpr const char* delim = "\n";
        constexpr const std::size_t DELIM_LEN = std::char_traits<char>::length(delim);
        auto res = strstr(buffer, delim);

        while(res != NULL)
        {
            std::size_t len = static_cast<std::size_t>(res - buffer);
            if(len >= LINE_LEN)
            {
                len = LINE_LEN - 1;
            }
            memcpy(buff[newestLineIndex], buffer, len);
            buff[newestLineIndex][len] = '\0';

            newestLineIndex = (newestLineIndex + 1) & FINAL_INDEX_MASK;
            if(lineCount < BUFF_SIZE)
            {
                lineCount++;
            }

            buffer = res + DELIM_LEN;
            res = strstr(buffer, delim);
        }
    }

    template<typename... Args>
    void Terminal::printf(const char* buf, Args... args)
    {
        constexpr std::size_t MAX_ARGS_SIZE = 10;
        char charArgs[MAX_ARGS_SIZE][LINE_LEN];
        std::size_t idx = 0;
        (
            [&] {
                if (idx < MAX_ARGS_SIZE)
                {
                    auto [ptr, ec] = std::to_chars(charArgs[idx], charArgs[idx] + LINE_LEN - 1, args);
                    *ptr = '\0';
                    idx++;
                }
            }(),
            ...
        );

        char newBuf[LINE_LEN];

        constexpr const char* delim = "%";
        constexpr const std::size_t DELIM_LEN = std::char_traits<char>::length(delim);
        auto res = strstr(buf, delim);

        char* newBufPos = newBuf;
        char* const newBufEnd = newBuf + LINE_LEN - 1;

        std::size_t argInsertIdx = 0;
        while (res != NULL)
        {
            auto next = res + 1;

            std::size_t len = static_cast<std::size_t>(res - buf);
            if (newBufPos + len > newBufEnd)
            {
                len = static_cast<std::size_t>(newBufEnd - newBufPos);
            }
            memcpy(newBufPos, buf, len);
            newBufPos += len;
            *newBufPos = '\0';

            switch (*next)
            {
                case 'd':
                [[fallthrough]];
                case 'f':
                [[fallthrough]];
                case 'v': // v as in value
                {
                    if (argInsertIdx < idx)
                    {
                        auto argLen = strlen(charArgs[argInsertIdx]);
                        if ((newBufPos + argLen) <= newBufEnd)
                        {
                            memcpy(newBufPos, charArgs[argInsertIdx], argLen);
                            newBufPos += argLen;
                            *newBufPos = '\0';
                            argInsertIdx++;
                        }
                    }
                    buf = (next + 1);
                    break;
                }
                case '\0':
                    *newBufPos = '\0';
                    buf = next;
                    break;
                case '%':
                    if (newBufPos + 1 <= newBufEnd)
                    {
                        *newBufPos = '%';
                        newBufPos++;
                        *newBufPos = '\0';
                    }
                    buf = (next + 1);
                    break;
                default:
                    memcpy(newBuf, "Wrong format %_ <--\n", 21);
                    write(newBuf);
                    return;
            }

            res = strstr(buf, delim);
        }

        std::size_t remLen = strlen(buf);
        if (newBufPos + remLen > newBufEnd)
        {
            remLen = static_cast<std::size_t>(newBufEnd - newBufPos);
        }
        memcpy(newBufPos, buf, remLen);
        newBufPos += remLen;
        *newBufPos = '\0';
        write(newBuf);
    }

    template void Terminal::printf<int>(const char*, int);
    template void Terminal::printf<float>(const char*, float);

    const std::tuple<const char(*)[LINE_LEN], std::size_t, std::size_t> Terminal::data()
    {
        return { buff, newestLineIndex, lineCount };
    }
};
