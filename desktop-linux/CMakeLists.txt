cmake_minimum_required (VERSION 3.10)
project(JnRMaker-Linux)

# We are using C++ 17 and will make use of C++ exceptions.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE "Debug")

set(THIRD_PARTY_DIR "../external")
set(MAIN_SOURCE_DIR "../src")
set(MAIN_INCLUDE_DIR "../include")
set(RESOURCES_DIR "../resources")
include_directories(${MAIN_INCLUDE_DIR})
include_directories(${MAIN_INCLUDE_DIR}/utils)
include_directories(${MAIN_INCLUDE_DIR}/utils/logging)
include_directories(${MAIN_INCLUDE_DIR}/entities)
include_directories(${MAIN_INCLUDE_DIR}/graphics)
include_directories(${MAIN_INCLUDE_DIR}/ui)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(CMAKE_BINARY_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/archive)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")
set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}")

# OpenGL
set(OpenGL_GL_PRFERENCE "LEGACY")
find_package(OpenGL REQUIRED)
find_package(glfw3 3.2 REQUIRED)
find_package(GLEW REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR} ${GLFW3_INCLUDE_DIRS} ${GLEW_INCLUDE_DIR})

# # Soil
# find_package(soil CONFIG REQUIRED)
# include_directories(${SOIL_INCLUDE_DIR})

# Cereal
# add_subdirectory(${THIRD_PARTY_DIR}/cereal ${PROJECT_BINARY_DIR}/cereal.dir)

# Dear ImGui
set(IMGUI_DIR ${THIRD_PARTY_DIR}/imgui)
include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends ..)
add_definitions(-DIMGUI_USER_CONFIG="${PROJECT_SOURCE_DIR}/${MAIN_INCLUDE_DIR}/imconfig_jnrmaker.h")

# Implot
# set(IMPLOT_DIR ${THIRD_PARTY_DIR}/implot)
# include_directories(${IMPLOT_DIR})

# SDL2
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# cJSON
# set(CJSON_DIR ${THIRD_PARTY_DIR}/cJSON)
# include_directories(${CJSON_DIR})

# json
set(JSON_DIR ${THIRD_PARTY_DIR}/json)
include_directories(${JSON_DIR}/single_include)

# RapidJSON
set(RAPIDJSON_DIR ${THIRD_PARTY_DIR}/rapidjson)
include_directories(${RAPIDJSON_DIR}/include)

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

# Native File Dialog
set(NFD_DIR ${THIRD_PARTY_DIR}/nfd-extended)
add_subdirectory(${NFD_DIR} ${PROJECT_BINARY_DIR}/nfd-extended.dir)
include_directories(${NFD_DIR}/src/include)

# Copy data files
file(COPY "../resources/" DESTINATION "${PROJECT_BINARY_DIR}/resources")

add_subdirectory(${MAIN_INCLUDE_DIR}/entities)
add_subdirectory(${MAIN_INCLUDE_DIR}/graphics)
add_subdirectory(${MAIN_INCLUDE_DIR}/ui)
add_subdirectory(${MAIN_INCLUDE_DIR}/utils)

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
    ${MAIN_SOURCE_DIR}/Main.cpp
)

#target_link_libraries(jnrmaker app imgui nfd GLEW::GLEW glfw ${SDL2_LIBRARIES} SDL2::Image ${OPENGL_LIBRARIES} -ldl stdc++fs)
target_link_libraries(jnrmaker entities graphics ui utils imgui nfd GLEW::GLEW glfw ${SDL2_LIBRARIES} SDL2::Image ${OPENGL_LIBRARIES} -ldl stdc++fs)


# #Change this if you need to target a specific CMake version
# cmake_minimum_required (VERSION 3.6)
# set(CMAKE_BUILD_TYPE Debug)

# # Enable debug symbols by default
# # must be done before project() statement
# IF(NOT CMAKE_BUILD_TYPE)
#   SET(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
# ENDIF(NOT CMAKE_BUILD_TYPE)
# # (you can also set it on the command line: -D CMAKE_BUILD_TYPE=Release)

