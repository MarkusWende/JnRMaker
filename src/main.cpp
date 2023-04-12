//#define IMGUI_USER_CONFIG "config.h"

//#include "config.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>

#include "gui.h"
#include "message_manager.h"
#include "project_manager.h"
#include "resource_manager.h"
#include "scene.h"
#include "events.h"
#include "window.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/fetch.h>
#endif


// #include <GLFW/glfw3.h>
// #include <iostream>

// #include "../include/resource_manager.h"

#include <SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif

#include <GL/glew.h>    // Initialize with glewInit()

// Emscripten requires to have full control over the main loop. We're going to store our SDL book-keeping variables globally.
// Having a single function that acts as a loop prevents us to store state in the stack of said function. So we need some location for this.
SDL_Window*     g_Window = NULL;
SDL_GLContext   g_GLContext = NULL;

// Gui
std::shared_ptr<Gui> appGui;
std::shared_ptr<Scene> appScene;
//Gui* appGui;
//Scene* appScene;

// For clarity, our main loop code is declared at the end.
static void main_loop(void*);

void GLAPIENTRY
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
    (void)userParam;
    (void)length;
    (void)id;
    std::stringstream msg;
    msg << source << "\tOpenGL: " << " type = 0x" << type << ", severity = 0x" << severity << ", message = " << message;
    if (type == GL_DEBUG_TYPE_ERROR)
    {
        MessageManager::AddMessage(msg, message_t::INFO);
    }
}

GLenum glCheckError_(const char *file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::stringstream msg;
        msg << error << " | " << file << " (" << line << ")";
        MessageManager::AddMessage(msg, message_t::INFO );
    }
    return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

