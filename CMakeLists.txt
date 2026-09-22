# Notes:
# With ninja we need to set build type by either the CMAKE_BUILD_TYPE variable
# or the command cmake -DCMAKE_BUILD_TYPE=<config>.

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

# Vulkan (Moved up so Vulkan::Vulkan target is available for ImGui)
find_package(Vulkan REQUIRED)

#### IMGUI ####
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui)
set(IMGUI_SOURCES
		${IMGUI_DIR}/imgui.cpp
		${IMGUI_DIR}/imgui_demo.cpp
		${IMGUI_DIR}/imgui_draw.cpp
		${IMGUI_DIR}/imgui_tables.cpp
		${IMGUI_DIR}/imgui_widgets.cpp
		${IMGUI_DIR}/backends/imgui_impl_win32.cpp  # Added Win32 platform backend
		${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp
)

add_library(imgui STATIC ${IMGUI_SOURCES})

target_include_directories(imgui PUBLIC
		${IMGUI_DIR}
		${IMGUI_DIR}/backends
)

target_link_libraries(imgui PUBLIC Vulkan::Vulkan)

add_subdirectory(shaders)

# Create executable
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
)

target_include_directories(raytracer
		PUBLIC include
)

target_link_libraries(raytracer
		PRIVATE
		Vulkan::Vulkan
		imgui
)