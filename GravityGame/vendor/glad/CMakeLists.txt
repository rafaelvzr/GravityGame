# CMakeLists.txt for GLAD

# Create a library for GLAD
add_library(glad STATIC src/glad.c)

# Include directories for GLAD
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
