# List all the source files
set(SOURCES
    Framebuffer.cpp
    PrimitiveCube.cpp
    PrimitiveLine.cpp
    PrimitiveQuad.cpp
    Primitive.cpp
    Scene.cpp
    Shader.cpp
    SolidCS.cpp
    SolidGrid.cpp
    SolidSprite.cpp
    Texture.cpp
    Tilemap.cpp
    Window.cpp
)

# Inluce directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Create a library with all the source files
add_library(graphics STATIC ${SOURCES})

# Link libraries
target_link_libraries(graphics PUBLIC ${SDL2_LIBRARIES})