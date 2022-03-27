# geg

geg is a vulkan / DX12 game engine (currently vulkan only) I'm working on to understand how game engines structured and exploring this area of game/systems dev it's written in C++17 and glsl for shaders

## building

should build on windows and linux .. tested on windows only .. (will test on linux from while to while)

you have to have vulkanSDK installed with debug libs if u wanna build for debug

you can build with :

- windows MSVC17
- Linux Clang / GCC
<br>
Make sure your ssh fingerprint is uploaded to GitHub.com, see https://docs.github.com/en/authentication/connecting-to-github-with-ssh/testing-your-ssh-connection <br>
<br>
```bash
git clone --recurse-submodules git@github.com:thegeeko/geg
cd geg
mkdir build && cd build
cmake .. && cmake --build
```

## dependencies

- [spdlog](https://github.com/gabime/spdlog)
- [glfw](https://github.com/glfw/glfw)
- [glm](https://github.com/g-truc/glm)
- [imgui](https://github.com/ocornut/imgui)
- [Vulkan Memory Allocator](https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator)
- [assimp](https://github.com/assimp/assimp)
- [Entt](https://github.com/skypjack/entt)

## resources helped me a lot

- [thecherno](https://www.youtube.com/watch?v=JxIZbV_XjAs&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT) he is so helpful and very nice place to learn a lot of stuff
- [Brendan Galea](https://www.youtube.com/watch?v=Y9U9IE0gVHA&list=PL8327DO66nu9qYVKLDmdLW_84-yE4auCR) he has such a great Vulkan series and amazing community
- [Vulkan guide](https://vkguide.dev/)
- [Vulkan Tutorial](https://vulkan-tutorial.com/)
- [Vulkan Talks](https://www.youtube.com/channel/UCimEyovXN8JliEq49sqUGOQ)