int main(int, char**)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
	{
		printf("Error: %s\n", SDL_GetError());
		return -1;
	}

    // Decide GL+GLSL versions
    const char* glsl_version = "#version 100";
    #if __APPLE__
        // GL 4.1 + GLSL 410
        glsl_version = "#version 330";
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    #elif __EMSCRIPTEN__
        // For the browser using Emscripten, we are going to use WebGL1 with GL ES2. See the Makefile. for requirement details.
        // It is very likely the generated file won't work in many browsers. Firefox is the only sure bet, but I have successfully
        // run this code on Chrome for Android for example.
        glsl_version = "#version 300 es";
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    #else
        // GL 4.1 + GLSL 410
        glsl_version = "#version 330";
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    #endif

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_DisplayMode current;
    SDL_GetCurrentDisplayMode(0, &current);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    g_Window = SDL_CreateWindow("JnRMaker", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
    g_GLContext = SDL_GL_CreateContext(g_Window);

    if (!g_GLContext)
    {
        fprintf(stderr, "Failed to initialize WebGL context!\n");
        return 1;
    }
    SDL_GL_SetSwapInterval(1); // Enable vsync

    // Initialize OpenGL loader
    // glewExperimental = GL_TRUE;
    bool err = glewInit() != GLEW_OK;
    std::stringstream msg;

    //bool err = false;
    if (err)
    {
#ifdef _WIN32
        FILE* stream;
        freopen_s(&stream, "log.txt", "w", stdout);
            if (stream) {
            fprintf(stream, "%s\tFailed to initialize OpenGL loader! Error: %s\n", TimeHelper::GetTimeinfo().c_str(), glewGetErrorString(err));
            fclose(stream);
            }
#endif // _WIN32
#ifdef __linux__
        FILE* stream;
        stream = fopen("./log.txt", "w");
        fprintf(stream, "%s\tFailed to initialize OpenGL loader! Error: %s\n", TimeHelper::GetTimeinfo().c_str(), glewGetErrorString(err));
        fclose(stream);
#endif // __linux__
        return 1;
    }
    else
    {
        const GLubyte* renderer = glGetString(GL_RENDERER);
        const GLubyte* vendor = glGetString(GL_VENDOR);
        const GLubyte* version = glGetString(GL_VERSION);
        const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

        if (glslVersion == NULL)
        {
#ifdef _WIN32
            FILE* stream;
            freopen_s(&stream, "log.txt", "w", stdout);
                if (stream)
                {
                    fprintf(stream, "%s\tOpenGL GLSL Version is Null.. Aborting\n", TimeHelper::GetTimeinfo().c_str());
                    fclose(stream);
                }
#endif // _WIN32
#ifdef __linux__
            FILE* stream;
            stream = fopen("./log.txt", "w");
            fprintf(stream, "%s\tOpenGL GLSL Version is Null.. Aborting\n", TimeHelper::GetTimeinfo().c_str());
            fclose(stream);
#endif // __linux__
            return 1;
        }
        

        GLint major, minor, depthBufferBits;
        glGetIntegerv(GL_MAJOR_VERSION, &major);
        glGetIntegerv(GL_MINOR_VERSION, &minor);
        glGetIntegerv(GL_DEPTH_BITS, &depthBufferBits );

        msg << "Successful initialized OpenGL..." << std::endl;
        msg << "\t\t\tGL Vendor:\t\t\t\t" << vendor << std::endl;
        msg << "\t\t\tGL Renderer:\t\t\t\t" << renderer << std::endl;
        msg << "\t\t\tGL Version (string):\t" << version << std::endl;
        msg << "\t\t\tGL Version (integer):\t" << major << "." << minor << std::endl;
        msg << "\t\t\tGLSL Version\t\t\t\t" << glslVersion << std::endl;
        msg << "\t\t\tDepth Buffer bits:\t\t" << depthBufferBits;
        MessageManager::AddMessage(msg, message_t::DEBUG);
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#ifndef __EMSCRIPTEN__
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_MULTISAMPLE); // Enabled by default on some drivers, but not all so always enable to make sure
    glDebugMessageCallback( MessageCallback, 0 );
#endif

#ifdef __EMSCRIPTEN_PTHREADS__
    {
        msg << "__EMSCRIPTEN_PTHREADS__ is enabled.";
        MessageManager::AddMessage(msg, message_t::DEBUG);
    }
#endif

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    //ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;

    // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
    // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
    io.IniFilename = NULL;

    // Setup Dear ImGui style
    //ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(g_Window, g_GLContext);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Gui
    //appGui = new Gui();
    appGui = std::make_shared<Gui>();

    // Scene
    appScene = std::make_shared<Scene>(1280, 720);

#ifdef __EMSCRIPTEN__
    // This function call won't return, and will engage in an infinite loop, processing events from the browser, and dispatching them.
    emscripten_set_main_loop_arg(main_loop, NULL, 60, true);
#else
    //ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    while (appGui->IsOpen())
    {
        main_loop(NULL);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    //ImPlot::DestroyContext();

    SDL_GL_DeleteContext(g_GLContext);
    SDL_DestroyWindow(g_Window);
    SDL_Quit();

    return 0;
#endif
}

static void main_loop(void* arg)
{
    ImGuiIO& io = ImGui::GetIO();
    IM_UNUSED(arg); // We can pass this argument as the second parameter of emscripten_set_main_loop_arg(), but we don't use that.

    // Our state (make them static = more or less global) as a convenience to keep the example terse.
    static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
    // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT)
            appGui->Close();
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(g_Window))
            appGui->Close();
    }

    if (ProjectManager::GetStatus() == project_status_t::SAVE)
    {
        std::stringstream msg;
        msg << "Saving..";
        MessageManager::AddMessage(msg, message_t::INFO);
        ProjectManager::SaveCreate();
    //     // create and open a character archive for output
    //     std::ofstream ofs(ProjectManager::GetName() + ".jrm");

    //     // save data to archive
        // cereal::XMLOutputArchive ar(msg, cereal::XMLOutputArchive::Options(6, true, false));
//         cereal::JSONOutputArchive ar(msg);
// //         //cereal::BinaryOutputArchive ar(ofs);
//         ar( CEREAL_NVP(appGui) );
//         ar( CEREAL_NVP(appScene) );
        appScene->save();
//         //ar( cereal::make_nvp("window_with_", oldWidth), cereal::make_nvp("window_height_", oldHeight) );

        // long double c = pow(0.5, 1000);
        // ar ( cereal::make_nvp("a_very_small_value_", c) );

        //ProjectManager::AddSaveFile(msg);
        ProjectManager::SaveWrite();
        ProjectManager::SetStatus(project_status_t::IDLE);
    }
    else if (ProjectManager::GetStatus() == project_status_t::LOAD)
    {
        std::stringstream msg;
        msg << "Loading..";
        MessageManager::AddMessage(msg, message_t::INFO);
        //std::ifstream is(ProjectManager::GetName() + ".jrm");
        // cereal::XMLInputArchive ar(msg);
    //     //cereal::JSONInputArchive ar(is);
    //     //cereal::BinaryInputArchive ar(is);

        // GLuint width = 12;
        // GLuint height = 4;

    //     ar( appGui );
    //     ar( appScene );
        // ar( width, height);

    //     //window->setSize({ width, height });

        MessageManager::AddMessage(msg, message_t::INFO);

        ProjectManager::SetStatus(project_status_t::IDLE);
    }

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    // Update and render gui
    int width, height;
    SDL_GetWindowSize(g_Window, &width, &height);
    appGui->WindowUpdate(appScene, width, height);
    processEvents(appScene, appGui);
    // appGui->Render(appScene);
    appGui->Draw(appScene);
    //appGui->ShowBackendCheckerWindow();

    appScene->Render();

    //ImGui::ShowDemoWindow();
    //ImPlot::ShowDemoWindow();

    // Rendering
    ImGui::Render();
    SDL_GL_MakeCurrent(g_Window, g_GLContext);
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

