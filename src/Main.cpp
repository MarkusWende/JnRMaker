//#define IMGUI_USER_CONFIG "config.h"

//#include "config.h"
#include <stdio.h>

#include "Gui.h"
#include "ProjectManager.h"
#include "ResourceManager.h"
#include "MessageManager.h"
#include "Scene.h"
#include "Events.h"
#include "Injector.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/fetch.h>
#endif

#include "JNRWindow.h"

struct MainLoopData {
    std::shared_ptr<Gui> gui;
    std::shared_ptr<Scene> scene;
};

// For clarity, our main loop code is declared at the end.
static void main_loop(void*);

int main(int, char**)
{
    MainLoopData data;
    Injector injector;
    auto logger = injector.GetLogger();
    auto graphicsManager = injector.GetGraphicsManager();

#ifdef __EMSCRIPTEN_PTHREADS__
    logger->Log(log_t::DEBUG, "__EMSCRIPTEN_PTHREADS__ is enabled.");
#endif

    //data.Window = appWindow.GetWindow();
    //data.GLContext = appWindow.GetGLContext();
    auto appWindow = injector.Create<JNRWindow>(logger);
    appWindow->InitSDL();
    appWindow->CreateSDLWindow();
    appWindow->CreateSDLContext();
    appWindow->InitGlew();
    appWindow->ConfigureOpenGL();

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
#ifdef __EMSCRIPTEN__
    io.IniFilename = nullptr;
#endif // __EMSCRIPTEN__

    // Setup Dear ImGui style
    //ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(appWindow->GetWindow(), appWindow->GetContext());
    ImGui_ImplOpenGL3_Init(appWindow->GetGLSLVersion().c_str());

    // Gui
    //appGui = new Gui();
    //appGui = std::make_shared<Gui>();
    //auto appGui = injector.Create<Gui>(logger);
    data.gui = injector.Create<Gui>(logger, graphicsManager);

    // Scene
    data.scene = injector.Create<Scene>(logger, 1280, 720);
    data.scene.get()->GetWidth();

#ifdef __EMSCRIPTEN__
    // This function call won't return, and will engage in an infinite loop, processing events from the browser, and dispatching them.
    emscripten_set_main_loop_arg(main_loop, &data, 60, true);
#else
    //ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    NFD_Init();
    
    // Main loop
    while (data.gui->IsOpen())
    {
        main_loop(&data);
    }

    // Cleanup
    NFD_Quit();

#endif

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    //ImPlot::DestroyContext();

    appWindow->CleanUp();

    return 0;
}

static void main_loop(void* arg)
{
    // Cast the argument back to its original type
    auto data = static_cast<MainLoopData*>(arg);
    auto currWindow = SDL_GL_GetCurrentWindow();
    auto currGLContext = SDL_GL_GetCurrentContext();

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
            data->gui->Close();
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(currWindow))
            data->gui->Close();
    }

    if (ProjectManager::GetStatus() == project_status_t::SAVE)
    {
        ProjectManager::SaveCreate();

        data->scene->save();

        ProjectManager::SaveWrite();
        ProjectManager::SetStatus(project_status_t::IDLE);
    }
    else if (ProjectManager::GetStatus() == project_status_t::LOAD)
    {
        ProjectManager::SetStatus(project_status_t::IDLE);
    }

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    // Update and render gui
    int width, height;
    SDL_GetWindowSize(currWindow, &width, &height);
    data->gui->WindowUpdate(data->scene, width, height);
    processEvents(data->scene, data->gui);
    // appGui->Render(appScene);
    data->gui->Draw(data->scene);
    //appGui->ShowBackendCheckerWindow();

    data->scene->Render();

    //ImGui::ShowDemoWindow();
    //ImPlot::ShowDemoWindow();

    // Rendering
    ImGui::Render();
    SDL_GL_MakeCurrent(currWindow, currGLContext);
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

    SDL_GL_SwapWindow(currWindow);
}