# List all the source files
set(SOURCES
    EntityWorldLayer.cpp
    EntitySolid.cpp
)

# Inluce directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Create a library with all the source files
add_library(entities STATIC ${SOURCES})

# Link libraries
target_link_libraries(entities PUBLIC ${SDL2_LIBRARIES})