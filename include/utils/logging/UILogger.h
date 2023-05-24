#pragma once

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

#include "ILogger.h"
#include "FormatHelper.h"
#include "TimeHelper.h"
#include "memory"

#include <GL/glew.h>    // Initialize with glewInit()

#include <SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES3)
#include <SDL_opengles3.h>
#else
#include <SDL_opengl.h>
#endif

#define MAX_STORED_LOGMSG 200               /**< Defines the maximal number of messages to be stored. */

class UILogger : public ILogger
{
public:
    UILogger();
    LogMessage Log(log_t type, const char* format, ...) override;
    LogMessage Log(const char* format, ...) override;
    std::shared_ptr<std::vector<LogMessage>> GetLogs() const { return std::make_shared<std::vector<LogMessage>>(logs_); };
    GLenum CheckGLError(const char *file, int line);

private:
    // void GLAPIENTRY glMessageCallback( GLenum source, GLenum type,GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
    GLenum glCheckError_(const char *file, int line);

    std::vector<LogMessage> logs_;               /**< Contains all messages, that are shared at the moment. */
};