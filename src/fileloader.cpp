#include "fileloader.h"

namespace Graphics {
    constexpr uint32 GLTF_MAGIC = 0x46546C67;
    constexpr size_t GLTF_HEADER_SIZE = sizeof(uint32);

    bool getOpenFilename(FileTypes type, std::string &filename) {
        LPCSTR typeFilter;
        switch (type) {
            case Obj:
                typeFilter = FILE_FILTER_OBJ;
                break;
            case Glb:
                typeFilter = FILE_FILTER_GLB;
                break;
            case Gltf:
                typeFilter = FILE_FILTER_GLTF;
                break;
            default:
                break;
        }

        OPENFILENAME ofn = {0};
        TCHAR szFile[256] = {0};

        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = nullptr;
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = typeFilter;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = nullptr;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        if (GetOpenFileName(&ofn) == TRUE) {
            std::string dest(ofn.lpstrFile);
            filename = dest;
            return true;
        } else {
            return false;
        }
    }

    std::vector<char> openBinaryFile(const std::string &filename) {
        /// Open file
        std::cout << "Opening file " << filename << "..." << std::endl;
        std::ifstream file(filename, std::ios::binary);    // Read as binary file

        // Get file size
        file.seekg(0, std::ios::end);                // Move to end
        std::streamsize fileSize = file.tellg();    // Get byte position at this point
        std::vector<char> buffer(fileSize);            // Construct new char buffer
        file.seekg(0, std::ios::beg);                // Move back to beginning
        file.read(&buffer.at(0), fileSize);            // Read all contents into memory into the address at the

        std::cout << "File is " << fileSize << " bytes" << std::endl;

        // beginning of the buffer
        std::cout << "Closing file " << filename << std::endl;
        file.close();

        return buffer;
    }

    std::string openAsciiFile(const std::string &filename) {
        /// Open file
        std::cout << "Opening file " << filename << "..." << std::endl;
        std::ifstream file(filename);    // Read as ascii file
        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();

        return buffer.str();
    }

    // https://computergraphics.stackexchange.com/questions/7519/how-mesh-geometry-data-vertex-coordinates-stored-in-gltf
    // Loop through meshes
    Mesh *parseGltfBinary(JSON::JsonObject &json, std::vector<char> &buffer, int bufferByteSize) {
        Mesh *mesh = new Mesh();

        for (int i = 0; i < json["meshes"].size(); i++) {
            std::cout << "Mesh " << i << std::endl;
            auto m = json["meshes"][i];
            // Loop through primitives
            for (int j = 0; j < m["primitives"].size(); j++) {
                std::cout << "Prim " << i << std::endl;
                auto p = m["primitives"][j];

                // Parse vertex indices
                std::cout << "INDICES" << std::endl;
                int indicesIndex = p["indices"].asInt().value();
                auto indicesAccessor = json["accessors"][indicesIndex];

                int indicesOffset = indicesAccessor["byteOffset"].asInt().value();
                int indicesCount = indicesAccessor["count"].asInt().value();
                int indicesType = indicesAccessor["componentType"].asInt().value();

                auto *indicesPtr = reinterpret_cast<unsigned short *>(&buffer.at(indicesOffset));
                std::vector<int> indices;
                for (int k = 0; k < indicesCount; k++) {
                    auto index = *indicesPtr++;
                    std::cout << ">>> " << index << std::endl;
                    indices.push_back(index);
                }
                mesh->setIndices(indices);

                // Parse vertex positions
                std::cout << "POSITIONS" << std::endl;
                int positionIndex = p["attributes"]["POSITION"].asInt().value();
                auto positionAccessor = json["accessors"][positionIndex];

                int positionOffset = positionAccessor["byteOffset"].asInt().value();
                int positionCount = positionAccessor["count"].asInt().value();
                int positionType = positionAccessor["componentType"].asInt().value();

                std::cout << positionType << std::endl;

                auto *positionPtr = reinterpret_cast<float *>(&buffer.at(positionOffset));
                std::vector<Vector3> vertices;
                for (int k = 0; k < positionCount; k++) {
                    auto x = *positionPtr++;
                    auto y = *positionPtr++;
                    auto z = *positionPtr++;
                    Vector3 pos(x, y, z);
                    vertices.push_back(pos);
                    std::cout << ">>> " << pos.toString() << std::endl;
                }
                mesh->setVertices(vertices);
            }
        }

        mesh->bindTris();
        return mesh;
    }

