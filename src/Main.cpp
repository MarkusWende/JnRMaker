//#define IMGUI_USER_CONFIG "config.h"

//#include "config.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>

#include "Gui.h"
#include "ProjectManager.h"
#include "ResourceManager.h"
#include "MessageManager.h"
#include "Scene.h"
#include "Events.h"
#include "Injector.h"
#include "JNRWindow.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/fetch.h>
#endif

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
        MessageManager::AddMessage(msg, message_t::ERROR_T);
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
        MessageManager::AddMessage(msg, message_t::ERROR_T);
    }
    return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

// Emscripten requires to have full control over the main loop. We're going to store our SDL book-keeping variables globally.
// Having a single function that acts as a loop prevents us to store state in the stack of said function. So we need some location for this.
SDL_Window*     sdlWindow = NULL;
SDL_GLContext   sdlGLContext = NULL;

// Gui
//std::shared_ptr<Scene> appScene;
//Gui* appGui;
//Scene* appScene;

struct MainLoopData {
    std::shared_ptr<Gui> Gui;
    std::shared_ptr<Scene> Scene;
};

// For clarity, our main loop code is declared at the end.
static void main_loop(void*);

int main(int, char**)
{
    MainLoopData data;
    //data.Window = appWindow.GetWindow();
    //data.GLContext = appWindow.GetGLContext();

    auto appWindow = JNRWindow();
    appWindow.InitSDL();

#ifdef __EMSCRIPTEN_PTHREADS__
    {
        std::stringstream msg;
        msg << "__EMSCRIPTEN_PTHREADS__ is enabled.";
        MessageManager::AddMessage(msg, message_t::DEBUG);
    }
#endif

    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    int screenWidth = dm.w;
    int screenHeight = dm.h;
    SDL_Rect windowRect = {0, 0, screenWidth, screenHeight};

    SDL_WindowFlags windowFlags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    sdlWindow = SDL_CreateWindow("JnRMaker", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowRect.w, windowRect.h, windowFlags);
    //std::shared_ptr<SDL_Window> windowSharedPtr(windowPtr, SDL_DestroyWindow);
	//window_ = windowSharedPtr;
#ifdef _WIN32
    int windowBorderSizeTop, windowBorderSizeLeft, windowBorderSizeBottom, windowBorderSizeRight;
    SDL_GetWindowBordersSize(sdlWindow, &windowBorderSizeTop, &windowBorderSizeLeft, &windowBorderSizeBottom, &windowBorderSizeRight);
    {
        std::stringstream msg;
        msg << "top: " << windowBorderSizeTop << "\tleft: " << windowBorderSizeLeft << "\tbottom: " << windowBorderSizeBottom << "\tright: " << windowBorderSizeRight;
        MessageManager::AddMessage(msg, message_t::WARNING);
    }

    SDL_SetWindowBordered(sdlWindow, SDL_FALSE);
    SDL_SetWindowPosition(sdlWindow, windowRect.x, screenHeight - windowRect.h);
    SDL_SetWindowSize(sdlWindow, windowRect.w, windowRect.h - (windowBorderSizeTop + windowBorderSizeBottom));
#endif

    // std::shared_ptr<JNRWindow> appWindow;
    // appWindow = std::make_shared<JNRWindow>();
    // appWindow.get()->SetupSDL();

    //gGLContext = SDL_GL_CreateContext(sdlWindow);
    //std::shared_ptr<SDL_GLContext> contextPtr(std::make_shared<SDL_GLContext>(sdlWindow));
    //std::shared_ptr<SDL_GLContext> contextPtr(new SDL_GLContext(SDL_GL_CreateContext(sdlWindow)), [](SDL_GLContext *context){ SDL_GL_DeleteContext(context); });
    sdlGLContext = SDL_GL_CreateContext(sdlWindow);
    //std::shared_ptr<SDL_GLContext> contextSharedPtr(new SDL_GLContext(contextPtr), SDL_GL_DeleteContext);
	//gl_context_ = contextSharedPtr;


    if (!sdlGLContext)
    {
        MessageManager::Log("Failed to initialize WebGL context!");
        return 1;
    }

    SDL_GL_SetSwapInterval(1); // Enable vsync

    // if (SDL_SetWindowFullscreen(sdlWindow, SDL_WINDOW_FULLSCREEN_DESKTOP) != 0)
	// {
	// 	std::stringstream msg;
	// 	msg << "Could not start in fullscreen mode: " << SDL_GetError();
	// 	MessageManager::AddMessage(msg, message_t::ERROR_T);
	// }

    // Initialize OpenGL loader
    // glewExperimental = GL_TRUE;
    
    appWindow.InitGLEW();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#ifndef __EMSCRIPTEN__
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_MULTISAMPLE); // Enabled by default on some drivers, but not all so always enable to make sure
    glDebugMessageCallback( MessageCallback, 0 );
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
    ImGui_ImplSDL2_InitForOpenGL(sdlWindow, sdlGLContext);
    ImGui_ImplOpenGL3_Init(appWindow.GetGLSLVersion().c_str());

    // Gui
    //appGui = new Gui();
    //appGui = std::make_shared<Gui>();
    
    Injector injector;
    auto logger = injector.GetLogger();
    auto resourceManager = injector.GetResourceManager();
    //auto appGui = injector.Create<Gui>(logger);
    data.Gui = injector.Create<Gui>(logger);

    // Scene
    data.Scene = std::make_shared<Scene>(1280, 720);
    data.Scene.get()->GetWidth();

#ifdef __EMSCRIPTEN__
    // This function call won't return, and will engage in an infinite loop, processing events from the browser, and dispatching them.
    emscripten_set_main_loop_arg(main_loop, &data, 60, true);
#else
    //ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    NFD_Init();
    
    // Main loop
    while (data.Gui->IsOpen())
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

    SDL_GL_DeleteContext(sdlGLContext);
    SDL_DestroyWindow(sdlWindow);
    SDL_Quit();

    return 0;
}

static void main_loop(void* arg)
{
    // Cast the argument back to its original type
    MainLoopData* data = static_cast<MainLoopData*>(arg);

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
            data->Gui->Close();
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(sdlWindow))
            data->Gui->Close();
    }

    if (ProjectManager::GetStatus() == project_status_t::SAVE)
    {
        ProjectManager::SaveCreate();

        data->Scene->save();

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
    SDL_GetWindowSize(sdlWindow, &width, &height);
    data->Gui->WindowUpdate(data->Scene, width, height);
    processEvents(data->Scene, data->Gui);
    // appGui->Render(appScene);
    data->Gui->Draw(data->Scene);
    //appGui->ShowBackendCheckerWindow();

    data->Scene->Render();

    //ImGui::ShowDemoWindow();
    //ImPlot::ShowDemoWindow();

    // Rendering
    ImGui::Render();
    SDL_GL_MakeCurrent(sdlWindow, sdlGLContext);
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

    SDL_GL_SwapWindow(sdlWindow);
}