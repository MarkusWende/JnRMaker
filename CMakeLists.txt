# cmake_minimum_required(VERSION 3.6)
# project (JnRMaker)

# find_package(OpenGL REQUIRED)
# find_package(GLEW REQUIRED)
# find_package(GLFW REQUIRED)
# find_package(SOIL REQUIRED)

# # List of Files
# file(GLOB SRC_MAIN "${PROJECT_BINARY_DIR}/../src/*.cpp")
# file(GLOB SRC_IMGUI "${PROJECT_BINARY_DIR}/../lib/imgui_old/*.cpp")

# # Copy assets folder
# file(COPY "./resources/assets" DESTINATION ${PROJECT_BINARY_DIR})

# # Define sources and executable
# set(EXECUTABLE_NAME "JnRMaker")
# add_executable(${EXECUTABLE_NAME} ${SRC_IMGUI} ${SRC_MAIN})

# set(SOURCE_FILES main.cpp Shader.h Shader.cpp)
# add_executable(my_opengl_executable ${SOURCE_FILES})

# target_link_libraries(my_opengl_executable ${OPENGL_LIBRARIES} GLEW::GLEW ${GLFW_LIBRARY} ${SOIL_LIBRARY})
# target_include_directories(my_opengl_executable PUBLIC ${OPENGL_INCLUDE_DIR} ${GLFW_INCLUDE_DIR} ${SOIL_INCLUDE_DIR})

# set_target_properties(my_opengl_executable PROPERTIES
#     CXX_STANDARD 17
#     CXX_STANDARD_REQUIRED ON
# )





#Change this if you need to target a specific CMake version
cmake_minimum_required (VERSION 3.6)
set(CMAKE_BUILD_TYPE Debug)

# Enable debug symbols by default
# must be done before project() statement
IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)
# (you can also set it on the command line: -D CMAKE_BUILD_TYPE=Release)

project (JnRMaker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set version information in a config.h file
set(JNRMAKER_VERSION_MAJOR 0)
set(JNRMAKER_VERSION_MINOR 1)
#configure_file("./include/config.h.in" "${PROJECT_BINARY_DIR}/include/config.h")
include_directories("${PROJECT_BINARY_DIR}")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")


IF (WIN32)
    include_directories(
        D:/Programme/vcpkg/installed/x64-windows/include/
        )

    link_directories(
        D:/Programme/vcpkg/installed/x64-windows/lib/
        )
ENDIF()

#set(GLUT_INCLUDE_DIR "D:/Programme/vcpkg/installed/x64-windows/include")
#set(SDL2_INCLUDE_DIR "D:/Programme/vcpkg/installed/x64-windows/include")
#set(Boost_INCLUDE_DIR "D:/Programme/vcpkg/installed/x64-windows/include")
#set(SDL2_LIBRARIES "D:/Programme/vcpkg/installed/x64-windows/debug/lib")
#set(OPENGL_LIBRARY_DIR "D:/Programme/vcpkg/installed/x64-windows/debug/lib")
#set(GLEW_LIBRARIES "D:/Programme/vcpkg/installed/x64-windows/debug/lib")

# # GLUT
# find_package(GLUT REQUIRED)
# include_directories(${GLUT_INCLUDE_DIR})
# if(NOT GLUT_FOUND)
#     message(ERROR "GLUT not found!")
# endif(NOT GLUT_FOUND)

# # GLEW
# find_package(GLEW REQUIRED)
# include_directories(${GLEW_INCLUDE_DIRS})
# link_libraries(${GLEW_LIBRARIES})
# if(NOT GLUT_FOUND)
#     message(ERROR "GLUT not found!")
# endif(NOT GLUT_FOUND)

# # OpenGL
# find_package(OpenGL REQUIRED)
# include_directories(${OpenGL_INCLUDE_DIRS})
# if(NOT OPENGL_FOUND)
#     message(ERROR "OPENGL not found!")
# endif(NOT OPENGL_FOUND)

set(OpenGL_GL_PRFERENCE "LEGACY")
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)

IF (WIN32)
  find_package(GLFW3 REQUIRED)
  find_package(soil CONFIG REQUIRED)
  find_package(cereal CONFIG REQUIRED)
  find_package(SDL2 CONFIG REQUIRED)
  find_package(sdl2-image CONFIG REQUIRED)
  set(Boost_USE_STATIC_LIBS ON)
  #find_package(BOOST 1.68.0 REQUIRED COMPONENTS system filesystem serialization)
