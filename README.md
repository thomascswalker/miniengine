# miniengine

## What is this?
This is primarily a learning project for myself. My goal was to build a real-time software rasterizer without the aid of external libraries.

## Requirements
- Visual Studio 2022
- C++ 20
- Windows 10.0 SDK (latest)

![](https://github.com/thomascswalker/miniengine/blob/ca63d62682592c27f7ef82e97fca5b7c4f1eb4a4/images/cow.gif)

## Progress
- [x] Launch window
- [x] Draw pixel buffer
- [x] Draw triangle
- [x] Project geometry
- [x] Control camera movement
- [x] Normals
- [ ] Conversion of `std::vector -> void*` where possible
- [ ] Proper clipping
- [ ] Cleanup pipeline
- [ ] Lambertian Shading (default light position)
- [ ] Procedural texturing
- [ ] Textures from file
- [ ] Point light
- [ ] .obj loader loads Faces

## Known bugs
- [ ] Performance tanks when 'inside' a model (or a model is behind the camera)
- [ ] Memory leak; performance degrades over time (after a minute or more)