#ifdef WIN32
    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call SDL_GL_MakeCurrent(window, gl_context) directly)
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
        SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
    }
#endif

    SDL_GL_SwapWindow(g_Window);
}







// /**
//  * JnRMaker - A tile editor and jump and run game maker
//  * See COPYRIGHT file at the top of the source tree.
//  *
//  * This program is free software: you can redistribute it and/or modify
//  * it under the terms of the GNU General Public License as published by
//  * the Free Software Foundation, either version 3 of the License, or
//  * (at your option) any later version.
//  *
//  * This program is distributed in the hope that it will be useful,
//  * but WITHOUT ANY WARRANTY; without even the implied warranty of
//  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//  * GNU General Public License for more details.
//  *
//  * You should have received a copy of the JnRMaker License Statement and
//  * the GNU General Public License along with this program.
//  *
//  */

// /**
//  * @file main.cpp
//  * @brief Root application file. Application window initialization and main application loop.
//  *
//  * @author Markus Wende
//  * https://github.com/MarkusWende
// **/

// #include "../include/imconfig.h"
// #include "imgui.h"
// #include "imgui_impl_sdl.h"
// #include "imgui_impl_opengl3.h"
// #include <SDL2/SDL.h>

// //#define IMGUI_IMPL_OPENGL_LOADER_GLEW

// // About OpenGL function loaders: modern OpenGL doesn't have a standard header file and requires individual function pointers to be loaded manually.
// // Helper libraries are often used for this purpose! Here we are supporting a few common ones: gl3w, glew, glad.
// // You may use another loader/header of your choice (glext, glLoadGen, etc.), or chose to manually implement your own.
// #if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
// #include <GL/gl3w.h>    // Initialize with gl3wInit()
// #elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
// #include <GL/glew.h>    // Initialize with glewInit()
// #elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
// #include <glad/glad.h>  // Initialize with gladLoadGL()
// #else
// #include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
// #endif

// #include "../include/project_manager.h"
// #include "../include/message_manager.h"
// #include "../include/scene.h"
// #include "../include/events.h"
// #include "../include/gui.h"
// #include "../include/window.h"


// #include <GLFW/glfw3.h>
// #include <iostream>

// #include "../include/resource_manager.h"
// //#include <SFML/Window/Mouse.hpp>

// // The Width of the screen
// const GLuint SCREEN_WIDTH = 1800;
// // The height of the screen
// const GLuint SCREEN_HEIGHT = 1000;

// int main(int argc, char* argv[])
// {
//     //glEnable(GL_BLEND);
//     //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//     JNRWindow* window = new JNRWindow();
//     //sf::RenderWindow* window = appWindow.Get();

//     if (window->SetupSDL() == -1) {
//         return -1;
//     }


//     //glfwInit();
//     //glewInit();

//     glEnable(GL_MULTISAMPLE); // Enabled by default on some drivers, but not all so always enable to make sure

//     //window->ShowLoadingWindow();
//     window->CreateNewWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "JnRMaker");