elseif (UNIX)
  find_package(glfw3 3.2 REQUIRED)
  find_package(SOIL REQUIRED)
  find_package(cereal REQUIRED)
  find_package(SDL2 REQUIRED)
  find_package(SDL2_image REQUIRED)
  #find_package(BOOST 1.67 COMPONENTS system filesystem serialization REQUIRED)
endif (UNIX)

# link boost serialization
#set(Boost_USE_STATIC_LIBS ON)
#find_package(BOOST 1.68.0 REQUIRED COMPONENTS system filesystem serialization)

IF (WIN32)
  # List of Files
  file(GLOB SRC_MAIN "${PROJECT_BINARY_DIR}/../src/*.cpp")
  file(GLOB SRC_IMGUI "${PROJECT_BINARY_DIR}/../lib/imgui/*.cpp")

  # Copy assets folder
  file(COPY "./resources/assets" DESTINATION ${PROJECT_BINARY_DIR})
elseif (UNIX)
  # List of Files
  file(GLOB SRC_MAIN "./src/*.cpp")
  file(GLOB SRC_IMGUI "./lib/imgui/*.cpp")

  # Copy assets folder
  file(COPY "./resources/assets" DESTINATION ${PROJECT_BINARY_DIR})
endif (UNIX)

# Define sources and executable
set(EXECUTABLE_NAME "JnRMaker")
add_executable(${EXECUTABLE_NAME} ${SRC_IMGUI} ${SRC_MAIN})

IF (WIN32)
  target_link_libraries(${EXECUTABLE_NAME} GLEW::GLEW glfw ${GLUT_glut_LIBRARY})
#  target_link_libraries(${EXECUTABLE_NAME} ${Boost_LIBRARIES} ${Boost_serialization_LIBRARY})
  target_link_libraries(${EXECUTABLE_NAME} soil)
  target_link_libraries(${EXECUTABLE_NAME} SDL2::SDL2main SDL2::SDL2 SDL2::SDL2_image)
elseif (UNIX)
  target_link_libraries(${EXECUTABLE_NAME} ${OPENGL_LIBRARIES} GLEW::GLEW glfw ${GLUT_glut_LIBRARY})
#  target_link_libraries(${EXECUTABLE_NAME} ${Boost_LIBRARIES} ${Boost_serialization_LIBRARY})
  target_link_libraries(${EXECUTABLE_NAME} ${SOIL_LIBRARY})
  target_link_libraries(${EXECUTABLE_NAME} ${SDL2_LIBRARIES} SDL2::Image)
  target_link_libraries(${EXECUTABLE_NAME} -ldl stdc++fs)
endif (UNIX)
target_include_directories(${EXECUTABLE_NAME} PUBLIC ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${GLFW3_INCLUDE_DIRS} ${Boost_INCLUDE_DIR} ${SOIL_INCLUDE_DIR})

# target_link_libraries( ${EXECUTABLE_NAME} SDL2::SDL2main SDL2::SDL2)
# target_link_libraries( ${EXECUTABLE_NAME} SDL2::SDL2_image)

# # target Link
# #target_link_libraries(${EXECUTABLE_NAME} PRIVATE ImGui-SFML::ImGui-SFML)
# target_link_libraries( ${EXECUTABLE_NAME} ${OPENGL_LIBRARIES} ${GLUT_glut_LIBRARY})
# target_link_libraries( ${EXECUTABLE_NAME} ${Boost_LIBRARIES})
# target_link_libraries( ${EXECUTABLE_NAME} ${Boost_serialization_LIBRARY})
# #target_link_libraries( ${EXECUTABLE_NAME} stdc++fs glfw SOIL SDL2_image)
# target_link_libraries( ${EXECUTABLE_NAME} ${SDL2_LIBRARY})
# #target_link_libraries( ${EXECUTABLE_NAME} SDL2::SDL2_image)
# target_link_libraries( ${EXECUTABLE_NAME} soil)

# Install target
install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)

# CPack packaging
include(InstallRequiredSystemLibraries)
#set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/COPYING")
set(CPACK_PACKAGE_VERSION_MAJOR "${JNRMAKER_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${JNRMAKER_VERSION_MINOR}")
include(CPack)
