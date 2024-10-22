#include <SFML/Graphics.hpp>
#include "GameManager.h"
#include <iostream>
#include <sstream>

int main()
{

    sf::RenderWindow window(sf::VideoMode(1000, 800), "Breakout");
    GameManager gameManager(&window);
    std::stringstream ss;

    gameManager.initialize();

    sf::Clock clock;
    float deltaTime;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (gameManager.getLevelComplete() == true) {
                if (event.type == sf::Event::TextEntered)
                {
              
                    if (event.text.unicode < 128 && event.text.unicode != 59 && event.text.unicode != 8)
                    {
                        ss.put(static_cast<char>(event.text.unicode));

                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace) && !ss.str().empty())
                    {
                        std::string temp(ss.str());
                        temp.erase(temp.length() - 1, 1);
                        ss.str(temp);
                        ss.seekp(temp.length());
                    }
                }
            }
           
        }

        deltaTime = clock.restart().asSeconds();

        gameManager.update(deltaTime, &ss);

        window.clear();
        gameManager.render();
        window.display();
    }

    return 0;
}
