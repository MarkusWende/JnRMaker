set(THIRD_PARTY_DIR "../external")
set(MAIN_SOURCE_DIR "./")
set(MAIN_INCLUDE_DIR "../include")
set(RESOURCES_DIR "../resources")
set(IMGUI_DIR ${THIRD_PARTY_DIR}/imgui)
set(STB_DIR ${THIRD_PARTY_DIR}/stb)
set(GLM_DIR ${THIRD_PARTY_DIR}/glm)
set(JSON_DIR ${THIRD_PARTY_DIR}/json)
set(RAPIDJSON_DIR ${THIRD_PARTY_DIR}/rapidjson)
set(NFD_DIR ${THIRD_PARTY_DIR}/nfd-extended)
set(CLIP_DIR ${THIRD_PARTY_DIR}/clip)

if (NOT EMSCRIPTEN)
    set(OpenGL_GL_PRFERENCE "LEGACY")
endif ()

if (WIN32)
    set(Boost_USE_STATIC_LIBS OFF )
    set(Boost_USE_MULTITHREADED ON)
    set(Boost_USE_STATIC_RUNTIME OFF)
    set(Boost_DEBUG ON)
elseif (UNIX)
    if (NOT EMSCRIPTEN)
        set(Boost_USE_STATIC_LIBS OFF)
        set(Boost_USE_STATIC_RUNTIME OFF)
    endif ()
endif ()

find_package(SDL2 REQUIRED)

if (WIN32)
    find_package(GLFW3 CONFIG REQUIRED)
    find_package(sdl2-image CONFIG REQUIRED)
elseif (UNIX)
    if (NOT EMSCRIPTEN)
        find_package(glfw3 3.2 REQUIRED)
        find_package(SDL2_image REQUIRED)
    endif ()
endif ()

if (NOT EMSCRIPTEN)
    find_package(GLEW REQUIRED)
    find_package(OpenGL REQUIRED)
    find_package(OpenGL REQUIRED)
    find_package(Boost 1.69 REQUIRED COMPONENTS system filesystem)

    if (Boost_FOUND)
        link_directories(${Boost_LIBRARY_DIRS})
        include_directories(${Boost_INCLUDE_DIRS})
    endif (Boost_FOUND)
endif ()

if (NOT EMSCRIPTEN)
    add_subdirectory(${NFD_DIR} ${PROJECT_BINARY_DIR}/nfd-extended.dir)
endif ()
add_subdirectory(${GLM_DIR} ${PROJECT_BINARY_DIR}/glm.dir)
add_subdirectory(${THIRD_PARTY_DIR}/googletest ${PROJECT_BINARY_DIR}/googletest.dir)
add_subdirectory(${CLIP_DIR} ${PROJECT_BINARY_DIR}/clip.dir)

include_directories(${MAIN_INCLUDE_DIR})
include_directories(${MAIN_INCLUDE_DIR}/utils)
include_directories(${MAIN_INCLUDE_DIR}/utils/logging)
include_directories(${MAIN_INCLUDE_DIR}/utils/manager)
include_directories(${MAIN_INCLUDE_DIR}/entities)
include_directories(${MAIN_INCLUDE_DIR}/graphics)
include_directories(${MAIN_INCLUDE_DIR}/ui)
include_directories(${RAPIDJSON_DIR}/include)
include_directories(${JSON_DIR}/single_include)
include_directories(${GLM_DIR})
include_directories(${STB_DIR})
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends ..)
if (NOT EMSCRIPTEN)
    include_directories(${NFD_DIR}/src/include)
endif ()
include_directories(${CLIP_DIR})

if (NOT EMSCRIPTEN)
    include_directories(${OPENGL_INCLUDE_DIR} ${GLFW3_INCLUDE_DIRS} ${GLEW_INCLUDE_DIR})
endif ()

add_definitions(-DIMGUI_USER_CONFIG="${PROJECT_SOURCE_DIR}/${MAIN_INCLUDE_DIR}/ui/ImConfigJnRMaker.h")

if (WIN32)
    add_definitions( -DBOOST_ALL_NO_LIB )
    add_definitions( -DBOOST_ALL_DYN_LINK )
endif ()

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

