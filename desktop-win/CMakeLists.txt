cmake_minimum_required (VERSION 3.6)
project(JnRMaker)

# We are using C++ 17 and will make use of C++ exceptions.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /bigobj")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE "Debug")

set(THIRD_PARTY_DIR "../external")
set(MAIN_SOURCE_DIR "../src")
set(MAIN_INCLUDE_DIR "../include")
include_directories(${MAIN_INCLUDE_DIR})

# OpenGL
set(OpenGL_GL_PRFERENCE "LEGACY")
find_package(OpenGL REQUIRED)
find_package(GLFW3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR} ${GLFW3_INCLUDE_DIRS} ${GLEW_INCLUDE_DIR})

# # Soil
# find_package(soil CONFIG REQUIRED)
# include_directories(${SOIL_INCLUDE_DIR})

# Cereal
set(CEREAL_DIR ${THIRD_PARTY_DIR}/cereal)
include_directories(${CEREAL_DIR}/include)
add_subdirectory(${CEREAL_DIR} ${PROJECT_BINARY_DIR}/cereal.dir)

# Dear ImGui
set(IMGUI_DIR ${THIRD_PARTY_DIR}/imgui)
include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends ..)
add_definitions(-DIMGUI_USER_CONFIG="${PROJECT_SOURCE_DIR}/${MAIN_INCLUDE_DIR}/imconfig_jnrmaker.h")

# Implot
# set(IMPLOT_DIR ${THIRD_PARTY_DIR}/implot)
# include_directories(${IMPLOT_DIR})

# SDL2
find_package(SDL2 REQUIRED)
find_package(sdl2-image CONFIG REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# cJSON
# set(CJSON_DIR ${THIRD_PARTY_DIR}/cJSON)
# include_directories(${CJSON_DIR})

# Boost
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME ON)

find_package(Boost 1.69 COMPONENTS system filesystem)
IF(Boost_FOUND)
    link_directories(${Boost_LIBRARY_DIRS})
    include_directories(${Boost_INCLUDE_DIRS})
ENDIF(Boost_FOUND)

# stb
set(STB_DIR ${THIRD_PARTY_DIR}/stb)
include_directories(${STB_DIR})

# Googletest
add_subdirectory(${THIRD_PARTY_DIR}/googletest ${PROJECT_BINARY_DIR}/googletest.dir)

# GLM
set(GLM_DIR ${THIRD_PARTY_DIR}/glm)
include_directories(${GLM_DIR})
add_subdirectory(${GLM_DIR} ${PROJECT_BINARY_DIR}/glm.dir)

# json
set(JSON_DIR ${THIRD_PARTY_DIR}/json)
include_directories(${JSON_DIR}/single_include)

# Copy data files
file(COPY "../data/" DESTINATION "${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/data")
file(COPY "../src/shaders/" DESTINATION "${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/src/shaders")

add_library(
    app
    ${MAIN_SOURCE_DIR}/entity_world_layer.cpp
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
    ${IMGUI_DIR}/backends/imgui_impl_sdl2.h
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.h
    ${IMGUI_DIR}/backends/imgui_impl_sdl2.cpp
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

# add_library(
#     implot
#     ${IMPLOT_DIR}/implot.h
#     ${IMPLOT_DIR}/implot_internal.h
#     ${IMPLOT_DIR}/implot.cpp
#     ${IMPLOT_DIR}/implot_demo.cpp
#     ${IMPLOT_DIR}/implot_items.cpp
# )

# add_library(
#     cjson
#     ${CJSON_DIR}/cJSON.h
#     ${CJSON_DIR}/cJSON_Utils.h
#     ${CJSON_DIR}/cJSON.c
#     ${CJSON_DIR}/cJSON_Utils.c
# )

# The 'ancmonitor' executable
add_executable(
    jnrmaker
    ${MAIN_SOURCE_DIR}/main.cpp
)

target_link_libraries(jnrmaker app imgui cereal GLEW::GLEW glfw SDL2::SDL2main SDL2::SDL2 SDL2::SDL2_image ${OPENGL_LIBRARIES})

install(TARGETS jnrmaker DESTINATION bin)