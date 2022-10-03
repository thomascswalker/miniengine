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

enum FileType
{
	OBJ
};

class MeshLoader
{
public:
	static Mesh load(std::string filename, FileType type = OBJ);
};


#endif