//     // Initialize OpenGL loader
// #if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
//     bool err = gl3wInit() != 0;
// #elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
//     glewExperimental = GL_TRUE;
//     bool err = glewInit() != GLEW_OK;
// #elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
//     bool err = gladLoadGL() == 0;
// #else
//     bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of initialization.
// #endif

// //bool err = false;
// if (err)
// {
//     FILE* stream;
// #ifdef _WIN32
//     freopen_s(&stream, "log.txt", "w", stdout);
//         if (stream) {
//         fprintf(stream, "%s\tFailed to initialize OpenGL loader! Error: %s\n", time_helper::GetTimeinfo().c_str(), glewGetErrorString(err));
//         fclose(stream);
//         }
// #endif // _WIN32
// #ifdef __linux__
//     stream = fopen("./log.txt", "w");
//     fprintf(stream, "%s\tFailed to initialize OpenGL loader! Error: %s\n", time_helper::GetTimeinfo().c_str(), glewGetErrorString(err));
//     fclose(stream);
// #endif // __linux__
//     return 1;
// }
// else
// {
//     const GLubyte* renderer = glGetString(GL_RENDERER);
//     const GLubyte* vendor = glGetString(GL_VENDOR);
//     const GLubyte* version = glGetString(GL_VERSION);
//     const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

//     GLint major, minor;
//     glGetIntegerv(GL_MAJOR_VERSION, &major);
//     glGetIntegerv(GL_MINOR_VERSION, &minor);

//     FILE* stream;

// #ifdef _WIN32
//     freopen_s(&stream, "log.txt", "w", stdout);

//     if (stream)
//     {
//         fprintf(stream, "%s\tSuccessful initialized OpenGL...\n", time_helper::GetTimeinfo().c_str());
//         fprintf(stream, "\t\t\t\t\tGL Vendor\t\t: %s\n", vendor);
//         fprintf(stream, "\t\t\t\t\tGL Renderer\t\t: %s\n", renderer);
//         fprintf(stream, "\t\t\t\t\tGL Version (string)\t: %s\n", version);
//         fprintf(stream, "\t\t\t\t\tGL Version (integer)\t: %d.%d\n", major, minor);
//         fprintf(stream, "\t\t\t\t\tGLSL Version\t\t: %s\n", glslVersion);
//         fclose(stream);
//     }
// #endif // _WIN32
// #ifdef __linux__
//     stream = fopen("./log.txt", "w");

//     if (stream)
//     {
//         fprintf(stream, "%s\tSuccessful initialized OpenGL...\n", time_helper::GetTimeinfo().c_str());
//         fprintf(stream, "\t\t\t\t\tGL Vendor\t\t: %s\n", vendor);
//         fprintf(stream, "\t\t\t\t\tGL Renderer\t\t: %s\n", renderer);
//         fprintf(stream, "\t\t\t\t\tGL Version (string)\t: %s\n", version);
//         fprintf(stream, "\t\t\t\t\tGL Version (integer)\t: %d.%d\n", major, minor);
//         fprintf(stream, "\t\t\t\t\tGLSL Version\t\t: %s\n", glslVersion);
//         fclose(stream);
//     }
// #endif // __linux__
// }

//     glEnable(GL_BLEND);
//     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//     // Setup Dear ImGui context
//     IMGUI_CHECKVERSION();
//     ImGui::CreateContext();
//     ImGuiIO& io = ImGui::GetIO(); (void)io;
//     io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
//     //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
//     io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
//     io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
//     //io.ConfigViewportsNoAutoMerge = true;
//     //io.ConfigViewportsNoTaskBarIcon = true;

//     // Setup Dear ImGui style
//     //ImGui::StyleColorsDark();
//     //ImGui::StyleColorsClassic();

//     // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
//     ImGuiStyle& style = ImGui::GetStyle();
//     if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
//     {
//         style.WindowRounding = 0.0f;
//         style.Colors[ImGuiCol_WindowBg].w = 1.0f;
//     }


//     // Create gui object
//     Gui appGui;
//     // Create default scene object

//     Scene appScene(1200, 800);
//     // Update Gui to display the windows
//     //appGui.WindowUpdate(appScene, window->getSize().x, window->getSize().y);
//     //appGui.WindowUpdate(window->GetWidth(), window->GetHeight());

//     //sf::Clock deltaClock;