    Mesh *loadGlbFile(const std::string &filename) {
        Mesh *mesh = new Mesh();
        std::vector<Vertex> vertices;    // Empty vertex array
        std::vector<int> indices;        // Empty index array
        std::vector<Vector3> normals;

        auto buffer = openBinaryFile(filename);

        /// Start reading the first 12-byte header
        // Validate the magic number header at the top of the file. This should read as 'glTF', 1 byte per letter.
        // 4 bytes total.
        auto header = reinterpret_cast<unsigned int *>(&buffer.at(0));
        std::string magicString(reinterpret_cast<const char *>(header), 4);
        if (magicString == "glTF") {
            std::cout << "Valid magic header found." << std::endl;
        } else {
            throw std::runtime_error("Invalid magic header!");
        }

        // Read the version and validate it is version 2
        header++; // Go 4 bytes forward
        if (*header != 2) {
            std::cout << "Found version: " << *header << std::endl;
            throw std::runtime_error("Invalid version number. Only supports version 2.");
        }
        std::cout << "Valid version found." << std::endl;

        // Read the length, which is the file size, and compare to the actual file size
        header++; // Go 4 bytes forward

        // Read the JSON content length
        header++; // Go 4 bytes forward
        auto jsonLength = *header;
        std::cout << "JSON length is " << jsonLength << std::endl;

        // Extract JSON content as a string
        auto *chunk0 = reinterpret_cast<const char *>(&buffer.at(20));

        // Extract JSON string from bytes.
        std::string jsonString(chunk0, jsonLength);

        // Parse the JSON string
        JSON::JsonObject json;
        try {
            json = JSON::loadString(jsonString);
        }
        catch (const std::runtime_error &e) {
            std::cout << e.what() << std::endl;
        }

        std::cout << "JSON loaded successfully" << std::endl;
        std::cout << json << std::endl;

        auto bufferSize = json["buffers"][0]["byteLength"].asInt().value();

        std::vector<char> chunk(buffer.begin() + 20 + jsonLength, buffer.end());
        return parseGltfBinary(json, chunk, bufferSize);
    }

    Mesh *loadGltfFile(const std::string &filename) {
        auto found = filename.find_last_of('\\');
        std::string directory = filename.substr(0, found);

        std::cout << "Reading .gltf file: " << filename << std::endl;
        JSON::JsonObject json = JSON::loadFile(filename);
        auto binBasename = json["buffers"][0]["uri"].asString().value();
        auto bufferSize = json["buffers"][0]["byteLength"].asInt().value();
        auto binFilename = directory + "\\" + binBasename;
        std::cout << "Found .bin file: " << binFilename << std::endl;
        auto buffer = openBinaryFile(binFilename);
        return parseGltfBinary(json, buffer, bufferSize);
    }

