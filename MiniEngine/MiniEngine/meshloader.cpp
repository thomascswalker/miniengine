#include "meshloader.h"

template <typename Out>
void split(const std::string& s, char delim, Out result)
{
	std::istringstream iss(s);
	std::string item;
	while (std::getline(iss, item, delim)) {
		*result++ = item;
	}
}

std::vector<std::string> split(const std::string& s, char delim)
{
	std::vector<std::string> elems;
	split(s, delim, std::back_inserter(elems));
	return elems;
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
		
		std::string line;
		while (std::getline(stream, line, '\n'))
		{
			// If line is commented or is empty, we'll continue
			if (!line.starts_with('v') && !line.starts_with('f'))
			{
				continue;
			}

			// Vertex positions
			if (line.starts_with('v'))
			{
				std::vector<std::string> elements;
				split(line, ' ', std::back_inserter(elements));

				// Skip zeroth element as it's just 'v'
				double x = std::stod(elements[2]);
				double y = std::stod(elements[3]);
				double z = std::stod(elements[4]);

				Vertex vtx(x, y, z);
				vertices.push_back(vtx);
			}

			// Face indices
			if (line.starts_with('f'))
			{
				std::vector<std::string> elements;
				split(line, ' ', std::back_inserter(elements));

				// Skip zeroth element as it's just 'f'
				int i1 = std::stoi(elements[1]);
				int i2 = std::stoi(elements[2]);
				int i3 = std::stoi(elements[3]);

				indices.push_back(i1);
				indices.push_back(i2);
				indices.push_back(i3);
			}
		}
	}

	Mesh mesh(vertices, indices);
	return mesh;
}