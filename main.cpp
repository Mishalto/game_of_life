#include <iostream>
#include <vector>
#include <utility>
#include <thread>
#include <chrono>
#include <SFML/Graphics.hpp>

constexpr int width = 1920;
constexpr int height = 1080;

constexpr int cell_size = 20;
constexpr int x_cells = width / cell_size;
constexpr int y_cells = height / cell_size;

void set_start_pos(std::vector<std::vector<sf::RectangleShape>>& grid) {
    std::vector<std::pair<int, int>> start_pos = {{28, 42}, {29, 42}, {30, 42}, {31, 42}, {32, 42}};
    for (const auto& s : start_pos) {
        grid[s.first][s.second].setFillColor(sf::Color::Cyan);
    }
}

void init_grid(std::vector<std::vector<sf::RectangleShape>>& grid) {
    for (size_t i = 0; i < grid.size(); ++i) {
        for (size_t j = 0; j < grid[i].size(); ++j) {
            grid[i][j].setFillColor(sf::Color::Black);
            grid[i][j].setSize({cell_size, cell_size});
            grid[i][j].setPosition({static_cast<float>(j) * cell_size, static_cast<float>(i) * cell_size});
        }
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode({width, height}), "Game of life", sf::Style::None);
    window.setFramerateLimit(60);

    std::vector<std::vector<sf::RectangleShape>> grid(y_cells, std::vector<sf::RectangleShape>(x_cells));
    init_grid(grid);
    set_start_pos(grid);
    std::vector<std::vector<sf::RectangleShape>> next_grid(y_cells, std::vector<sf::RectangleShape>(x_cells));
    init_grid(next_grid);
    set_start_pos(next_grid);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        for (size_t i = 0; i < grid.size(); ++i) {
            for (size_t j = 0; j < grid[i].size(); ++j) {
                int living_cells = 0;

                // Top
                if (i - 1 >= 0) {
                    if (j - 1 >= 0 && grid[i - 1][j - 1].getFillColor() == sf::Color::Cyan) {
                        living_cells += 1;
                    }
                    if (grid[i - 1][j].getFillColor() == sf::Color::Cyan) {
                        living_cells += 1;
                    }
                    if (j + 1 < grid[i].size() && grid[i - 1][j + 1].getFillColor() == sf::Color::Cyan) {
                        living_cells += 1;
                    }
                }
                // Middle
                if (j - 1 >= 0 && grid[i][j - 1].getFillColor() == sf::Color::Cyan) {
                    living_cells += 1;
                }
                if (grid[i][j].getFillColor() == sf::Color::Cyan) {
                    living_cells += 1;
                }
                if (j + 1 < grid[i].size() && grid[i][j + 1].getFillColor() == sf::Color::Cyan) {
                    living_cells += 1;
                }
                // Bottom
                if (i + 1 < grid.size()) {
                    if (j - 1 >= 0 && grid[i + 1][j - 1].getFillColor() == sf::Color::Cyan) {
                        living_cells += 1;
                    }
                    if (grid[i + 1][j].getFillColor() == sf::Color::Cyan) {
                        living_cells += 1;
                    }
                    if (j + 1 < grid[i].size() && grid[i + 1][j + 1].getFillColor() == sf::Color::Cyan) {
                        living_cells += 1;
                    }
                }

                // Condition
                if (grid[i][j].getFillColor() == sf::Color::Black && living_cells == 3) {
                    next_grid[i][j].setFillColor(sf::Color::Cyan);
                }
                if (grid[i][j].getFillColor() == sf::Color::Cyan && living_cells < 2) {
                    next_grid[i][j].setFillColor(sf::Color::Black);
                }
                if (grid[i][j].getFillColor() == sf::Color::Cyan && living_cells > 3) {
                    next_grid[i][j].setFillColor(sf::Color::Black);
                }
            }
        }

        window.clear();
        for (const auto& s : grid) {
            for (const auto& j : s) {
                window.draw(j);
            }
        }
        window.display();

        std::this_thread::sleep_for(std::chrono::seconds(1));
        for (size_t i = 0; i < grid.size(); ++i) {
            for (size_t j = 0; j < grid[i].size(); ++j) {
                next_grid[i][j] = grid[i][j];
            }
        }
    }
}

