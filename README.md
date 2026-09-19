# Vulkan Ray Tracing Renderer

A from-scratch real-time ray tracing renderer built with C++ and Vulkan.

The project is primarily a learning project focused on understanding Vulkan and GPU rendering at a low level, while gradually building a functional ray tracing renderer.

## Current Progress

* [x] Win32 abstraction
* [x] Vulkan initialization
* [x] Physical/logical device selection
* [x] Swapchain

## Goals

The goal is to keep the renderer relatively small and focused while gaining a deeper understanding of Vulkan, GPU memory, synchronization, and hardware-accelerated ray tracing.

Feel free to ask me questions or follow along!

## How to Build and Run the Renderer

### Dependencies and Environment

The renderer currently requires:

* **Windows**
* **Vulkan SDK** https://vulkan.lunarg.com/sdk/home
* A GPU with **Vulkan 1.3** and **hardware ray tracing** support
* **CMake 3.23** or newer
* A **C++20** compatible compiler
* Any CMake-supported build generator

### Build

Configure the project with CMake from the project's root directory:

```bash
cmake -B build
```

This will use CMake's default generator. You can also specify a generator explicitly.

For example, I use **Ninja**:

```bash
cmake -B build -G Ninja
```

Then build the project:

```bash
cmake --build build
```

The generated build files will be placed in the `build` directory.

### Run

After building, run `raytracer.exe` from the generated build directory.