# project (JnRMaker)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# # Set version information in a config.h file
# set(JNRMAKER_VERSION_MAJOR 0)
# set(JNRMAKER_VERSION_MINOR 1)
# #configure_file("./include/config.h.in" "${PROJECT_BINARY_DIR}/include/config.h")
# include_directories("${PROJECT_BINARY_DIR}")

# if (UNIX)
#   set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")
#   set (CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}")
# endif (UNIX)


# IF (WIN32)
#   set(DCMAKE_TOOLCHAIN_FILE "D:/Programme/vcpkg/scripts/buildsystems/vcpkg.cmake")
#   include_directories(D:/Programme/vcpkg/installed/x64-windows/include/)
#   link_directories(D:/Programme/vcpkg/installed/x64-windows/lib/)
# ENDIF(WIN32)

# #set(GLUT_INCLUDE_DIR "D:/Programme/vcpkg/installed/x64-windows/include")
# #set(SDL2_INCLUDE_DIR "D:/Programme/vcpkg/installed/x64-windows/include")
# #set(Boost_INCLUDE_DIR "D:/Programme/vcpkg/installed/x64-windows/include")
# #set(SDL2_LIBRARIES "D:/Programme/vcpkg/installed/x64-windows/debug/lib")
# #set(OPENGL_LIBRARY_DIR "D:/Programme/vcpkg/installed/x64-windows/debug/lib")
# #set(GLEW_LIBRARIES "D:/Programme/vcpkg/installed/x64-windows/debug/lib")

# # # GLUT
# # find_package(GLUT REQUIRED)
# # include_directories(${GLUT_INCLUDE_DIR})
# # if(NOT GLUT_FOUND)
# #     message(ERROR "GLUT not found!")
# # endif(NOT GLUT_FOUND)

# # # GLEW
# # find_package(GLEW REQUIRED)
# # include_directories(${GLEW_INCLUDE_DIRS})
# # link_libraries(${GLEW_LIBRARIES})
# # if(NOT GLUT_FOUND)
# #     message(ERROR "GLUT not found!")
# # endif(NOT GLUT_FOUND)

# # # OpenGL
# # find_package(OpenGL REQUIRED)
# # include_directories(${OpenGL_INCLUDE_DIRS})
# # if(NOT OPENGL_FOUND)
# #     message(ERROR "OPENGL not found!")
# # endif(NOT OPENGL_FOUND)

# set(OpenGL_GL_PRFERENCE "LEGACY")
# find_package(OpenGL REQUIRED)
# find_package(GLUT REQUIRED)
# find_package(GLEW REQUIRED)

# IF (WIN32)
#   find_package(GLFW3 REQUIRED)
#   find_package(soil CONFIG REQUIRED)
#   find_package(cereal CONFIG REQUIRED)
#   find_package(SDL2 CONFIG REQUIRED)
#   find_package(sdl2-image CONFIG REQUIRED)
#   set(Boost_USE_STATIC_LIBS ON)
#   #find_package(BOOST 1.68.0 REQUIRED COMPONENTS system filesystem serialization)
# endif (WIN32)
# if (UNIX)
#   find_package(glfw3 3.2 REQUIRED)
#   find_package(SOIL REQUIRED)
#   find_package(cereal REQUIRED)
#   find_package(SDL2 REQUIRED)
#   find_package(SDL2_image REQUIRED)
#   #find_package(BOOST 1.67 COMPONENTS system filesystem serialization REQUIRED)
# endif (UNIX)

# # link boost serialization
# #set(Boost_USE_STATIC_LIBS ON)
# #find_package(BOOST 1.68.0 REQUIRED COMPONENTS system filesystem serialization)

# # Dear ImGui
# set(IMGUI_DIR lib/imgui/)
# include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends ..)

# IF (WIN32)
#   # List of Files
#   file(GLOB SRC_MAIN "src/*.cpp")
#   #file(GLOB SRC_IMGUI "lib/imgui/*.cpp")
#   #file(GLOB SRC_IMGUI "lib/imgui/backends/*.cpp")

