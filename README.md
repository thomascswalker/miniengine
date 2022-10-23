# miniengine

## What is this?
This is primarily a learning project for myself. My goal was to build a real-time software rasterizer without the aid of external libraries.

## Requirements
- Visual Studio 2022
- C++ 20
- Windows 10.0 SDK (latest)

![](https://github.com/thomascswalker/miniengine/blob/e99b5a63a63e2cab16f54befa923bdc81b38a659/teapot.gif)

## TODO
- Optimization of framerate
  - Conversion of std::vector -> void* where possible
  - Proper clipping
  - Cleanup pipeline
- Normals
- Lambertian Shading (default light position)
- Procedural texturing
- Textures from file
- Point light