    Mesh *loadObjFile(const std::string &filename) {
        Mesh *mesh = new Mesh();
        std::vector<Vertex> vertices;    // Empty vertex array
        std::vector<int> indices;        // Empty index array
        std::vector<Vector3> normals;

        std::ifstream file(filename);    // New filestream
        if (!file) {
            throw std::runtime_error("Invalid file: " + filename);
        }
        std::stringstream stream;    // New string stream
        stream << file.rdbuf();        // Read data

        std::string line;            // New buffer for the current line

        std::cout << "Reading file contents..." << std::endl;

        // Keep reading until end-of-file
        while (stream.peek() != -1) {
            // (Safely) get the current line
            readLine(stream, line);
            std::cout << line << std::endl;

            if (line.starts_with('\0'))            // Skip if line is empty
            {
                continue;
            }

            if (line.starts_with('#'))            // Skip if the line is commented
            {
                continue;
            }

            // Pointer to first character in line
            const char *token = line.c_str();

            // Vertices
            if (*token == 'v')    // v 0.5 2.32843 -1.23
            {
                std::istringstream istream(line);
                std::string str;
                std::vector<double> values;

                // Split the line by spaces
                while (std::getline(istream, str, ' ')) {
                    if (std::count(INVALID_VERTEX_TOKENS.begin(), INVALID_VERTEX_TOKENS.end(), str)) {
                        continue;
                    }

                    // Attempt to parse the result to a double
                    double result = 0.0;
                    if (parseNumber(str, &result)) {
                        // If it is a valid double, we'll append to our values array
                        values.push_back(result);
                    }
                }

                if (values.size() != 3) {
                    throw std::runtime_error("Incorrect vertex definition.");
                }

                // Create a new Vertex and append it to the vertices array
                Vertex vtx(values[0], values[1], values[2]);
                vertices.push_back(vtx);
            }

            if (*token == 'v' && *token + 1 == 'n') {
                std::istringstream istream(line);
                std::string str;
                std::vector<double> values;

                // Split the line by spaces
                while (std::getline(istream, str, ' ')) {
                    if (std::count(INVALID_VERTEX_TOKENS.begin(), INVALID_VERTEX_TOKENS.end(), str)) {
                        continue;
                    }

                    // Attempt to parse the result to a double
                    double result = 0.0;
                    if (parseNumber(str, &result)) {
                        // If it is a valid double, we'll append to our values array
                        values.push_back(result);
                    }
                }

                if (values.size() != 3) {
                    throw std::runtime_error("Incorrect vertex normal definition.");
                }

                // Create a new Vertex and append it to the vertices array
                Vector3 normal(values[0], values[1], values[2]);
                normals.push_back(normal);
            }

            // Indices
            if (*token == 'f')    // f 0 2 3 ... n
            {
                std::istringstream istream(line);
                std::string str;
                std::vector<int> values;

                // Split the line by spaces
                while (std::getline(istream, str, ' ')) {
                    if (std::count(INVALID_FACE_TOKENS.begin(), INVALID_FACE_TOKENS.end(), str)) {
                        continue;
                    }

                    // Account for v/vt/vn in one face
                    std::vector<std::string> segments = splitString(str, '/');
                    if (segments.size() > 1) {
                        str = segments[0];
                    }

                    // Attempt to parse the result to an integer
                    int result = 0;
                    if (parseNumber(str, &result)) {
                        // If it is a valid integer, we'll append to our values array
                        values.push_back(result - 1); // Account for .obj being 1-based
                    }
                }

                // Check if we have less than 3 indices per face
                if (values.size() < 3) {
                    throw std::runtime_error("Incorrect index definition.");
                }

                // Append each index to the indices array
                int triCount = (int) values.size() - 2;
                for (int i = 0; i < triCount; i++) {
                    int i1 = 0;
                    int i2 = i + 1;
                    int i3 = i + 2;

                    indices.push_back(values[i1]);
                    indices.push_back(values[i2]);
                    indices.push_back(values[i3]);
                }
            }
        }

        mesh->setVertices(vertices);
        mesh->setIndices(indices);
        mesh->bindTris();

        return mesh;
    }

    StandardShader *loadShaderFile(const std::string &filename) {
        auto *shader = new StandardShader();

        std::ifstream file(filename);    // New filestream
        if (file) {
            std::stringstream stream;    // New string stream
            stream << file.rdbuf();        // Read data

            std::string line;            // New buffer for the current line

            // Keep reading until end-of-file
            while (stream.peek() != -1) {
                readLine(stream, line);                // Read current line
                const char *token = line.c_str();    // Pointer to first character in line

                if (line.starts_with('\0'))            // Skip if line is empty
                {
                    continue;
                }

                if (line.starts_with('#'))            // Skip if the line is commented
                {
                    continue;
                }

                auto equalsIndex = line.find('=');
                if (equalsIndex == std::string::npos)    // If there's no = found
                {
                    continue;
                }
                auto key = line.substr(0, equalsIndex);
                auto value = line.substr(equalsIndex + 1, line.size());

                auto test = splitString(value, ',');
                std::vector<double> rgba;
                for (const auto &str: test) {
                    int result = 0;
                    if (parseNumber(str, &result)) {
                        rgba.push_back(result);
                    }
                }
                Vector3 vec(rgba[0], rgba[1], rgba[2]);

                if (key == "color") {
                    continue;
                    //shader->setColor(vec);
                } else {
                    continue;
                }
            }
        }

        return shader;
    }


}