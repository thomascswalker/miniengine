// https://github.com/tinyobjloader/tinyobjloader/blob/master/tiny_obj_loader.h

#include "meshloader.h"

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
				line += static_cast<char>(currentChar);
			}
		}
	}

	return stream;
}

template <typename T>
static bool
parseValue(std::string value, T *result)
{
	try
	{
		if constexpr (std::is_floating_point_v<T>)
		{
			*result = std::stod(value, NULL);
		}
		else if (std::is_integral_v<T>)
		{
			*result = std::stoi(value, NULL);
		}
		else
		{
			return false;
		}
	}
	catch (...)
	{
		return false;
	}

	return true;
}

Mesh
MeshLoader::load(std::string filename, FileType type)
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
					// Attempt to parse the result to a double
					double result = 0.0;
					if (parseValue(str, &result))
					{
						// If it is a valid double, we'll append to our values array
						values.push_back(result);
					}
				}

				// Create a new Vertex and append it to the vertices array
				Vertex vtx(values[0], values[1], values[2]);
				vertices.push_back(vtx);
			}

			// Indices
			if (*token == 'f')	// f 0 2 3
			{
				std::istringstream istream(lineBuffer);
				std::string str;
				std::vector<int> values;

				// Split the line by spaces
				while (std::getline(istream, str, ' '))
				{
					// Attempt to parse the result to a integer
					int result = 0;
					if (parseValue(str, &result))
					{
						// If it is a valid integer, we'll append to our values array
						values.push_back(result);
					}
				}

				// Append each index to the indices array
				indices.push_back(values[0]);
				indices.push_back(values[1]);
				indices.push_back(values[2]);
			}
		}
	}
	Mesh mesh(vertices, indices);
	return mesh;
}