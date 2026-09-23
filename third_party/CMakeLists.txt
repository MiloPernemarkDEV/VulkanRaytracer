set(THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(THIRD_PARTY_SOURCES
        ${THIRD_PARTY_DIR}/imgui/imgui.cpp
        ${THIRD_PARTY_DIR}/imgui/imgui_demo.cpp
        ${THIRD_PARTY_DIR}/imgui/imgui_draw.cpp
        ${THIRD_PARTY_DIR}/imgui/imgui_tables.cpp
        ${THIRD_PARTY_DIR}/imgui/imgui_widgets.cpp
        ${THIRD_PARTY_DIR}/imgui/backends/imgui_impl_win32.cpp
        ${THIRD_PARTY_DIR}/imgui/backends/imgui_impl_vulkan.cpp
)

add_library(third_party STATIC ${THIRD_PARTY_SOURCES})

target_include_directories(third_party PUBLIC
        ${THIRD_PARTY_DIR}/imgui
        ${THIRD_PARTY_DIR}/imgui/backends
        ${THIRD_PARTY_DIR}/IconFontCppHeaders
        ${THIRD_PARTY_DIR}/vma/include
)

target_link_libraries(third_party PUBLIC
        Vulkan::Vulkan
)
