cmake_minimum_required (VERSION 3.10)
project(JnRMaker-Win)

# We are using C++ 17 and will make use of C++ exceptions.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /bigobj")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE "Debug")

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

# OpenGL
# set(OpenGL_GL_PRFERENCE "LEGACY")
# find_package(OpenGL REQUIRED)
# find_package(GLFW3 CONFIG REQUIRED)
# find_package(GLEW REQUIRED)
# include_directories(${OPENGL_INCLUDE_DIR} ${GLFW3_INCLUDE_DIRS} ${GLEW_INCLUDE_DIR})

# # Soil
# find_package(soil CONFIG REQUIRED)
# include_directories(${SOIL_INCLUDE_DIR})

# Cereal
# set(CEREAL_DIR ${THIRD_PARTY_DIR}/cereal)
# include_directories(${CEREAL_DIR}/include)
# add_subdirectory(${CEREAL_DIR} ${PROJECT_BINARY_DIR}/cereal.dir)

# Dear ImGui
# set(IMGUI_DIR ${THIRD_PARTY_DIR}/imgui)
# include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends ..)
# add_definitions(-DIMGUI_USER_CONFIG="${PROJECT_SOURCE_DIR}/${MAIN_INCLUDE_DIR}/ui/ImConfigJnRMaker.h")

# Implot
# set(IMPLOT_DIR ${THIRD_PARTY_DIR}/implot)
# include_directories(${IMPLOT_DIR})

# SDL2
# find_package(SDL2 REQUIRED)
# find_package(sdl2-image CONFIG REQUIRED)
# include_directories(${SDL2_INCLUDE_DIRS})

# cJSON
# set(CJSON_DIR ${THIRD_PARTY_DIR}/cJSON)
# include_directories(${CJSON_DIR})

# Boost
# set(Boost_USE_STATIC_LIBS OFF )
# set(Boost_USE_MULTITHREADED ON)
# set(Boost_USE_STATIC_RUNTIME OFF)
# set (Boost_DEBUG ON)

# find_package(Boost 1.69 COMPONENTS system filesystem serialization)
# IF(Boost_FOUND)
#     link_directories(${Boost_LIBRARY_DIRS})
#     include_directories(${Boost_INCLUDE_DIRS})
# ENDIF(Boost_FOUND)

# if (WIN32)
#     add_definitions( -DBOOST_ALL_NO_LIB )
#     add_definitions( -DBOOST_ALL_DYN_LINK )
# endif()

# stb
# set(STB_DIR ${THIRD_PARTY_DIR}/stb)
# include_directories(${STB_DIR})

# Googletest
# add_subdirectory(${THIRD_PARTY_DIR}/googletest ${PROJECT_BINARY_DIR}/googletest.dir)

# GLM
# set(GLM_DIR ${THIRD_PARTY_DIR}/glm)
# include_directories(${GLM_DIR})
# add_subdirectory(${GLM_DIR} ${PROJECT_BINARY_DIR}/glm.dir)

# json
# set(JSON_DIR ${THIRD_PARTY_DIR}/json)
# include_directories(${JSON_DIR}/single_include)

# RapidJSON
# set(RAPIDJSON_DIR ${THIRD_PARTY_DIR}/rapidjson)
# include_directories(${RAPIDJSON_DIR}/include)

# Native File Dialog
# set(NFD_DIR ${THIRD_PARTY_DIR}/nfd-extended)
# add_subdirectory(${NFD_DIR} ${PROJECT_BINARY_DIR}/nfd-extended.dir)
# include_directories(${NFD_DIR}/src/include)

# Copy data files
# file(COPY "../resources/" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}/resources")

# add_subdirectory(${MAIN_SOURCE_DIR}/entities ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/entities)
# add_subdirectory(${MAIN_SOURCE_DIR}/graphics ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/graphics)
# add_subdirectory(${MAIN_SOURCE_DIR}/ui ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/ui)
# add_subdirectory(${MAIN_SOURCE_DIR}/utils ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/utils)
add_subdirectory(${MAIN_SOURCE_DIR} app)

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
# add_executable(
#     jnrmaker
#     ${MAIN_SOURCE_DIR}/Main.cpp
# )

#target_link_libraries(jnrmaker app imgui bcrypt nfd GLEW::GLEW glfw SDL2::SDL2main SDL2::SDL2 SDL2::SDL2_image ${OPENGL_LIBRARIES} ${Boost_LIBRARIES})
#target_link_libraries(jnrmaker entities graphics ui utils imgui bcrypt nfd GLEW::GLEW glfw SDL2::SDL2main SDL2::SDL2 SDL2::SDL2_image ${OPENGL_LIBRARIES} ${Boost_LIBRARIES})

# install(TARGETS jnrmaker DESTINATION bin)