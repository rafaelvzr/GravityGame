# ImGui CMakeLists.txt

# Define the ImGui library
add_library(imgui
    imgui.cpp
    imgui_draw.cpp
    imgui_tables.cpp
    imgui_widgets.cpp
    imgui_demo.cpp
)

# Optionally add backends if needed (GLFW and OpenGL for your project)
target_sources(imgui PRIVATE
    backends/imgui_impl_glfw.cpp
    backends/imgui_impl_opengl3.cpp
)

# Include directories for ImGui headers
target_include_directories(imgui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/backends
)


# Link dependencies (GLFW and OpenGL) if needed
find_package(OpenGL REQUIRED)
target_link_libraries(imgui PUBLIC glfw OpenGL::GL)
