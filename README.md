# miniengine

## What is this?
This is primarily a learning project for myself. My goal was to build a real-time software rasterizer without the aid of external libraries.

## Requirements
- Visual Studio 2022
- C++ 20
- Windows 10.0 SDK (latest)

## Wiki
Follow this [link](https://github.com/thomascswalker/miniengine/wiki) to view the wiki.

## Example
![](https://github.com/thomascswalker/miniengine/blob/af18643c20aff401325ff25a2ff8983e489b1999/images/example.gif)

## Progress
- [x] Launch window
- [x] Draw pixel buffer
- [x] Draw triangle
- [x] Project geometry
- [x] Control camera movement
- [x] Normals
- [x] Conversion of `std::vector -> void*` where possible
- [x] Proper clipping
- [x] Cleanup pipeline
- [ ] Lambertian Shading (default light position)
- [ ] Procedural texturing
- [ ] Textures from file
- [ ] Point light
- [x] .obj loader loads Faces

## Known bugs
- [ ] Memory leak; performance degrades over time (after a minute or more)
