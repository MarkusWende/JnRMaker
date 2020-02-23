// gui.h
#ifndef GUI_H
#define GUI_H

#include <iostream>

#include <SFML/OpenGL.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "../include/imgui/imgui.h"
#include "../include/imgui/imgui-SFML.h"
#include "../include/resource_manager.h"
#include "../include/message_manager.h"
#include "../include/scene.h"

struct GuiWindow {
  GLuint w;
  GLuint h;
  GLfloat wPercent;
  GLfloat hPercent;
};

class Gui
{
public:
  Gui();
	~Gui();

  void            Update(GLuint width, GLuint height, Scene &scene);
  void				    Render();

private:
  GLuint          width_;
  GLuint          height_;
  GuiWindow       windowScene_;
  GuiWindow       windowMessages_;
  GuiWindow       windowSideBarRight_;
  GLuint          mainMenuBarHeight_;

  void            init_();
  void            customGuiStyle_();
};

#endif	// GUI_H
