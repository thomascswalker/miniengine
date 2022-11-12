#ifndef MESHLOADER_H
#define MESHLOADER_H

#include <iostream>
#include <iterator>
#include <fstream>
#include <set>
#include <sstream>
#include <string>

#include "core.h"
#include "mesh.h"

MINI_NAMESPACE_OPEN
MINI_USING_DIRECTIVE

enum FileType
{
	OBJ
};

class MeshLoader
{
public:
	static void load(std::string filename, Mesh* mesh);
};

MINI_NAMESPACE_CLOSE

#endif