/**
 * JnRMaker - A tile editor and jump and run game maker
 * See COPYRIGHT file at the top of the source tree.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the JnRMaker License Statement and
 * the GNU General Public License along with this program.
 *
 */

/**
 * @file main.cpp
 * @brief Root application file. Application window initialization and main application loop.
 *
 * @author Markus Wende
 * https://github.com/MarkusWende
**/

#include "../lib/imgui/imgui.h"
#include "../lib/imgui/imgui_impl_sdl.h"
#include "../lib/imgui/imgui_impl_opengl3.h"
#include <SDL2/SDL.h>

#define IMGUI_IMPL_OPENGL_LOADER_GLEW

// About OpenGL function loaders: modern OpenGL doesn't have a standard header file and requires individual function pointers to be loaded manually.
// Helper libraries are often used for this purpose! Here we are supporting a few common ones: gl3w, glew, glad.
// You may use another loader/header of your choice (glext, glLoadGen, etc.), or chose to manually implement your own.
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>    // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>    // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>  // Initialize with gladLoadGL()
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif

#include "../include/project_manager.h"
#include "../include/message_manager.h"
#include "../include/scene.h"
#include "../include/events.h"
#include "../include/gui.h"
#include "../include/window.h"

#include <GLFW/glfw3.h>
#include <iostream>

#include "../include/resource_manager.h"
//#include <SFML/Window/Mouse.hpp>

// The Width of the screen
const GLuint SCREEN_WIDTH = 1800;
// The height of the screen
const GLuint SCREEN_HEIGHT = 1000;

int main(int argc, char* argv[])
{
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    JNRWindow* window = new JNRWindow();
    //sf::RenderWindow* window = appWindow.Get();

    if (window->SetupSDL() == -1) {
        return -1;
    }


    //glfwInit();
    //glewInit();

    glEnable(GL_MULTISAMPLE); // Enabled by default on some drivers, but not all so always enable to make sure

    //window->ShowLoadingWindow();
    window->CreateNewWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "JnRMaker");


    // Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
    bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
    glewExperimental = GL_TRUE;
    bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
    bool err = gladLoadGL() == 0;
#else
    bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of initialization.
#endif

