find_program(DXC_EXECUTABLE NAMES dxc
	HINTS "$ENV{VULKAN_SDK}/Bin" "$ENV{VULKAN_SDK}/bin"
)
find_program(GLSLC_EXECUTABLE NAMES glslc
	HINTS "$ENV{VULKAN_SDK}/Bin" "$ENV{VULKAN_SDK}/bin"
)

file(GLOB_RECURSE SHADER_SOURCES CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/*.hlsl"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.glsl"
)

set(SHADER_SPV)
foreach(SHADER_SRC ${SHADER_SOURCES})
	file(RELATIVE_PATH SHADER_REL "${CMAKE_CURRENT_SOURCE_DIR}" "${SHADER_SRC}")
	if(SHADER_REL MATCHES "^spirv[/\\]")
		continue()
	endif()

	cmake_path(GET SHADER_SRC FILENAME SHADER_NAME)
	cmake_path(GET SHADER_SRC STEM LAST_ONLY SHADER_STEM)
	cmake_path(GET SHADER_REL PARENT_PATH SHADER_SUBDIR)

	set(SHADER_OUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/spirv")
	if(SHADER_SUBDIR)
		string(APPEND SHADER_OUT_DIR "/${SHADER_SUBDIR}")
	endif()
	set(SHADER_OUT "${SHADER_OUT_DIR}/${SHADER_STEM}.spv")

	if(SHADER_NAME MATCHES "\\.vs\\.hlsl$")
		set(SHADER_COMPILER "${DXC_EXECUTABLE}")
		set(SHADER_ARGS -spirv -T vs_6_0 -E main "${SHADER_SRC}" -Fo "${SHADER_OUT}")
	elseif(SHADER_NAME MATCHES "\\.fs\\.hlsl$")
		set(SHADER_COMPILER "${DXC_EXECUTABLE}")
		set(SHADER_ARGS -spirv -T ps_6_0 -E main "${SHADER_SRC}" -Fo "${SHADER_OUT}")
	elseif(SHADER_NAME MATCHES "\\.cs\\.hlsl$")
		set(SHADER_COMPILER "${DXC_EXECUTABLE}")
		set(SHADER_ARGS -spirv -T cs_6_0 -E main "${SHADER_SRC}" -Fo "${SHADER_OUT}")
	elseif(SHADER_NAME MATCHES "\\.vs\\.glsl$")
		set(SHADER_COMPILER "${GLSLC_EXECUTABLE}")
		set(SHADER_ARGS -fshader-stage=vertex "${SHADER_SRC}" -o "${SHADER_OUT}")
	elseif(SHADER_NAME MATCHES "\\.fs\\.glsl$")
		set(SHADER_COMPILER "${GLSLC_EXECUTABLE}")
		set(SHADER_ARGS -fshader-stage=fragment "${SHADER_SRC}" -o "${SHADER_OUT}")
	elseif(SHADER_NAME MATCHES "\\.cs\\.glsl$")
		set(SHADER_COMPILER "${GLSLC_EXECUTABLE}")
		set(SHADER_ARGS -fshader-stage=compute "${SHADER_SRC}" -o "${SHADER_OUT}")
	else()
		continue()
	endif()

	if(NOT SHADER_COMPILER)
		message(FATAL_ERROR "No compiler found for ${SHADER_NAME}")
	endif()

	file(MAKE_DIRECTORY "${SHADER_OUT_DIR}")
	add_custom_command(
		OUTPUT "${SHADER_OUT}"
		COMMAND "${SHADER_COMPILER}" ${SHADER_ARGS}
		DEPENDS "${SHADER_SRC}"
		COMMENT "Compiling ${SHADER_REL}"
		VERBATIM
	)
	list(APPEND SHADER_SPV "${SHADER_OUT}")
endforeach()

add_custom_target(shaders ALL DEPENDS ${SHADER_SPV})
