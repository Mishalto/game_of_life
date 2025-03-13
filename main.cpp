#include <SFML/Graphics.hpp>

constexpr int width = 1920;
constexpr int heigth = 1080;
[[maybe_unused]]constexpr int cell_size = 20;

int main()
{
    sf::RenderWindow window(sf::VideoMode({width, heigth}), "Gane of life");
    window.setFramerateLimit(60);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        window.display();
    }
}