add_library(
    app
    ${MAIN_SOURCE_DIR}/entities/EntityWorldLayer.cpp
    ${MAIN_SOURCE_DIR}/entities/EntitySolid.cpp
    ${MAIN_SOURCE_DIR}/graphics/Framebuffer.cpp
    ${MAIN_SOURCE_DIR}/graphics/Primitive.cpp
    ${MAIN_SOURCE_DIR}/graphics/PrimitiveCube.cpp
    ${MAIN_SOURCE_DIR}/graphics/PrimitiveLine.cpp
    ${MAIN_SOURCE_DIR}/graphics/PrimitiveQuad.cpp
    ${MAIN_SOURCE_DIR}/graphics/Scene.cpp
    ${MAIN_SOURCE_DIR}/graphics/Shader.cpp
    ${MAIN_SOURCE_DIR}/graphics/SolidCS.cpp
    ${MAIN_SOURCE_DIR}/graphics/SolidGrid.cpp
    ${MAIN_SOURCE_DIR}/graphics/SolidSprite.cpp
    ${MAIN_SOURCE_DIR}/graphics/Texture.cpp
    ${MAIN_SOURCE_DIR}/graphics/Tilemap.cpp
    ${MAIN_SOURCE_DIR}/graphics/AppWindow.cpp
    ${MAIN_SOURCE_DIR}/ui/Gui.cpp
    ${MAIN_SOURCE_DIR}/utils/logging/UILogger.cpp
    ${MAIN_SOURCE_DIR}/utils/manager/ShaderManager.cpp
    ${MAIN_SOURCE_DIR}/utils/Injector.cpp
    ${MAIN_SOURCE_DIR}/utils/MessageManager.cpp
    ${MAIN_SOURCE_DIR}/utils/ProjectManager.cpp
    ${MAIN_SOURCE_DIR}/utils/ResourceManagerOld.cpp
    ${MAIN_SOURCE_DIR}/utils/TilemapManager.cpp
)

# The 'jnrmaker' executable
add_executable(
    jnrmaker
    ${MAIN_SOURCE_DIR}/Main.cpp
)

if (EMSCRIPTEN)
    file(GLOB libraryJsFiles ${RESOURCES_DIR}/www/js/*.js)

    if (CMAKE_BUILD_TYPE STREQUAL Debug)
        SET(linkFlags "-g3 -s NO_EXIT_RUNTIME=1 -sASSERTIONS=1")
    else() # Either MinSizeRel, RelWithDebInfo or Release, all which run with optimizations enabled.
        SET(linkFlags "-O2 -s NO_EXIT_RUNTIME=1")
    endif()

    # Ensure synchronous startup for this test, whose output expects it
    SET(linkFlags "${linkFlags} -sWASM=1 -sUSE_PTHREADS=1 -sPTHREAD_POOL_SIZE=4")
    SET(linkFlags "${linkFlags} -sUSE_WEBGL2=1 -sMIN_WEBGL_VERSION=2 -sMAX_WEBGL_VERSION=2 -sFULL_ES3=1")
    SET(linkFlags "${linkFlags} -sUSE_SDL=2 -sUSE_SDL_IMAGE=2 -sUSE_BOOST_HEADERS=1")
    SET(linkFlags "${linkFlags} -sFETCH -sEXPORTED_FUNCTIONS=[\"_getJSON\",\"_main\",\"_getLocalTilemapFile\",\"_saveJSONFile\",\"_viewFullscreen\"] -sEXPORTED_RUNTIME_METHODS=ccall,cwrap")
    SET(linkFlags "${linkFlags} -sINITIAL_MEMORY=300MB -lidbfs.js -lembind --no-heap-copy")
    SET(linkFlags "${linkFlags} --preload-file ../../../resources@/resources")

    target_link_libraries(jnrmaker app imgui ${SDL2_LIBRARIES})

    # These properties specify what kind of Emscripten build to perform and are assigned to our 'jnrmaker' executable target.
    set_target_properties(jnrmaker PROPERTIES LINK_FLAGS "${linkFlags}")
    #set_target_properties(jnrmaker PROPERTIES LINK_FLAGS "-g -sASSERTIONS=1 -Os -sUSE_PTHREADS=1 -sPTHREAD_POOL_SIZE=2 -sUSE_WEBGL2=1 -s MIN_WEBGL_VERSION=2 -s MAX_WEBGL_VERSION=2 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_BOOST_HEADERS=1 -s INITIAL_MEMORY=300MB -s EXPORTED_RUNTIME_METHODS=lengthBytesUTF8,stringToUTF8 -s FETCH -lidbfs.js -lembind -sASYNCIFY --no-heap-copy --preload-file ../../data@/data --preload-file ../../src/shaders@/src/shaders")


    message(STATUS "js libs '${libraryJsFiles}'")
    # To link .js files using the --js-library flag, use the following helper function.
    em_link_js_library(jnrmaker ${libraryJsFiles})

    add_custom_command(
        TARGET jnrmaker
        POST_BUILD
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMAND ./cmake-post-build.sh
    )
elseif (WIN32)
    file(COPY "../resources/" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}/resources")
    target_link_libraries(jnrmaker app imgui bcrypt nfd clip GLEW::GLEW glfw SDL2::SDL2main SDL2::SDL2 SDL2::SDL2_image ${OPENGL_LIBRARIES} ${Boost_LIBRARIES})
    install(TARGETS jnrmaker DESTINATION bin)
else ()
    file(COPY "../resources/" DESTINATION "${PROJECT_BINARY_DIR}/resources")
    target_link_libraries(jnrmaker app imgui nfd clip GLEW::GLEW glfw ${SDL2_LIBRARIES} SDL2::Image ${OPENGL_LIBRARIES} -ldl stdc++fs)
endif ()