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

#include "json.h"

constexpr auto FILE_FILTER_OBJ = "Wavefront OBJ (.obj)\0*.obj\0";
constexpr auto FILE_FILTER_GLB = "GLB (.glb)\0*.glb\0";
constexpr auto FILE_FILTER_GLTF = "glTF (.gltf)\0*.gltf\0";

// https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#meshes-overview
constexpr auto GLB_PRIM_POINTS = 0;
constexpr auto GLB_PRIM_LINE_STRIPS = 1;
constexpr auto GLB_PRIM_LINE_LOOPS = 2;
constexpr auto GLB_PRIM_LINES = 3;
constexpr auto GLB_PRIM_TRIANGLES = 4;
constexpr auto GLB_PRIM_TRIANGLE_STRIPS = 5;
constexpr auto GLB_PRIM_TRIANGLE_FANS = 6;

// https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#accessor-data-types
typedef signed char GLB_ACC_TYPE_SB;
typedef unsigned char GLB_ACC_TYPE_UB;
typedef signed short GLB_ACC_TYPE_SS;
typedef unsigned short GLB_ACC_TYPE_US;
typedef unsigned int GLB_ACC_TYPE_UI;
typedef float GLB_ACC_TYPE_F;


namespace Graphics {
    enum FileTypes {
        Obj,
        Glb,
        Gltf
    };

    static std::vector<const char *> INVALID_VERTEX_TOKENS = {"v", "vn", "vt", "", " "};
    static std::vector<const char *> INVALID_FACE_TOKENS = {"f", "", " "};

    static std::istream &readLine(std::istream &stream, std::string &line) {
        // Clear the content of the line
        line.clear();

        // Prepare stream for input
        std::istream::sentry sentry(stream, true);
        if (!sentry) {
            return stream;
        }

        // Create a new buffer from our stream
        std::streambuf *streamBuffer = stream.rdbuf();


        // Switch on current char
        while (true) {
            // Read the current character
            int currentChar = streamBuffer->sbumpc();
            switch (currentChar) {
                // For \n
                case '\n': {
                    return stream;
                }
                    // For \r\n
                case '\r': {
                    // If the next character is a new line, we'll bump to that next line
                    if (streamBuffer->sgetc() == '\n') {
                        streamBuffer->sbumpc();
                    }
                    return stream;
                }
                    // For \r
                case EOF: // -1
                {
                    if (line.empty()) {
                        stream.setstate(std::ios::eofbit);
                    }
                    return stream;
                }
                    // All other cases, the rest of the line
                default: {
                    line += (char) currentChar;
                }
            }
        }

        return stream;
    }

    static bool doesStringContainAny(const std::string &str, const std::string &value) {
        if (str.empty()) {
            return false;
        }
        bool result = false;
        for (const char s: str) {
            for (const char v: value) {
                if (s == v) {
                    result = true;
                }
            }
        }
        return result;
    }

    static bool isStringANumber(std::string str) {
        return doesStringContainAny(str, "0123456789");
    }

    static std::vector<std::string> splitString(const std::string &string, const char delim) {
        std::stringstream stream(string);
        std::vector<std::string> list;
        std::string segment;

        while (std::getline(stream, segment, delim)) {
            list.push_back(segment);
        }

        return list;
    }

/*Given a string, attempt to parse a number from it.*/
    template<typename T>
    static bool parseNumber(const std::string &value, T *result) {
        if constexpr (std::is_floating_point_v<T>) {
            *result = std::stod(value);
            return true;
        } else if (std::is_integral_v<T>) {
            *result = std::stoi(value);
            return true;
        } else {
            return false;
        }
    }

    /// <summary>
    /// Wrapper for GetOpenFileNameW to simplify the parameter inputs.
    /// </summary>
    bool getOpenFilename(FileTypes type, std::string &filename);

    std::vector<char> openBinaryFile(const std::string &filename);

    std::string openAsciiFile(const std::string &filename);

    Mesh* parseGltfBinary(JSON::JsonObject& json, std::vector<char>& buffer, int bufferByteSize);

    Mesh *loadGlbFile(const std::string &filename);

    Mesh *loadGltfFile(const std::string &filename);

    Mesh *loadObjFile(const std::string &filename);

    StandardShader *loadShaderFile(const std::string &filename);
}

#endif