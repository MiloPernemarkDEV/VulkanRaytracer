###### Notes: #################################################################
## With ninja we need to set build type by either the CMAKE_BUILD_TYPE variable
## or the command cmake -DCMAKE_BUILD_TYPE=Debug or Release.
###############################################################################

cmake_minimum_required(VERSION 3.23)
project(raytracer)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "export compile commands")
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

##### Vulkan #####
find_package(Vulkan REQUIRED)

add_subdirectory(shaders)
add_subdirectory(third_party)

target_link_libraries(imgui PUBLIC Vulkan::Vulkan)

add_executable(raytracer WIN32
		src/vulkan_pipelines.cpp
		src/ui.cpp
)

add_dependencies(raytracer shaders)

set_target_properties(raytracer PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

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
		src/ui.cpp
		src/viewport.cpp

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
		include/ui.h
		include/math_util.h
		include/vec3.h
		include/viewport.h
)

target_include_directories(raytracer
		PUBLIC include
)

target_link_libraries(raytracer
		PRIVATE
		Vulkan::Vulkan
		imgui
)