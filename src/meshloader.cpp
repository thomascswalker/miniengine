#include "meshloader.h"

MINI_NAMESPACE_OPEN
MINI_USING_DIRECTIVE

// https://github.com/taurreco/sr/blob/6257fc7776a7fdf087d1fe782e6acf72eabfb70f/src/sr_obj.c

static std::vector<const char*> INVALID_VERTEX_TOKENS = {"v", "vn", "vt", "", " "};
static std::vector<const char*> INVALID_FACE_TOKENS = {"f", "", " "};

std::istream&
readLine(std::istream& stream, std::string& line)
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

static bool
doesStringContainAny(std::string str, std::string value)
{
	if (str == "")
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

static bool
isStringANumber(std::string str)
{
	return doesStringContainAny(str, "0123456789");
}

static std::vector<std::string> splitString(std::string string, const char del)
{
	std::stringstream stream(string);
	std::vector<std::string> list;
	std::string segment;

	while (std::getline(stream, segment, del))
	{
		list.push_back(segment);
	}

	return list;
}

/*Given a string, attempt to parse a number from it.*/
template <typename T>
static bool
parseNumber(std::string value, T *result)
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

void
MeshLoader::load(std::string filename, Mesh* mesh)
{
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
}

MINI_NAMESPACE_CLOSE