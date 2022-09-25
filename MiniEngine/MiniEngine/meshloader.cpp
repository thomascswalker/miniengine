// https://github.com/tinyobjloader/tinyobjloader/blob/master/tiny_obj_loader.h

#include "meshloader.h"
#include <cassert>

#define IS_SPACE(x) (((x) == ' ') || ((x) == '\t'))
#define IS_DIGIT(x) (static_cast<unsigned int>((x) - '0') < static_cast<unsigned int>(10))
#define IS_NEW_LINE(x) (((x) == '\r') || ((x) == '\n') || ((x) == '\0'))

//template <typename Out>
//void split(const std::string& s, char delim, Out result)
//{
//	std::istringstream iss(s);
//	std::string item;
//	while (std::getline(iss, item, delim)) {
//		*result++ = item;
//	}
//}
//
//std::vector<std::string> split(const std::string& s, char delim)
//{
//	std::vector<std::string> elems;
//	split(s, delim, std::back_inserter(elems));
//	return elems;
//}

std::istream& readLine(std::istream& stream, std::string& line)
{
	// Clear the content of the line
	line.clear();

	// Create a new buffer from our stream
	std::streambuf* streamBuffer = stream.rdbuf();

	// Read the current character
	int currentChar = streamBuffer->sbumpc();

	// Switch on current char
	switch (currentChar)
	{
		// For \n
		case '\n':
		{
			break;	
		}
		// For \r\n
		case '\r':
		{
			// If the next character is a new line, we'll bump to that next line
			if (streamBuffer->sgetc() == '\n')
			{
				streamBuffer->sbumpc();
			}
			break;
		}
		// For \r
		case EOF: // -1
		{
			if (line.empty())
			{
				stream.setstate(std::ios::eofbit);
			}
			break;
		}
		// All other cases, the rest of the line
		default:
		{
			line += (char) currentChar;
		}
	}

	return stream;
}

template <typename T>
static bool parseValue(const char* current, const char* end, T *result)
{
	if (current >= end)
	{
		return false;
	}

	char sign = '+';

	// Get the current sign (+ or -)
	if (*current == '+' || *current == '-')
	{
		sign = *current;
		current++;
	}
	else if (IS_DIGIT(*current))
	{
		// Pass
	}
	else
	{
		return false;
	}
}

Mesh MeshLoader::load(std::string filename, FileType type)
{
	std::vector<Vertex> vertices;
	std::vector<int> indices;

	std::ifstream file(filename);
	if (file)
	{
		std::stringstream stream;
		stream << file.rdbuf(); // Read data
		
		std::string lineBuffer;
		size_t lineNum = 0;

		// Keep reading until end-of-file
		while (stream.peek() != -1)
		{
			// (Safely) get the current line
			readLine(stream, lineBuffer);
			lineNum++;

			int index = 0;
			// Trim trailing whitespace
			if (lineBuffer.size() > 0)
			{
				// Find the index of the last char before ' ' or '\t'
				index = lineBuffer.find_last_not_of(" \t") + 1;
				lineBuffer = lineBuffer.substr(0, index);
			}

			// Trim newlines
			if (lineBuffer.size() > 0)
			{
				index = lineBuffer.size() - 1;
				if (lineBuffer[index] == '\r')
				{
					lineBuffer.erase(index);
				}
			}

			// Skip if the line is now empty
			if (lineBuffer.empty())
			{
				continue;
			}

			// Pointer to first char in line
			const char *token = lineBuffer.c_str();

			// Skip over leading space
			token += strspn(token, " \t"); // Add number of occurences of spaces/tabs to pointer

			// Skip if the line is now empty
			if (token[0] == '\0')
			{
				continue;
			}

			// Skip if the line is commented
			if (token[0] == '#')
			{
				continue;
			}

			// Vertices
			if (token[0] == 'v')
			{
				// Skip initial 'v' char and space
				token += 2;

				double x, y, z;
				if (!parseValue(token, token, &x) || !parseValue(token, token, &x); || !parseValue(token, token, &x))
				{
					assert ("Failed to load!");
				}
			}
		}
		//while (std::getline(stream, line, '\n'))
		//{
		//	// If line is commented or is empty, we'll continue
		//	if (!line.starts_with('v') && !line.starts_with('f'))
		//	{
		//		continue;
		//	}

		//	// Vertex positions
		//	if (line.starts_with('v'))
		//	{
		//		std::vector<std::string> elements;
		//		split(line, ' ', std::back_inserter(elements));

		//		double x = std::stod(elements[2]);
		//		double y = std::stod(elements[3]);
		//		double z = std::stod(elements[4]);

		//		Vertex vtx(x, y, z);
		//		vertices.push_back(vtx);
		//	}

		//	// Face indices
		//	if (line.starts_with('f'))
		//	{
		//		std::vector<std::string> elements;
		//		split(line, ' ', std::back_inserter(elements));

		//		int i1 = std::stoi(elements[1]);
		//		int i2 = std::stoi(elements[2]);
		//		int i3 = std::stoi(elements[3]);

		//		indices.push_back(i1);
		//		indices.push_back(i2);
		//		indices.push_back(i3);
		//	}
		//}
	}

	Mesh mesh(vertices, indices);
	return mesh;
}