add_definitions(-DSTB_IMAGE_STATIC)


cmake_minimum_required(VERSION 3.10)
project(img_filterer)

set(CMAKE_CXX_STANDARD 17)

# ---------------------------------------------------------
# Common source files (shared between GUI and CLI)
# ---------------------------------------------------------
set(SHARED_SOURCES
    lib/ImageEditor.cpp
    lib/ImageEditor.h
    lib/Image_Class.h
    lib/stb_singleton.cpp
    lib/stb_write_singleton.cpp
)


# ---------------------------------------------------------
# GUI version (Dear ImGui + OpenGL + GLFW)
# ---------------------------------------------------------
add_executable(photorafy
    gui.cpp
    ${SHARED_SOURCES}
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui_demo.cpp
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp
    imgui/ImGuiFileDialog.cpp
)

target_include_directories(photorafy PRIVATE
    ${CMAKE_SOURCE_DIR}/lib
    ${CMAKE_SOURCE_DIR}/imgui
    ${CMAKE_SOURCE_DIR}/imgui/backends
    ${CMAKE_SOURCE_DIR}/glfw/include
)

target_link_directories(photorafy PRIVATE
    ${CMAKE_SOURCE_DIR}/glfw/lib-mingw-w64
)

target_link_libraries(photorafy
    glfw3
    opengl32
    gdi32
    user32
    shell32
)

# Prevent console window from appearing in GUI version
set_target_properties(photorafy PROPERTIES WIN32_EXECUTABLE TRUE)
