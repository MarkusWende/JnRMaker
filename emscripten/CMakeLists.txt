cmake_minimum_required(VERSION 3.4.1)
project(ANCMonitor)

# We are using C++ 17 and will make use of C++ exceptions.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE "Release")

set(THIRD_PARTY_DIR "../external")
set(MAIN_SOURCE_DIR "../src")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)

# Dear ImGui
set(IMGUI_DIR ${THIRD_PARTY_DIR}/imgui)
include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends ..)

# Implot
set(IMPLOT_DIR ${THIRD_PARTY_DIR}/implot)
include_directories(${IMPLOT_DIR})

# SDL2
find_package(SDL2 REQUIRED)
include_directories(${CMAKE_SOURCE_DIR}/include ${SDL2_INCLUDE_DIRS})

# cJSON
set(CJSON_DIR ${THIRD_PARTY_DIR}/cJSON)
include_directories(${CJSON_DIR})

add_library(
    app
    ${MAIN_SOURCE_DIR}/gui.hpp
    ${MAIN_SOURCE_DIR}/gui.cpp
    ${MAIN_SOURCE_DIR}/message_manager.hpp
    ${MAIN_SOURCE_DIR}/message_manager.cpp
    ${MAIN_SOURCE_DIR}/metric_manager.hpp
    ${MAIN_SOURCE_DIR}/metric_manager.cpp
    ${MAIN_SOURCE_DIR}/time_helper.hpp
    ${MAIN_SOURCE_DIR}/ws_emscripten.hpp
    ${MAIN_SOURCE_DIR}/ws_emscripten.cpp
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

add_library(
    implot
    ${IMPLOT_DIR}/implot.h
    ${IMPLOT_DIR}/implot_internal.h
    ${IMPLOT_DIR}/implot.cpp
    ${IMPLOT_DIR}/implot_demo.cpp
    ${IMPLOT_DIR}/implot_items.cpp
)

add_library(
    cjson
    ${CJSON_DIR}/cJSON.h
    ${CJSON_DIR}/cJSON_Utils.h
    ${CJSON_DIR}/cJSON.c
    ${CJSON_DIR}/cJSON_Utils.c
)

# The 'ancmonitor' executable
add_executable(
    ancmonitor
    ${MAIN_SOURCE_DIR}/ANCMonitor.cpp
)

target_link_libraries(ancmonitor app imgui implot cjson ${SDL2_LIBRARIES})

# These properties specify what kind of Emscripten build to perform and are assigned to our 'ancmonitor' executable target.
set_target_properties(
    ancmonitor
    PROPERTIES
    LINK_FLAGS
    "-O2 -s USE_SDL=2 -s USE_WEBGL2=1 -s INITIAL_MEMORY=134217728 -lwebsocket.js -lidbfs.js --no-heap-copy --preload-file ../../data/fonts@/fonts"
)

# When the build is complete we will run the 'cmake-post-build.sh' script.
add_custom_command(
    TARGET ancmonitor
    POST_BUILD
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMAND ./cmake-post-build.sh
)