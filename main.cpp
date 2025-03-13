#include <iostream>
#include <array>
#include <vector>
#include <SFML/Graphics.hpp>

constexpr int width = 1920;
constexpr int height = 1080;

constexpr int cell_size = 20;
constexpr int x_cells = width / cell_size;
constexpr int y_cells = height / cell_size;

void init_grid(std::vector<std::vector<sf::RectangleShape>>& grid) {
    for (size_t i = 0; i < grid.size(); ++i) {
        for (size_t j = 0; j < grid[i].size(); ++j) {
            grid[i][j].setFillColor(sf::Color::White);
            grid[i][j].setSize({cell_size, cell_size});
            grid[i][j].setPosition({static_cast<float>(j) * cell_size, static_cast<float>(i) * cell_size});
        }
    }
}

int main() {
    std::vector<std::vector<sf::RectangleShape>> grid(y_cells, std::vector<sf::RectangleShape>(x_cells));
    init_grid(grid);

    sf::RenderWindow window(sf::VideoMode({width, height}), "Game of life", sf::Style::None);
    window.setFramerateLimit(60);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.clear();
        for (const auto& s : grid) {
            for (const auto& j : s) {
                window.draw(j);
            }
        }
        window.display();
    }
}