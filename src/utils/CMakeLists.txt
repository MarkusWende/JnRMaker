# List all the source files
set(SOURCES
    MessageManager.cpp
    ProjectManager.cpp
    ResourceManager.cpp
    TilemapManager.cpp
)

# Inluce directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Create a library with all the source files
add_library(utils STATIC ${SOURCES})

add_subdirectory(logging logging)

# Link libraries
target_link_libraries(utils PUBLIC logging ${SDL2_LIBRARIES})