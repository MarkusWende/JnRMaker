#include "../include/window.h"
#include "../include/message_manager.h"
#include "sstream"


int JNRWindow::SetupSDL()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
	{
		printf("Error: %s\n", SDL_GetError());
		return -1;
	}

	// Decide GL+GLSL versions
#if __APPLE__
	// GL 4.1 + GLSL 410
	glsl_version_ = "#version 330";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#else
	// GL 4.1 + GLSL 410
	glsl_version_ = "#version 330";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#endif

	return 0;
}

void JNRWindow::CleanUp() {
	SDL_GL_DeleteContext(gl_context_);
	SDL_DestroyWindow(window_);
	SDL_Quit();
	return;
}
/*
void JNRWindow::ShowLoadingWindow() {
	SDL_Surface* loadingWindowImage = IMG_Load("loading.png");
	// create the loading window
	SDL_Window* loadingWindow = SDL_CreateWindow("Loading Image", SDL_WINDOWPOS_CENTERED_DISPLAY(0), SDL_WINDOWPOS_CENTERED_DISPLAY(0), 600, 400, SDL_WINDOW_OPENGL);
	// set the window boarder to false
	SDL_SetWindowBordered(loadingWindow, SDL_FALSE);
	// create the loading window surface
	SDL_Surface* gLoadingScreenSurface = SDL_GetWindowSurface(loadingWindow);
	// apply the image
	SDL_BlitSurface(loadingWindowImage, NULL, gLoadingScreenSurface, NULL);
	// update the surface
	SDL_UpdateWindowSurface(loadingWindow);
	// wait four seconds
	SDL_Delay(4000);

	// deallocate surfaces
	SDL_FreeSurface(gLoadingScreenSurface);
	gLoadingScreenSurface = nullptr;
	loadingWindowImage = nullptr;
	// destroy loading window
	SDL_DestroyWindow(loadingWindow);
	loadingWindow = nullptr;
}
*/
void JNRWindow::CreateNewWindow(int width, int height, const char* name) {
	SDL_GetCurrentDisplayMode(0, &display_mode_);

	window_ = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED_DISPLAY(0), SDL_WINDOWPOS_CENTERED_DISPLAY(0), width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	gl_context_ = SDL_GL_CreateContext(window_);

	// VSync
	SDL_GL_SetSwapInterval(1);		// enable
	//SDL_GL_SetSwapInterval(0);			// disable

	// load software icon
	SDL_Surface* icon = IMG_Load("icon.png");
	// set window icon
	SDL_SetWindowIcon(window_, icon);
	//SDL_FreeSurface(icon);
}

int JNRWindow::GetWidth() {
	int width, height;
	SDL_GetWindowSize(window_, &width, &height);
	return width;
}

void JNRWindow::SetWidth(int width){
	int dummy_width, height;
	SDL_GetWindowSize(window_, &dummy_width, &height);
	SDL_SetWindowSize(window_, width, height);
	return;
}

int JNRWindow::GetHeight() {
	int width, height;
	SDL_GetWindowSize(window_, &width, &height);
	return height;
}

void JNRWindow::SetHeight(int height) {
	int width, dummy_height;
	SDL_GetWindowSize(window_, &width, &dummy_height);
	SDL_SetWindowSize(window_, width, height);
	return;
}

void JNRWindow::SetSize(int width, int height) {
	SDL_SetWindowSize(window_, width, height);
	return;
}

const char* JNRWindow::GetName() {
	return SDL_GetWindowTitle(window_);
}

void JNRWindow::SetName(const char* name) {
	SDL_SetWindowTitle(window_, name);
	return;
}

SDL_Window* JNRWindow::GetWindow() {
	return window_;
}

SDL_GLContext JNRWindow::GetGLContext() {
	return gl_context_;
}

const char* JNRWindow::GetGLSLVersion() {
	return glsl_version_;
}
