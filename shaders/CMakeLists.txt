find_program(DXC_EXECUTABLE NAMES dxc
	HINTS "$ENV{VULKAN_SDK}/Bin" "$ENV{VULKAN_SDK}/bin"
	REQUIRED
)

function(hlsl_entry_point SOURCE STAGE OUT_VAR)
	file(READ "${SOURCE}" HLSL_SOURCE)
	string(REGEX MATCH
		"\\[shader\\(\"${STAGE}\"\\)\\][ \t\r\n]*[A-Za-z_][A-Za-z0-9_]*[ \t\r\n]+([A-Za-z_][A-Za-z0-9_]*)[ \t\r\n]*\\("
		_
		"${HLSL_SOURCE}"
	)
	if(NOT CMAKE_MATCH_1)
		message(FATAL_ERROR "${SOURCE} is missing [shader(\"${STAGE}\")] with an entry function")
	endif()
	set(${OUT_VAR} "${CMAKE_MATCH_1}" PARENT_SCOPE)
endfunction()

file(GLOB_RECURSE SHADER_SOURCES CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/*.hlsl"
)

set(SHADER_SPV)
set(SHADER_OUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/spirv")
file(MAKE_DIRECTORY "${SHADER_OUT_DIR}")

foreach(SHADER_SRC ${SHADER_SOURCES})
	file(RELATIVE_PATH SHADER_REL "${CMAKE_CURRENT_SOURCE_DIR}" "${SHADER_SRC}")
	if(SHADER_REL MATCHES "^spirv[/\\]")
		continue()
	endif()

	cmake_path(GET SHADER_SRC STEM LAST_ONLY SHADER_STEM)

	hlsl_entry_point("${SHADER_SRC}" vertex VS_ENTRY)
	hlsl_entry_point("${SHADER_SRC}" pixel PS_ENTRY)

	set(VS_SPV "${SHADER_OUT_DIR}/${SHADER_STEM}.vs.spv")
	set(FS_SPV "${SHADER_OUT_DIR}/${SHADER_STEM}.fs.spv")

	add_custom_command(
		OUTPUT "${VS_SPV}"
		COMMAND "${DXC_EXECUTABLE}"
			-spirv -T vs_6_0
			-E "${VS_ENTRY}"
			-fspv-entrypoint-name=main
			"${SHADER_SRC}"
			-Fo "${VS_SPV}"
		DEPENDS "${SHADER_SRC}"
		COMMENT "Compiling ${SHADER_REL} (vertex)"
		VERBATIM
	)
	add_custom_command(
		OUTPUT "${FS_SPV}"
		COMMAND "${DXC_EXECUTABLE}"
			-spirv -T ps_6_0
			-E "${PS_ENTRY}"
			-fspv-entrypoint-name=main
			"${SHADER_SRC}"
			-Fo "${FS_SPV}"
		DEPENDS "${SHADER_SRC}"
		COMMENT "Compiling ${SHADER_REL} (pixel)"
		VERBATIM
	)
	list(APPEND SHADER_SPV "${VS_SPV}" "${FS_SPV}")
endforeach()

add_custom_target(shaders ALL DEPENDS ${SHADER_SPV})
