#include "UILogger.h"

// void GLAPIENTRY
// glMessageCallback( GLenum source,
//                  GLenum type,
//                  GLuint id,
//                  GLenum severity,
//                  GLsizei length,
//                  const GLchar* message,
//                  const void* userParam )
// {
//     //(void)userParam;
//     auto logger = *static_cast<std::vector<ILogger>*>(const_cast<void*>(userParam));
//     (void)length;
//     (void)id;
//     std::stringstream msg;
//     msg << source << "\tOpenGL: " << " type = 0x" << type << ", severity = 0x" << severity << ", message = " << message;
// 	LogMessage tmpMsg;
// 	tmpMsg.msg = msg.str();
// 	tmpMsg.timeinfo = TimeHelper::GetTimeinfo();
// 	tmpMsg.popup = false;
// 	tmpMsg.type = log_t::INFO;

//     if (type == GL_DEBUG_TYPE_ERROR)
// 		tmpMsg.type = log_t::ERROR_T;

// 	//logs_.insert(logs_.begin(), tmpMsg);
// }

UILogger::UILogger()
{
    // PFNGLDEBUGMESSAGECALLBACKARBPROC glDebugMessageCallback = (PFNGLDEBUGMESSAGECALLBACKARBPROC)SDL_GL_GetProcAddress("glDebugMessageCallbackARB");
    // if (glDebugMessageCallback)
    // {
    //     // Function pointer is valid, proceed to set the callback
    //     glEnable(GL_DEBUG_OUTPUT);
    //     glDebugMessageCallback( glMessageCallback, &logs_);
    // }
    // else
    // {
    //     // glDebugMessageCallback is not supported
    // }
}

LogMessage
UILogger::Log(const char* format, ...)
{
    va_list args;
    va_start(args, format);

    std::string buffer = FormatHelper::vformat(format, args);
    int n = buffer.size();
    va_end(args);

	std::stringstream out;
    out.write(buffer.data(), n);

    LogMessage tmpMsg;

	tmpMsg.msg = out.str();
	tmpMsg.timeinfo = TimeHelper::GetTimeinfo();
	tmpMsg.type = log_t::INFO;
	tmpMsg.popup = false;

	logs_.insert(logs_.begin(), tmpMsg);

	if (logs_.size() > MAX_STORED_LOGMSG)
		logs_.pop_back();

	out.str("");

    return tmpMsg;
}

LogMessage
UILogger::Log(log_t type, const char* format, ...)
{
    va_list args;
    va_start(args, format);

    std::string buffer = FormatHelper::vformat(format, args);
    int n = buffer.size();
    va_end(args);

	std::stringstream out;
    out.write(buffer.data(), n);

    LogMessage tmpMsg;

	tmpMsg.msg = out.str();
	tmpMsg.timeinfo = TimeHelper::GetTimeinfo();
	tmpMsg.type = type;
	tmpMsg.popup = false;

	logs_.insert(logs_.begin(), tmpMsg);

	if (logs_.size() > MAX_STORED_LOGMSG)
		logs_.pop_back();

	out.str("");

    return tmpMsg;
}

GLenum
UILogger::CheckGLError(const char *file, int line)
{
	return glCheckError_(file, line);
}

GLenum
UILogger::glCheckError_(const char *file, int line)
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
		LogMessage tmpMsg;

		tmpMsg.msg = msg.str();
		tmpMsg.timeinfo = TimeHelper::GetTimeinfo();
		tmpMsg.type = log_t::ERROR_T;
		tmpMsg.popup = false;

		logs_.insert(logs_.begin(), tmpMsg);
    }
    return errorCode;
}
//#define glCheckError() UILogger::glCheckError_(__FILE__, __LINE__)