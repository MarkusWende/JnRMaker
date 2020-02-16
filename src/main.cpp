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


int main()
{
  //glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  sf::RenderWindow window(sf::VideoMode(1800, 1024), "[NewProject.jrm] - JnRMaker");
  auto desktop = sf::VideoMode::getDesktopMode();
  window.setPosition(sf::Vector2i(desktop.width/2 - window.getSize().x/3, desktop.height/2 - window.getSize().y/2));
  window.setFramerateLimit(120);
  ImGui::SFML::Init(window);

  sf::CircleShape shape(100.f);
  shape.setFillColor(sf::Color::Green);

  ResourceManager::LoadTexture("assets/tiles/keen4_tiles2.png", "face");

  // Create gui object
  Gui appGui;

  sf::Clock deltaClock;
  while (window.isOpen()) {
    processEvents(window);

    ImGui::SFML::Update(window, deltaClock.restart());

    appGui.Update(window.getSize().x, window.getSize().y);
    appGui.Render();

    window.clear();
    //window.draw(shape);
    ImGui::SFML::Render(window);
    window.display();
  }

  ImGui::SFML::Shutdown();

  return 0;
}
