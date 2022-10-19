#ifndef PRINT_BUFFER_H
#define PRINT_BUFFER_H

#include <stdarg.h>
#include <string>
#include <vector>
#include <wtypes.h>
#include <format>

class PrintBuffer
{
private:
    static PrintBuffer *instance;
    PrintBuffer() {};

    std::vector<std::string> m_entries;
    std::wstring m_buffer = L"";

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
    //void addEntry(const char* format, ...);
    LPCWSTR getEntries();

    static void clear() { getInstance()->m_entries.clear(); }
};

#endif
