#ifndef PRINT_BUFFER_H
#define PRINT_BUFFER_H

#include <cstdarg>
#include <string>
#include <vector>
#include <wtypes.h>
#include "api.h"

namespace Graphics
{
    class PrintBuffer
    {
     private:
        static PrintBuffer* instance;
        PrintBuffer() = default;

        std::vector<std::string> m_entries;
        std::wstring m_buffer;

     public:
        static PrintBuffer* getInstance()
        {
            if (instance == nullptr)
            {
                instance = new PrintBuffer();
            }
            return instance;
        }

        static void debugPrintToScreen(const char* format, ...);
        LPCWSTR getEntries();

        static void clear()
        {
            getInstance()->m_entries.clear();
        }
    };
}

#endif
