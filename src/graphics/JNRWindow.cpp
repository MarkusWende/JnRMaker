#include "JNRWindow.h"

void
JNRWindow::InitSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
	{
		MessageManager::Log("Error: %s\n", SDL_GetError());
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
JNRWindow::InitGLEW()
{
    bool err = glewInit() != GLEW_OK;
    std::stringstream msg;

    //bool err = false;
    if (err)
    {
        MessageManager::Log("Failed to initialize OpenGL loader! Error: %s", glewGetErrorString(err));
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
        MessageManager::Log(msg);
    }
}