#ifndef MESHLOADER_H
#define MESHLOADER_H

#include <iostream>
#include <iterator>
#include <fstream>
#include <sstream>
#include <string>

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