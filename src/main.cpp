#include <iostream>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include "../include/imgui/imgui.h"
#include "../include/imgui/imgui-SFML.h"
#include "../include/events.h"
#include "../include/gui.h"
#include "../include/resource_manager.h"
#include "../include/scene.h"


int main()
{
  //glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  sf::RenderWindow window(sf::VideoMode(1800, 1024), "[NewProject.jrm] - JnRMaker");
  auto desktop = sf::VideoMode::getDesktopMode();
  window.setPosition(sf::Vector2i(desktop.width/2 - window.getSize().x/3, desktop.height/2 - window.getSize().y/2));
  window.setFramerateLimit(60);
  ImGui::SFML::Init(window);

  // Create gui object
  Gui appGui;
  // Create default scene object
  Scene appScene(800, 800);
  // Update Gui to display the windows
  appGui.Update(window.getSize().x, window.getSize().y, appScene);

  sf::Clock deltaClock;

  unsigned int oldWidth = window.getSize().x;
  unsigned int oldHeight = window.getSize().y;
  while (window.isOpen()) {
    processEvents(window);

    ImGui::SFML::Update(window, deltaClock.restart());

    if (oldWidth != window.getSize().x || oldHeight != window.getSize().y)
      appGui.Update(window.getSize().x, window.getSize().y, appScene);
    appGui.Render();

    appScene.Render();

    window.clear();
    ImGui::SFML::Render(window);
    window.display();

    oldWidth = window.getSize().x;
    oldHeight = window.getSize().y;
  }

  ImGui::SFML::Shutdown();

  return 0;
}
