#include "printbuffer.h"

namespace Graphics
{
    void PrintBuffer::debugPrintToScreen(const char* format, ...)
    {
        auto buffer = PrintBuffer::getInstance();
        char text[1024];
        va_list arg;
        va_start(arg, format);
        _vsnprintf_s(text, sizeof(text), format, arg);
        va_end(arg);

        std::string str = text;
        buffer->m_entries.push_back(str);
    }

    LPCWSTR PrintBuffer::getEntries()
    {
        std::string bufferStr;
        for (const std::string& entry : m_entries)
        {
            bufferStr += entry + "\n";
        }
        m_buffer = std::wstring(bufferStr.begin(), bufferStr.end());
        return (LPCWSTR)m_buffer.c_str();
    }

    PrintBuffer* PrintBuffer::instance = 0;
}