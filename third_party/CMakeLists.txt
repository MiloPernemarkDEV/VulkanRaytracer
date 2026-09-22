# ================================
# IMGUI
# ================================
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)

set(IMGUI_SOURCES
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp

        ${IMGUI_DIR}/backends/imgui_impl_win32.cpp
        ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp
)

add_library(imgui STATIC ${IMGUI_SOURCES})

# ================================
# IconFontsAwesome
# ================================
set(ICON_FONTS_AWESOME_DIR
        ${CMAKE_CURRENT_SOURCE_DIR}/IconFontCppHeaders
)

target_include_directories(imgui PUBLIC
        ${IMGUI_DIR}
        ${IMGUI_DIR}/backends
        ${ICON_FONTS_AWESOME_DIR}
)

target_link_libraries(imgui PUBLIC
        Vulkan::Vulkan
)