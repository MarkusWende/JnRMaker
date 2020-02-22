// gui.h
#ifndef GUI_H
#define GUI_H

#include <SFML/OpenGL.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

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

  void            Update(GLuint width, GLuint height);
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
