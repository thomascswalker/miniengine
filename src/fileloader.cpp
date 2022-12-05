#include "fileloader.h"

MINI_NAMESPACE_OPEN
MINI_USING_DIRECTIVE

#define T(x) L##x

bool getOpenFilename(const char* filter, std::string& filename)
{
    OPENFILENAME ofn = { 0 };
    TCHAR szFile[256] = { 0 };

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = 0;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = T(filter);
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn) == TRUE)
    {
        std::wstring source = (std::wstring) ofn.lpstrFile;
        std::string dest(source.begin(), source.end());
		filename = dest;
		return true;
    }
    else
    {
        return false;
    }
}

Mesh* loadMeshFile(std::string filename)
{
	Mesh* mesh = new Mesh();
	std::vector<Vertex> vertices;	// Empty vertex array
	std::vector<int> indices;		// Empty index array

	std::ifstream file(filename);	// New filestream
	if (file)
	{
		std::stringstream stream;	// New string stream
		stream << file.rdbuf();		// Read data
		
		std::string lineBuffer;		// New buffer for the current line

		// Keep reading until end-of-file
		while (stream.peek() != -1)
		{
			// (Safely) get the current line
			readLine(stream, lineBuffer);

			// Pointer to first character in line
			const char *token = lineBuffer.c_str();

			// Skip if the line is empty
			if (*token == '\0')
			{
				continue;
			}

			// Skip if the line is commented
			if (*token == '#')
			{
				continue;
			}

			// Vertices
			if (*token == 'v')	// v 0.5 2.32843 -1.23
			{
				std::istringstream istream(lineBuffer);
				std::string str;
				std::vector<double> values;

				// Split the line by spaces
				while (std::getline(istream, str, ' '))
				{
					if (std::count(INVALID_VERTEX_TOKENS.begin(), INVALID_VERTEX_TOKENS.end(), str))
					{
						continue;
					}

					// Attempt to parse the result to a double
					double result = 0.0;
					if (parseNumber(str, &result))
					{
						// If it is a valid double, we'll append to our values array
						values.push_back(result);
					}
				}

				if (values.size() != 3)
				{
					throw std::runtime_error("Incorrect vertex definition.");
				}

				// Create a new Vertex and append it to the vertices array
				Vertex vtx(values[0], values[1], values[2]);
				vertices.push_back(vtx);
			}

			// Indices
			if (*token == 'f')	// f 0 2 3 ... n
			{
				std::istringstream istream(lineBuffer);
				std::string str;
				std::vector<int> values;

				// Split the line by spaces
				while (std::getline(istream, str, ' '))
				{
					if (std::count(INVALID_FACE_TOKENS.begin(), INVALID_FACE_TOKENS.end(), str))
					{
						continue;
					}
					
					// Account for v/vt/vn in one face
					std::vector<std::string> segments = splitString(str, '/');
					if (segments.size() > 1)
					{
						str = segments[0];
					}

					// Attempt to parse the result to a integer
					int result = 0;
					if (parseNumber(str, &result))
					{
						// If it is a valid integer, we'll append to our values array
						values.push_back(result - 1); // Account for .obj being 1-based
					}
				}

				// Check if we have less than 3 indices per face
				if (values.size() < 3)
				{
					throw std::runtime_error("Incorrect index definition.");
				}

				// Append each index to the indices array
				int triCount = (int) values.size() - 2;
				for (int i = 0; i < triCount; i++)
				{
					int i1 = 0;
					int i2 = i + 1;
					int i3 = i + 2;

					indices.push_back(values[i1]);
					indices.push_back(values[i2]);
					indices.push_back(values[i3]);
				}
			}
		}
	}
	
	mesh->setVertices(vertices);
	mesh->setIndices(indices);
	mesh->bindTris();

	return mesh;
}

PixelShader* loadShaderFile(std::string filename)
{
	PixelShader* shader = new PixelShader();
	return shader;
}

MINI_NAMESPACE_CLOSE