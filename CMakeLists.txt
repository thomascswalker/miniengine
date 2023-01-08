cmake_minimum_required(VERSION 3.24)
project(miniengine)

set(CMAKE_CXX_STANDARD 20)

include_directories(src)
include_directories(include)

add_executable(miniengine
        src/api.h
        src/application.cpp
        src/application.h
        src/camera.cpp
        src/camera.h
        src/channel.h
        src/color.cpp
        src/color.h
        src/coordinates.h
        src/core.cpp
        src/core.h
        src/fileloader.cpp
        src/fileloader.h
        src/framebuffer.cpp
        src/framebuffer.h
        src/maths.h
        src/matrix.cpp
        src/matrix.h
        src/mesh.cpp
        src/mesh.h
        src/object.cpp
        src/object.h
        src/printbuffer.cpp
        src/printbuffer.h
        src/quaternion.cpp
        src/quaternion.h
        src/rotation.h
        src/shader.h
        src/staticmesh.h
        src/transform.cpp
        src/transform.h
        src/triangle.cpp
        src/triangle.h
        src/vector.cpp
        src/vector.h
        src/vertex.h
        include/json.cpp
        include/json.h
        main.cpp
        )
