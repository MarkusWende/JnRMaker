#include "sstream"

#include "AppWindow.h"
#include "MessageManager.h"

int AppWindow::SetupSDL()
{
	
}

void AppWindow::CreateNewWindow(int width, int height, const char* name)
{
// 	SDL_GetCurrentDisplayMode(0, &display_mode_);

// 	window_ = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED_DISPLAY(0), SDL_WINDOWPOS_CENTERED_DISPLAY(0), width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
// 	gl_context_ = SDL_GL_CreateContext(window_.get());

// 	// VSync
// 	//SDL_GL_SetSwapInterval(1);		// enable
// 	SDL_GL_SetSwapInterval(0);			// disable

// #ifdef __EMSCRIPTEN__
    
// #else
//     // load software icon
// 	SDL_Surface* icon = IMG_Load("icon.png");
// 	// set window icon
// 	SDL_SetWindowIcon(window_.get(), icon);
// 	//SDL_FreeSurface(icon);
// #endif
	
}

int AppWindow::GetWidth() {
	int width, height;
	SDL_GetWindowSize(window_.get(), &width, &height);
	return width;
}

void AppWindow::SetWidth(int width){
	int dummy_width, height;
	SDL_GetWindowSize(window_.get(), &dummy_width, &height);
	SDL_SetWindowSize(window_.get(), width, height);
	return;
}

int AppWindow::GetHeight() {
	int width, height;
	SDL_GetWindowSize(window_.get(), &width, &height);
	return height;
}

void AppWindow::SetHeight(int height) {
	int width, dummy_height;
	SDL_GetWindowSize(window_.get(), &width, &dummy_height);
	SDL_SetWindowSize(window_.get(), width, height);
	return;
}

void AppWindow::SetSize(int width, int height) {
	SDL_SetWindowSize(window_.get(), width, height);
	return;
}

const char* AppWindow::GetName() {
	return SDL_GetWindowTitle(window_.get());
}

void AppWindow::SetName(const char* name) {
	SDL_SetWindowTitle(window_.get(), name);
	return;
}

std::shared_ptr<SDL_Window> AppWindow::GetWindow()
{
	return window_;
}

std::shared_ptr<SDL_GLContext> AppWindow::GetGLContext()
{
	return gl_context_;
}

const char* AppWindow::GetGLSLVersion()
{
	return glsl_version_.c_str();
}