//bool err = false;
if (err) {
  FILE* stream;
  #ifdef _WIN32
    freopen_s(&stream, "log.txt", "w", stdout);
      if (stream) {
        fprintf(stream, "%s\tFailed to initialize OpenGL loader! Error: %s\n", time_helper::GetTimeinfo().c_str(), glewGetErrorString(err));
        fclose(stream);
        }
  #endif // _WIN32
  #ifdef __linux__
    stream = fopen("./log.txt", "w");
    fprintf(stream, "%s\tFailed to initialize OpenGL loader! Error: %s\n", time_helper::GetTimeinfo().c_str(), glewGetErrorString(err));
    fclose(stream);
  #endif // __linux__
  return 1;
} else {
  const GLubyte* renderer = glGetString(GL_RENDERER);
  const GLubyte* vendor = glGetString(GL_VENDOR);
  const GLubyte* version = glGetString(GL_VERSION);
  const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

  GLint major, minor;
  glGetIntegerv(GL_MAJOR_VERSION, &major);
  glGetIntegerv(GL_MINOR_VERSION, &minor);

  FILE* stream;

  #ifdef _WIN32
    freopen_s(&stream, "log.txt", "w", stdout);

    if (stream) {
      fprintf(stream, "%s\tSuccessful initialized OpenGL...\n", time_helper::GetTimeinfo().c_str());
      fprintf(stream, "\t\t\t\t\tGL Vendor\t\t: %s\n", vendor);
      fprintf(stream, "\t\t\t\t\tGL Renderer\t\t: %s\n", renderer);
      fprintf(stream, "\t\t\t\t\tGL Version (string)\t: %s\n", version);
      fprintf(stream, "\t\t\t\t\tGL Version (integer)\t: %d.%d\n", major, minor);
      fprintf(stream, "\t\t\t\t\tGLSL Version\t\t: %s\n", glslVersion);
      fclose(stream);
    }
  #endif // _WIN32
  #ifdef __linux__
    stream = fopen("./log.txt", "w");
    fprintf(stream, "%s\tSuccessful initialized OpenGL...\n", time_helper::GetTimeinfo().c_str());
    fprintf(stream, "\t\t\t\t\tGL Vendor\t\t: %s\n", vendor);
    fprintf(stream, "\t\t\t\t\tGL Renderer\t\t: %s\n", renderer);
    fprintf(stream, "\t\t\t\t\tGL Version (string)\t: %s\n", version);
    fprintf(stream, "\t\t\t\t\tGL Version (integer)\t: %d.%d\n", major, minor);
    fprintf(stream, "\t\t\t\t\tGLSL Version\t\t: %s\n", glslVersion);
    fclose(stream);
  #endif // __linux__
}

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

  // Create gui object
  Gui appGui;
  // Create default scene object

  Scene appScene(1200, 800);
  // Update Gui to display the windows
  //appGui.WindowUpdate(appScene, window->getSize().x, window->getSize().y);
  //appGui.WindowUpdate(window->GetWidth(), window->GetHeight());

  //sf::Clock deltaClock;

  // Always save the window size each loop cycle
  //GLuint oldWidth = window->getSize().x;
  //GLuint oldHeight = window->getSize().y;

  ProjectManager::SetStatus(project_status_t::IDLE);
  std::string projectName = "TestProject";
  ProjectManager::SetName(projectName);

  // Setup Platform/Renderer bindings
  ImGui_ImplSDL2_InitForOpenGL(window->GetWindow(), window->GetGLContext());
  ImGui_ImplOpenGL3_Init(window->GetGLSLVersion());

  // Main Loop
  while (appGui.IsOpen())
  {
    processEvents(window->GetWindow(), appScene, appGui);

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(window->GetWindow());
    ImGui::NewFrame();

    // Update and render 3d scene
    appScene.Update(window->GetWidth(), window->GetHeight());

    // Update and render gui
    //appGui.Update(window->GetWidth(), window->GetHeight());
    appGui.WindowUpdate(window->GetWidth(), window->GetHeight());
    appGui.Render(appScene);

    appScene.Render();

    ImGui::ShowDemoWindow();

    // If the project name changes, update the window title
    /*if (myGui.project_name_changed)
    {
        SDL_SetWindowTitle(window, ("[" + myGui.project_name + "] - SoundIMP").c_str());
        myGui.project_name_changed = false;
    }*/

    // Render ImGui to show the gui
    ImGui::Render();

    // Set the background color
    ImGuiStyle* style = &ImGui::GetStyle();
    ImColor col = style->Colors[ImGuiCol_PopupBg];
    glClearColor(col.Value.x, col.Value.y, col.Value.z, col.Value.w);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // render end
    SDL_GL_SwapWindow(window->GetWindow());

    // update
    glfwPollEvents();

    
    // if (ProjectManager::GetStatus() == project_status_t::SAVE)
    // {
    //     // create and open a character archive for output
    //     std::ofstream ofs(ProjectManager::GetName() + ".jrm");

    //     // save data to archive
    //     {
    //         cereal::XMLOutputArchive ar(ofs, cereal::XMLOutputArchive::Options(6, true, false));
    //         //cereal::JSONOutputArchive ar(ofs);
    //         //cereal::BinaryOutputArchive ar(ofs);
    //         ar( CEREAL_NVP(appGui) );
    //         ar( CEREAL_NVP(appScene) );
    //         //ar( cereal::make_nvp("window_with_", oldWidth), cereal::make_nvp("window_height_", oldHeight) );

    //         long double c = pow(0.5, 1000);
    //         ar ( cereal::make_nvp("a_very_small_value_", c) );
    //     }

    //     ProjectManager::SetStatus(project_status_t::IDLE);
    // }
    // else if (ProjectManager::GetStatus() == project_status_t::LOAD)
    // {
    //     std::ifstream is(ProjectManager::GetName() + ".jrm");
    //     cereal::XMLInputArchive ar(is);
    //     //cereal::JSONInputArchive ar(is);
    //     //cereal::BinaryInputArchive ar(is);

    //     GLuint width;
    //     GLuint height;

    //     ar( appGui );
    //     ar( appScene );
    //     //ar( width, height);

    //     //window->setSize({ width, height });

    //     ProjectManager::SetStatus(project_status_t::IDLE);
    // }
    
  }

  //window->close();
  //window = nullptr;
  //ImGui::SFML::Shutdown();

  return 0;
}
