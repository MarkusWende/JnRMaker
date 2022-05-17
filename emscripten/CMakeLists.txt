cmake_minimum_required (VERSION 3.6)
project(JnRMaker)

# We are using C++ 17 and will make use of C++ exceptions.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions -Wall -Wextra -Wpedantic -pthread")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE "Debug")

set(THIRD_PARTY_DIR "../external")
set(MAIN_SOURCE_DIR "../src")
set(MAIN_INCLUDE_DIR "../include")
include_directories(${MAIN_INCLUDE_DIR})

# # OpenGL
# set(OpenGL_GL_PRFERENCE "LEGACY")
# find_package(OpenGL REQUIRED)
# find_package(glfw3 3.2 REQUIRED)
# find_package(GLEW REQUIRED)
# include_directories(${OPENGL_INCLUDE_DIR} ${GLFW3_INCLUDE_DIRS} ${GLEW_INCLUDE_DIR})

# # Soil
# find_package(soil CONFIG REQUIRED)
# include_directories(${SOIL_INCLUDE_DIR})

# Cereal
#add_subdirectory(${THIRD_PARTY_DIR}/cereal ${PROJECT_BINARY_DIR}/cereal.dir)

# Dear ImGui
set(IMGUI_DIR ${THIRD_PARTY_DIR}/imgui)
include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends ..)
add_definitions(-DIMGUI_USER_CONFIG="${PROJECT_SOURCE_DIR}/${MAIN_INCLUDE_DIR}/imconfig_jnrmaker.h")

# Implot
# set(IMPLOT_DIR ${THIRD_PARTY_DIR}/implot)
# include_directories(${IMPLOT_DIR})

# SDL2
find_package(SDL2 REQUIRED)
# find_package(sdl2-image CONFIG REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# cJSON
# set(CJSON_DIR ${THIRD_PARTY_DIR}/cJSON)
# include_directories(${CJSON_DIR})

# # Boost
# set(Boost_USE_STATIC_LIBS ON)
# set(Boost_USE_STATIC_RUNTIME ON)

# find_package(Boost 1.69 COMPONENTS system filesystem)
# IF(Boost_FOUND)
#     link_directories(${Boost_LIBRARY_DIRS})
#     include_directories(${Boost_INCLUDE_DIRS})
# ENDIF(Boost_FOUND)

# stb
set(STB_DIR ${THIRD_PARTY_DIR}/stb)
include_directories(${STB_DIR})

# glm
set(GLM_DIR ${THIRD_PARTY_DIR}/glm)
include_directories(${GLM_DIR})

# Googletest
#add_subdirectory(${THIRD_PARTY_DIR}/googletest ${PROJECT_BINARY_DIR}/googletest.dir)

# # Copy data files
# file(COPY "../data/" DESTINATION "${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/data")
# file(COPY "../src/shaders/" DESTINATION "${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/src/shaders")

add_library(
    app
    ${MAIN_SOURCE_DIR}/entity_level_layer.cpp
    ${MAIN_SOURCE_DIR}/entity_solid.cpp
    ${MAIN_SOURCE_DIR}/framebuffer.cpp
    ${MAIN_SOURCE_DIR}/gui.cpp
    ${MAIN_SOURCE_DIR}/message_manager.cpp
    ${MAIN_SOURCE_DIR}/primitive_cube.cpp
    ${MAIN_SOURCE_DIR}/primitive_line.cpp
    ${MAIN_SOURCE_DIR}/primitive_quad.cpp
    ${MAIN_SOURCE_DIR}/primitive.cpp
    ${MAIN_SOURCE_DIR}/project_manager.cpp
    ${MAIN_SOURCE_DIR}/resource_manager.cpp
    ${MAIN_SOURCE_DIR}/scene.cpp
    ${MAIN_SOURCE_DIR}/shader.cpp
    ${MAIN_SOURCE_DIR}/solid_cs.cpp
    ${MAIN_SOURCE_DIR}/solid_grid.cpp
    ${MAIN_SOURCE_DIR}/solid_sprite.cpp
    ${MAIN_SOURCE_DIR}/texture.cpp
    ${MAIN_SOURCE_DIR}/tilemap.cpp
    ${MAIN_SOURCE_DIR}/tilemap_manager.cpp
    ${MAIN_SOURCE_DIR}/window.cpp
)

add_library(
    imgui
    ${IMGUI_DIR}/backends/imgui_impl_sdl.h
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.h
    ${IMGUI_DIR}/backends/imgui_impl_sdl.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
    ${IMGUI_DIR}/imgui.h
    ${IMGUI_DIR}/imconfig.h
    ${IMGUI_DIR}/imgui_internal.h
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
)

# The 'jnrmaker' executable
add_executable(
    jnrmaker
    ${MAIN_SOURCE_DIR}/main.cpp
)

target_link_libraries(jnrmaker app imgui ${SDL2_LIBRARIES})

#add_definitions("-s EXPORTED_RUNTIME_METHODS='[\"ccall\",\"cwrap\"]'")
#add_definitions("-s EXTRA_EXPORTED_RUNTIME_METHODS='[\"ccall\",\"cwrap\"]'")
#add_definitions("-s EXPORTED_FUNCTIONS='[\"_sync_tunnel\"]'")

# These properties specify what kind of Emscripten build to perform and are assigned to our 'jnrmaker' executable target.
set_target_properties(jnrmaker PROPERTIES LINK_FLAGS "-g -sASSERTIONS=1 -Os -sUSE_PTHREADS=1 -sPTHREAD_POOL_SIZE=2 -sUSE_WEBGL2=1 -s MIN_WEBGL_VERSION=2 -s MAX_WEBGL_VERSION=2 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_BOOST_HEADERS=1 -s INITIAL_MEMORY=300MB -s EXPORTED_RUNTIME_METHODS=lengthBytesUTF8,stringToUTF8 -s FETCH -lidbfs.js -lembind -sASYNCIFY --no-heap-copy --preload-file ../../data@/data --preload-file ../../src/shaders@/src/shaders")

# When the build is complete we will run the 'cmake-post-build.sh' script.
add_custom_command(
    TARGET jnrmaker
    POST_BUILD
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMAND ./cmake-post-build.sh
)
