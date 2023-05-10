cmake_minimum_required (VERSION 3.10)
project(JnRMaker-Web)

# We are using C++ 17 and will make use of C++ exceptions.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions -Wall -Wextra -Wpedantic -pthread")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE "Release")

# set(THIRD_PARTY_DIR "../external")
# set(MAIN_SOURCE_DIR "../src")
# set(MAIN_INCLUDE_DIR "../include")
# set(RESOURCES_DIR "../resources")
# include_directories(${MAIN_INCLUDE_DIR})
# include_directories(${MAIN_INCLUDE_DIR}/utils)
# include_directories(${MAIN_INCLUDE_DIR}/utils/logging)
# include_directories(${MAIN_INCLUDE_DIR}/entities)
# include_directories(${MAIN_INCLUDE_DIR}/graphics)
# include_directories(${MAIN_INCLUDE_DIR}/ui)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(CMAKE_BINARY_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/archive)

# file(GLOB libraryJsFiles ${RESOURCES_DIR}/www/js/*.js)

# if (CMAKE_BUILD_TYPE STREQUAL Debug)
# 	SET(linkFlags "-g3 -s NO_EXIT_RUNTIME=1 -sASSERTIONS=1")
# else() # Either MinSizeRel, RelWithDebInfo or Release, all which run with optimizations enabled.
# 	SET(linkFlags "-O2 -s NO_EXIT_RUNTIME=1")
# endif()

# # Ensure synchronous startup for this test, whose output expects it
# SET(linkFlags "${linkFlags} -sUSE_PTHREADS=1 -sPTHREAD_POOL_SIZE=4")
# SET(linkFlags "${linkFlags} -sUSE_WEBGL2=1 -sMIN_WEBGL_VERSION=2 -sMAX_WEBGL_VERSION=2")
# SET(linkFlags "${linkFlags} -sUSE_SDL=2 -sUSE_SDL_IMAGE=2 -sUSE_BOOST_HEADERS=1")
# SET(linkFlags "${linkFlags} -sFETCH -sEXPORTED_FUNCTIONS=_getJSON,_main,_getLocalTilemapFile,_saveJSONFile,_viewFullscreen -sEXPORTED_RUNTIME_METHODS=ccall,cwrap")
# SET(linkFlags "${linkFlags} -sINITIAL_MEMORY=300MB -lidbfs.js -lembind --no-heap-copy")
# SET(linkFlags "${linkFlags} --preload-file ../../resources@/resources")

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
# set(CEREAL_DIR ${THIRD_PARTY_DIR}/cereal)
# include_directories(${CEREAL_DIR}/include)
# add_subdirectory(${CEREAL_DIR} ${PROJECT_BINARY_DIR}/cereal.dir)

# # Dear ImGui
# set(IMGUI_DIR ${THIRD_PARTY_DIR}/imgui)
# include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends ..)
# add_definitions(-DIMGUI_USER_CONFIG="${PROJECT_SOURCE_DIR}/${MAIN_INCLUDE_DIR}/ui/ImConfigJnRMaker.h")

# Implot
# set(IMPLOT_DIR ${THIRD_PARTY_DIR}/implot)
# include_directories(${IMPLOT_DIR})

# # SDL2
# find_package(SDL2 REQUIRED)
# # find_package(sdl2-image CONFIG REQUIRED)
# include_directories(${SDL2_INCLUDE_DIRS})

# # cJSON
# # set(CJSON_DIR ${THIRD_PARTY_DIR}/cJSON)
# # include_directories(${CJSON_DIR})

# # # Boost
# # set(Boost_USE_STATIC_LIBS ON)
# # set(Boost_USE_STATIC_RUNTIME ON)

# # find_package(Boost 1.69 COMPONENTS system filesystem)
# # IF(Boost_FOUND)
# #     link_directories(${Boost_LIBRARY_DIRS})
# #     include_directories(${Boost_INCLUDE_DIRS})
# # ENDIF(Boost_FOUND)

# # stb
# set(STB_DIR ${THIRD_PARTY_DIR}/stb)
# include_directories(${STB_DIR})

# # glm
# set(GLM_DIR ${THIRD_PARTY_DIR}/glm)
# include_directories(${GLM_DIR})

# # json
# set(JSON_DIR ${THIRD_PARTY_DIR}/json)
# include_directories(${JSON_DIR}/single_include)

# # RapidJSON
# set(RAPIDJSON_DIR ${THIRD_PARTY_DIR}/rapidjson)
# include_directories(${RAPIDJSON_DIR}/include)

# Googletest
#add_subdirectory(${THIRD_PARTY_DIR}/googletest ${PROJECT_BINARY_DIR}/googletest.dir)

