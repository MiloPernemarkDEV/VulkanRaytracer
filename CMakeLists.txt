# Notes:
# With ninja we need to set build type by either the CMAKE_BUILD_TYPE variabe# or the command cmake -DCMAKE_BUILD_TYPE=<config>.

cmake_minimum_required(VERSION 3.23)

# When cmake sees the project() command they will do checks to ensure we have a suitable environment for developing by checking compilers
# and build tooling
project(raytracer)

# Create a target which is a collection of properties that will be associated with it 
add_executable(raytracer WIN32)

# Generate compile_commands.json  
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "export compile commands")

# Vulkan
find_package(Vulkan REQUIRED)

target_compile_definitions(raytracer 
PRIVATE
	UNICODE
	_UNICODE
	WIN32_LEAN_AND_MEAN
	GLM_ENABLE_EXPERIMENTAL
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
)

target_include_directories(raytracer
PUBLIC include
)

target_link_libraries(raytracer
PRIVATE
  Vulkan::Vulkan
)