//     // Always save the window size each loop cycle
//     //GLuint oldWidth = window->getSize().x;
//     //GLuint oldHeight = window->getSize().y;

//     ProjectManager::SetStatus(project_status_t::IDLE);
//     std::string projectName = "TestProject";
//     ProjectManager::SetName(projectName);

//     // Setup Platform/Renderer bindings
//     ImGui_ImplSDL2_InitForOpenGL(window->GetWindow(), window->GetGLContext());
//     ImGui_ImplOpenGL3_Init(window->GetGLSLVersion());

//     // Main Loop
//     while (appGui.IsOpen())
//     {
//         //processEvents(window->GetWindow(), appScene, appGui);
//         SDL_Event event;

//         while (SDL_PollEvent(&event))
//         {
//             ImGui_ImplSDL2_ProcessEvent(&event);
//             //ImGui::SFML::ProcessEvent(event);

//             if (event.type == SDL_QUIT)
//                 appGui.Close();
//             if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window->GetWindow()))
//                 appGui.Close();
//         }
//         // Start the Dear ImGui frame
//         ImGui_ImplOpenGL3_NewFrame();
//         //ImGui_ImplSDL2_NewFrame(window->GetWindow());
//         ImGui_ImplSDL2_NewFrame();
//         ImGui::NewFrame();

//         // Update and render gui
//         appGui.WindowUpdate(window->GetWidth(), window->GetHeight());
//         processEvents(appScene, appGui);
//         appGui.Render(appScene);

//         appScene.Render();

//         // If the project name changes, update the window title
//         /*if (myGui.project_name_changed)
//         {
//             SDL_SetWindowTitle(window, ("[" + myGui.project_name + "] - SoundIMP").c_str());
//             myGui.project_name_changed = false;
//         }*/

//         // Render ImGui to show the gui
//         ImGui::Render();

//         // Set the background color
//         ImGuiStyle* style = &ImGui::GetStyle();
//         ImColor col = style->Colors[ImGuiCol_PopupBg];
//         glClearColor(col.Value.x, col.Value.y, col.Value.z, col.Value.w);
//         glClear(GL_COLOR_BUFFER_BIT);
//         glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound
//         ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

//         if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
//         {
//             SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
//             SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
//             ImGui::UpdatePlatformWindows();
//             ImGui::RenderPlatformWindowsDefault();
//             SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
//         }

//         // render end
//         SDL_GL_SwapWindow(window->GetWindow());

//         // update
//         //glfwPollEvents();


//         // if (ProjectManager::GetStatus() == project_status_t::SAVE)
//         // {
//         //     // create and open a character archive for output
//         //     std::ofstream ofs(ProjectManager::GetName() + ".jrm");

//         //     // save data to archive
//         //     {
//         //         cereal::XMLOutputArchive ar(ofs, cereal::XMLOutputArchive::Options(6, true, false));
//         //         //cereal::JSONOutputArchive ar(ofs);
//         //         //cereal::BinaryOutputArchive ar(ofs);
//         //         ar( CEREAL_NVP(appGui) );
//         //         ar( CEREAL_NVP(appScene) );
//         //         //ar( cereal::make_nvp("window_with_", oldWidth), cereal::make_nvp("window_height_", oldHeight) );

//         //         long double c = pow(0.5, 1000);
//         //         ar ( cereal::make_nvp("a_very_small_value_", c) );
//         //     }

//         //     ProjectManager::SetStatus(project_status_t::IDLE);
//         // }
//         // else if (ProjectManager::GetStatus() == project_status_t::LOAD)
//         // {
//         //     std::ifstream is(ProjectManager::GetName() + ".jrm");
//         //     cereal::XMLInputArchive ar(is);
//         //     //cereal::JSONInputArchive ar(is);
//         //     //cereal::BinaryInputArchive ar(is);

//         //     GLuint width;
//         //     GLuint height;

//         //     ar( appGui );
//         //     ar( appScene );
//         //     //ar( width, height);

//         //     //window->setSize({ width, height });

//         //     ProjectManager::SetStatus(project_status_t::IDLE);
//         // }
//     }

//     // Cleanup
//     ImGui_ImplOpenGL3_Shutdown();
//     ImGui_ImplSDL2_Shutdown();
//     ImGui::DestroyContext();

//     // Close SDL
//     window->CleanUp();

//     return 0;
// }