# add_subdirectory(${MAIN_SOURCE_DIR}/entities ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/entities)
# add_subdirectory(${MAIN_SOURCE_DIR}/graphics ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/graphics)
# add_subdirectory(${MAIN_SOURCE_DIR}/ui ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/ui)
# add_subdirectory(${MAIN_SOURCE_DIR}/utils ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/utils)
add_subdirectory(../src app)

# add_library(
#     app
#     ${MAIN_SOURCE_DIR}/entities/EntityWorldLayer.cpp
#     ${MAIN_SOURCE_DIR}/entities/EntitySolid.cpp
#     ${MAIN_SOURCE_DIR}/graphics/Framebuffer.cpp
#     ${MAIN_SOURCE_DIR}/graphics/Primitive.cpp
#     ${MAIN_SOURCE_DIR}/graphics/PrimitiveCube.cpp
#     ${MAIN_SOURCE_DIR}/graphics/PrimitiveLine.cpp
#     ${MAIN_SOURCE_DIR}/graphics/PrimitiveQuad.cpp
#     ${MAIN_SOURCE_DIR}/graphics/Scene.cpp
#     ${MAIN_SOURCE_DIR}/graphics/Shader.cpp
#     ${MAIN_SOURCE_DIR}/graphics/SolidCS.cpp
#     ${MAIN_SOURCE_DIR}/graphics/SolidGrid.cpp
#     ${MAIN_SOURCE_DIR}/graphics/SolidSprite.cpp
#     ${MAIN_SOURCE_DIR}/graphics/Texture.cpp
#     ${MAIN_SOURCE_DIR}/graphics/Tilemap.cpp
#     ${MAIN_SOURCE_DIR}/graphics/Window.cpp
#     ${MAIN_SOURCE_DIR}/ui/Gui.cpp
#     ${MAIN_SOURCE_DIR}/utils/logging/SDLLogger.cpp
#     ${MAIN_SOURCE_DIR}/utils/MessageManager.cpp
#     ${MAIN_SOURCE_DIR}/utils/ProjectManager.cpp
#     ${MAIN_SOURCE_DIR}/utils/ResourceManager.cpp
#     ${MAIN_SOURCE_DIR}/utils/TilemapManager.cpp
# )

# add_library(
#     imgui
#     ${IMGUI_DIR}/backends/imgui_impl_sdl2.h
#     ${IMGUI_DIR}/backends/imgui_impl_opengl3.h
#     ${IMGUI_DIR}/backends/imgui_impl_sdl2.cpp
#     ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
#     ${IMGUI_DIR}/imgui.h
#     ${IMGUI_DIR}/imconfig.h
#     ${IMGUI_DIR}/imgui_internal.h
#     ${IMGUI_DIR}/imgui.cpp
#     ${IMGUI_DIR}/imgui_draw.cpp
#     ${IMGUI_DIR}/imgui_demo.cpp
#     ${IMGUI_DIR}/imgui_tables.cpp
#     ${IMGUI_DIR}/imgui_widgets.cpp
# )

# # The 'jnrmaker' executable
# add_executable(
#     jnrmaker
#     ${MAIN_SOURCE_DIR}/Main.cpp
# )

# target_link_libraries(jnrmaker app imgui ${SDL2_LIBRARIES})

#add_definitions("-s EXPORTED_RUNTIME_METHODS='[\"ccall\",\"cwrap\"]'")
#add_definitions("-s EXTRA_EXPORTED_RUNTIME_METHODS='[\"ccall\",\"cwrap\"]'")
#add_definitions("-s EXPORTED_FUNCTIONS='[\"_sync_tunnel\"]'")

# # These properties specify what kind of Emscripten build to perform and are assigned to our 'jnrmaker' executable target.
# set_target_properties(jnrmaker PROPERTIES LINK_FLAGS "${linkFlags}")
# #set_target_properties(jnrmaker PROPERTIES LINK_FLAGS "-g -sASSERTIONS=1 -Os -sUSE_PTHREADS=1 -sPTHREAD_POOL_SIZE=2 -sUSE_WEBGL2=1 -s MIN_WEBGL_VERSION=2 -s MAX_WEBGL_VERSION=2 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_BOOST_HEADERS=1 -s INITIAL_MEMORY=300MB -s EXPORTED_RUNTIME_METHODS=lengthBytesUTF8,stringToUTF8 -s FETCH -lidbfs.js -lembind -sASYNCIFY --no-heap-copy --preload-file ../../data@/data --preload-file ../../src/shaders@/src/shaders")


# message(STATUS "js libs '${libraryJsFiles}'")
# # To link .js files using the --js-library flag, use the following helper function.
# em_link_js_library(jnrmaker ${libraryJsFiles})

# add_custom_command(
#     TARGET jnrmaker
#     POST_BUILD
#     WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
#     COMMAND ./cmake-post-build.sh
# )