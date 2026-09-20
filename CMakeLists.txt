# Notes:
# With ninja we need to set build type by either the CMAKE_BUILD_TYPE variabe# or the command cmake -DCMAKE_BUILD_TYPE=<config>.

cmake_minimum_required(VERSION 3.23)

# When cmake sees the project() command they will do checks to ensure we have a suitable environment for developing by checking compilers
# and build tooling
project(raytracer)

# Generate compile_commands.json  
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "export compile commands")

# set cpp standard
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(shaders)

# Create a target which is a collection of properties that will be associated with it
add_executable(raytracer WIN32
		src/vulkan_commands.cpp
		src/vulkan_image.cpp
		src/vulkan_pipelines.cpp)

add_dependencies(raytracer shaders)

# Vulkan
find_package(Vulkan REQUIRED)

target_compile_definitions(raytracer 
PRIVATE
	UNICODE
	_UNICODE
	WIN32_LEAN_AND_MEAN
	GLM_ENABLE_EXPERIMENTAL
	_CRT_USE_BUILTIN_OFFSETOF
)

target_precompile_headers(raytracer 
PRIVATE 
include/pch.h
)

target_sources(raytracer
PRIVATE
	src/main.cpp
	src/window.cpp
	src/application.cpp
	src/arena.cpp
	src/renderer.cpp
	src/vulkan_core.cpp
	src/gpu_export.cpp
	src/vulkan_commands.cpp
	src/vulkan_sync.cpp
	src/vulkan_image.cpp
	src/shader.cpp

PUBLIC
	FILE_SET includes
	TYPE HEADERS
	BASE_DIRS
		include
	FILES
	include/defines.h
	include/window.h	
	include/application.h
	include/arena.h
	include/config.h
	include/pch.h
	include/renderer.h
	include/vulkan_core.h
	include/vertex.h
	include/win32_helper.h
	include/vulkan_commands.h
	include/vulkan_sync.h
	include/vulkan_image.h
	include/shader.h
)

target_include_directories(raytracer
PUBLIC include
)

target_link_libraries(raytracer
PRIVATE
  Vulkan::Vulkan
)