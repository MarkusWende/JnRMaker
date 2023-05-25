#include "AppWindow.h"

void GLAPIENTRY
glMessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
    (void)length;
    auto logger = *static_cast<std::shared_ptr<ILogger>*>(const_cast<void*>(userParam));
    std::stringstream msg;
    msg << source << "\tOpenGL: " << " id = " << id << " type = 0x" << type << ", severity = 0x" << severity << ", message = " << message;
	auto logType = log_t::DEBUG;

    if (type == GL_DEBUG_TYPE_ERROR)
		logType = log_t::ERROR_T;

    logger.get()->Log(logType, "%s", msg.str().c_str());
}

AppWindow::AppWindow(std::shared_ptr<ILogger> logger) : logger_(logger)
{
    window_ = NULL;
    gl_context_ = NULL;
}

void
AppWindow::InitSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
	{
		logger_->Log(log_t::ERROR_T, "Error: %s\n", SDL_GetError());
		return;
	}

    // Decide GL+GLSL versions
    std::string glslVersion = "#version 100";
    #if __APPLE__
        // GL 4.1 + GLSL 410
        glslVersion = "#version 330";
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    #elif __EMSCRIPTEN__
        // For the browser using Emscripten, we are going to use WebGL1 with GL ES2. See the Makefile. for requirement details.
        // It is very likely the generated file won't work in many browsers. Firefox is the only sure bet, but I have successfully
        // run this code on Chrome for Android for example.
        glslVersion = "#version 300 es";
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    #else
        // GL 4.1 + GLSL 410
        glslVersion = "#version 330";
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    #endif

	glsl_version_ = glslVersion;

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
}

void
AppWindow::InitGlew()
{
    bool err = glewInit() != GLEW_OK;
    std::stringstream msg;

    //logger_->Log(log_t::DEBUG, "%s", glGetString(GL_EXTENSIONS));

    if (err)
    {
        logger_->Log(log_t::ERROR_T, "Failed to initialize OpenGL loader! Error: %s", glewGetErrorString(err));
        return;
    }
    else
    {
        const GLubyte* renderer = glGetString(GL_RENDERER);
        const GLubyte* vendor = glGetString(GL_VENDOR);
        const GLubyte* version = glGetString(GL_VERSION);
        const GLubyte* glslVersionNumber = glGetString(GL_SHADING_LANGUAGE_VERSION);

        if (glslVersionNumber == NULL)
        {
            //MessageManager::Log("OpenGL GLSL Version is Null.. Aborting");
            return;
        }
        

        GLint major, minor, depthBufferBits;
        glGetIntegerv(GL_MAJOR_VERSION, &major);
        glGetIntegerv(GL_MINOR_VERSION, &minor);
        glGetIntegerv(GL_DEPTH_BITS, &depthBufferBits );

        msg << "Successful initialized OpenGL..." << std::endl;
        msg << "\t\t\t\t\t\tGL Vendor:\t\t\t\t" << vendor << std::endl;
        msg << "\t\t\t\t\t\tGL Renderer:\t\t\t" << renderer << std::endl;
        msg << "\t\t\t\t\t\tGL Version (string):\t" << version << std::endl;
        msg << "\t\t\t\t\t\tGL Version (integer):\t" << major << "." << minor << std::endl;
        msg << "\t\t\t\t\t\tGLSL Version\t\t\t" << glslVersionNumber << std::endl;
        msg << "\t\t\t\t\t\tDepth Buffer bits:\t\t" << depthBufferBits;
        logger_->Log(log_t::DEBUG ,"%s", msg.str().c_str());
    }
}

void
AppWindow::ConfigureOpenGL()
{
    SDL_GL_SetSwapInterval(1); // Enable vsync
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE); // Enabled by default on some drivers, but not all so always enable to make sure

    PFNGLDEBUGMESSAGECALLBACKARBPROC glDebugMessageCallback = (PFNGLDEBUGMESSAGECALLBACKARBPROC)SDL_GL_GetProcAddress("glDebugMessageCallbackARB");
    if (glDebugMessageCallback)
    {
        // Function pointer is valid, proceed to set the callback
        logger_->Log(log_t::DEBUG, "glDebugMessageCallback available.");
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback( glMessageCallback, &logger_);
    }
    else
    {
        logger_->Log(log_t::WARNING, "glDebugMessageCallback not available.");
    }
}

void
AppWindow::CreateSDLWindow()
{
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    int screenWidth = dm.w;
    int screenHeight = dm.h;
    SDL_Rect windowRect = {0, 0, screenWidth, screenHeight};

    SDL_WindowFlags windowFlags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    window_ = SDL_CreateWindow("JnRMaker", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowRect.w, windowRect.h, windowFlags);

    //std::shared_ptr<SDL_Window> windowSharedPtr(windowPtr, SDL_DestroyWindow);
	//window_ = windowSharedPtr;
#ifdef _WIN32
    int windowBorderSizeTop, windowBorderSizeLeft, windowBorderSizeBottom, windowBorderSizeRight;
    SDL_GetWindowBordersSize(window_, &windowBorderSizeTop, &windowBorderSizeLeft, &windowBorderSizeBottom, &windowBorderSizeRight);
    {
        std::stringstream msg;
        msg << "top: " << windowBorderSizeTop << "\tleft: " << windowBorderSizeLeft << "\tbottom: " << windowBorderSizeBottom << "\tright: " << windowBorderSizeRight;
        logger_->Log(log_t::DEBUG ,"%s", msg.str().c_str());
    }

    SDL_SetWindowBordered(window_, SDL_FALSE);
    SDL_SetWindowPosition(window_, windowRect.x, screenHeight - windowRect.h);
    SDL_SetWindowSize(window_, windowRect.w, windowRect.h - (windowBorderSizeTop + windowBorderSizeBottom));
#endif
}

void
AppWindow::CreateSDLContext()
{
    gl_context_ = SDL_GL_CreateContext(window_);
    //std::shared_ptr<SDL_GLContext> contextSharedPtr(new SDL_GLContext(contextPtr), SDL_GL_DeleteContext);
	//gl_context_ = contextSharedPtr;

    if (!gl_context_)
    {
        logger_->Log(log_t::ERROR_T ,"Failed to initialize WebGL context!");
        return;
    }
}

void
AppWindow::CleanUp()
{
    SDL_GL_DeleteContext(gl_context_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
}