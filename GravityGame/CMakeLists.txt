# CMakeList.txt : CMake project for GravityGame, include source and define
# project-specific logic here.

# Specify the minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Project name and language standard
project(GravityGame)
set(CMAKE_CXX_STANDARD 20)

# Add source files to this project's executable
add_executable(GravityGame 
    renderer.cpp 
    VertexArray.cpp 
    VertexBuffer.cpp 
    IndexBuffer.cpp 
    Texture.cpp 
    Shader.cpp 
    GravityGame.cpp 
    vendor/stb_image/stb_image.cpp
    main.cpp
 "Sphere.h" "Sphere.cpp" "Camera.cpp" "Camera.h" "CelestialBody.h" "CelestialBody.cpp")

target_compile_definitions(GravityGame PRIVATE SHADERS_PATH="${CMAKE_SOURCE_DIR}/GravityGame/res/shaders")
target_compile_definitions(GravityGame PRIVATE TEXTURES_PATH="${CMAKE_SOURCE_DIR}/GravityGame/res/textures")

# Include directories for your project
target_include_directories(GravityGame PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb_image
)

# Add GLFW as a submodule and link it
add_subdirectory(vendor/glfw)
target_link_libraries(GravityGame PRIVATE glfw)

# Add ImGui as a submodule and link it
add_subdirectory(vendor/imgui)
target_link_libraries(GravityGame PRIVATE imgui)

add_subdirectory(vendor/glad)
target_link_libraries(GravityGame PRIVATE glad)

# Link OpenGL and GLAD
find_package(OpenGL REQUIRED)
target_link_libraries(GravityGame PRIVATE OpenGL::GL)
target_include_directories(GravityGame PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendor)
target_include_directories(GravityGame PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad)

# Define preprocessor macros for header-only libraries if needed
target_compile_definitions(GravityGame PRIVATE
    GLM_FORCE_RADIANS         # Use radians in GLM
    GLM_FORCE_DEPTH_ZERO_TO_ONE # Use Vulkan-style depth range
)