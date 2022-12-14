#ifndef FILELOADER_H
#define FILELOADER_H

#include <iostream>
#include <iterator>
#include <fstream>
#include <set>
#include <sstream>
#include <string>

#include "core.h"
#include "mesh.h"
#include "shader.h"

constexpr auto FILE_FILTER_OBJ = "Wavefront OBJ (.obj)\0*.obj\0";
#define FILE_FILTER_GLB L"GLB (.glb)\0*.glb\0"
#define FILE_FILTER_SHADER L"Pixel Shader File (.ini)\0*.pxl\0"

namespace Graphics
{
    enum FileTypes
    {
        Obj,
        Gltf
    };

    static std::vector<const char*> INVALID_VERTEX_TOKENS = { "v", "vn", "vt", "", " " };
    static std::vector<const char*> INVALID_FACE_TOKENS = { "f", "", " " };

    static std::istream& readLine(std::istream& stream, std::string& line)
    {
        // Clear the content of the line
        line.clear();

        // Prepare stream for input
        std::istream::sentry sentry(stream, true);
        if (!sentry)
        {
            return stream;
        }

        // Create a new buffer from our stream
        std::streambuf* streamBuffer = stream.rdbuf();


        // Switch on current char
        while (true)
        {
            // Read the current character
            int currentChar = streamBuffer->sbumpc();
            switch (currentChar)
            {
                // For \n
            case '\n':
            {
                return stream;
            }
                // For \r\n
            case '\r':
            {
                // If the next character is a new line, we'll bump to that next line
                if (streamBuffer->sgetc() == '\n')
                {
                    streamBuffer->sbumpc();
                }
                return stream;
            }
                // For \r
            case EOF: // -1
            {
                if (line.empty())
                {
                    stream.setstate(std::ios::eofbit);
                }
                return stream;
            }
                // All other cases, the rest of the line
            default:
            {
                line += (char)currentChar;
            }
            }
        }

        return stream;
    }

    static bool doesStringContainAny(const std::string& str, const std::string& value)
    {
        if (str.empty())
        {
            return false;
        }
        bool result = false;
        for (const char s : str)
        {
            for (const char v : value)
            {
                if (s == v)
                {
                    result = true;
                }
            }
        }
        return result;
    }

    static bool isStringANumber(std::string str)
    {
        return doesStringContainAny(str, "0123456789");
    }

    static std::vector<std::string> splitString(const std::string& string, const char delim)
    {
        std::stringstream stream(string);
        std::vector<std::string> list;
        std::string segment;

        while (std::getline(stream, segment, delim))
        {
            list.push_back(segment);
        }

        return list;
    }

/*Given a string, attempt to parse a number from it.*/
    template<typename T>
    static bool parseNumber(const std::string& value, T* result)
    {
        if constexpr (std::is_floating_point_v<T>)
        {
            *result = std::stod(value);
            return true;
        }
        else if (std::is_integral_v<T>)
        {
            *result = std::stoi(value);
            return true;
        }
        else
        {
            return false;
        }
    }

/// <summary>
/// Wrapper for GetOpenFileNameW to simplify the parameter inputs.
/// </summary>
    bool getOpenFilename(FileTypes type, std::string& filename);

    Mesh* loadGlbFile(const std::string& filename);
    Mesh* loadObjFile(const std::string& filename);
    StandardShader* loadShaderFile(const std::string& filename);
}

#endif