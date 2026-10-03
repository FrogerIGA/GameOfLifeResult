#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "functions.h"
using namespace sf;
using namespace std;
void main()
{
    // Стандар: B2 / S23      to born = 2; to survive = 2 or 3
    int window_size_x = 950, window_size_y = 950; // ОБЯЗАТЕЛЬНО КРАТНО 5
    int framerate_ = 100;
    // cellSize in pixels
    
    vector<int> neededToBorn = { 3, 5, 6, 7, 8 };
    vector<int> neededToLive = { 5, 6, 7, 8 };
    RenderWindow window(VideoMode(window_size_x, window_size_y), "DVD_copy");
    
    fieldClass field(window_size_x, window_size_y, 10, neededToLive, neededToBorn);
    bool wasPressed = false;

    cout << "press [CTRL] for single step" << endl;
    cout << "hold [SPACE] for fast steps" << endl;
    cout << "[LEFT CLICK] to set up a cell" << endl;
    cout << "[RIGHT CLICK] to clear a cell" << endl;


    while (window.isOpen())
    {
        window.setFramerateLimit(framerate_);

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::LControl)
                {
                    if (!wasPressed)
                    {
                        field.stepInit(window);
                        wasPressed = true;
                    }
                }
            }

            if (event.type == Event::KeyReleased)
            {
                if (event.key.code == Keyboard::LControl)
                {
                    wasPressed = false;
                }
            }
        }

        if (Mouse::isButtonPressed(Mouse::Left))
        {
            Vector2f mousePos = Vector2f(Mouse::getPosition(window).x, Mouse::getPosition(window).y);
            field.onClick(mousePos, window, -1);
        }
        if (Mouse::isButtonPressed(Mouse::Right))
        {
            Vector2f mousePos = Vector2f(Mouse::getPosition(window).x, Mouse::getPosition(window).y);
            field.onClick(mousePos, window, 1);
        }
        if (Keyboard::isKeyPressed(Keyboard::Space))
        {
            field.stepInit(window);
        }

        window.clear();
        field.draw(window);
        window.display();
    }
}