#   # Copy assets folder
#   file(COPY "resources/" DESTINATION "${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/resources")
#   file(COPY "src/shaders/" DESTINATION "${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/src/shaders")
# endif (WIN32)
# if (UNIX)
#   # List of Files
#   file(GLOB SRC_MAIN "./src/*.cpp")
#   file(GLOB SRC_IMGUI "./lib/imgui/*.cpp")

#   # Copy assets folder
#   file(COPY "resources/" DESTINATION "${PROJECT_BINARY_DIR}/resources")
#   file(COPY "src/shaders/" DESTINATION "${PROJECT_BINARY_DIR}/src/shaders")
# endif (UNIX)

# # Define sources and executable
# set(EXECUTABLE_NAME "JnRMaker")
# add_executable(${EXECUTABLE_NAME} ${SRC_MAIN} ${IMGUI_DIR}/backends/imgui_impl_sdl.cpp ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp ${IMGUI_DIR}/imgui.cpp ${IMGUI_DIR}/imgui_draw.cpp ${IMGUI_DIR}/imgui_demo.cpp ${IMGUI_DIR}/imgui_tables.cpp ${IMGUI_DIR}/imgui_widgets.cpp)

# IF (WIN32)
#   target_link_libraries(${EXECUTABLE_NAME} GLEW::GLEW glfw ${GLUT_glut_LIBRARY})
# #  target_link_libraries(${EXECUTABLE_NAME} ${Boost_LIBRARIES} ${Boost_serialization_LIBRARY})
#   target_link_libraries(${EXECUTABLE_NAME} soil)
#   target_link_libraries(${EXECUTABLE_NAME} SDL2::SDL2main SDL2::SDL2 SDL2::SDL2_image)
# endif (WIN32)
# if (UNIX)
#   target_link_libraries(${EXECUTABLE_NAME} ${OPENGL_LIBRARIES} GLEW::GLEW glfw ${GLUT_glut_LIBRARY})
# #  target_link_libraries(${EXECUTABLE_NAME} ${Boost_LIBRARIES} ${Boost_serialization_LIBRARY})
#   target_link_libraries(${EXECUTABLE_NAME} ${SOIL_LIBRARY})
#   target_link_libraries(${EXECUTABLE_NAME} ${SDL2_LIBRARIES} SDL2::Image)
#   target_link_libraries(${EXECUTABLE_NAME} -ldl stdc++fs)
# endif (UNIX)
# target_include_directories(${EXECUTABLE_NAME} PUBLIC ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${GLFW3_INCLUDE_DIRS} ${Boost_INCLUDE_DIR} ${SOIL_INCLUDE_DIR})

# # target_link_libraries( ${EXECUTABLE_NAME} SDL2::SDL2main SDL2::SDL2)
# # target_link_libraries( ${EXECUTABLE_NAME} SDL2::SDL2_image)

# # # target Link
# # #target_link_libraries(${EXECUTABLE_NAME} PRIVATE ImGui-SFML::ImGui-SFML)
# # target_link_libraries( ${EXECUTABLE_NAME} ${OPENGL_LIBRARIES} ${GLUT_glut_LIBRARY})
# # target_link_libraries( ${EXECUTABLE_NAME} ${Boost_LIBRARIES})
# # target_link_libraries( ${EXECUTABLE_NAME} ${Boost_serialization_LIBRARY})
# # #target_link_libraries( ${EXECUTABLE_NAME} stdc++fs glfw SOIL SDL2_image)
# # target_link_libraries( ${EXECUTABLE_NAME} ${SDL2_LIBRARY})
# # #target_link_libraries( ${EXECUTABLE_NAME} SDL2::SDL2_image)
# # target_link_libraries( ${EXECUTABLE_NAME} soil)

# # Install target
# install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)

# # CPack packaging
# include(InstallRequiredSystemLibraries)
# #set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/COPYING")
# set(CPACK_PACKAGE_VERSION_MAJOR "${JNRMAKER_VERSION_MAJOR}")
# set(CPACK_PACKAGE_VERSION_MINOR "${JNRMAKER_VERSION_MINOR}")
# include(CPack)
