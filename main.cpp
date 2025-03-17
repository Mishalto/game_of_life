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

void calc_next_grid(std::vector<std::vector<sf::RectangleShape>>& grid, std::vector<std::vector<sf::RectangleShape>>& next_grid) {
    for (int i = 0; i < static_cast<int>(grid.size()); ++i) {
        for (int j = 0; j < static_cast<int>(grid[i].size()); ++j) {
            int living_cells = 0;
            // Top
            if (i - 1 >= 0) {
                if (j - 1 >= 0 && grid[i - 1][j - 1].getFillColor() == sf::Color::Cyan) {
                    living_cells += 1;
                }
                if (grid[i - 1][j].getFillColor() == sf::Color::Cyan) {
                    living_cells += 1;
                }
                if (j + 1 < static_cast<int>(grid[i].size()) && grid[i - 1][j + 1].getFillColor() == sf::Color::Cyan) {
                    living_cells += 1;
                }
            }
            // Middle
            if (j - 1 >= 0 && grid[i][j - 1].getFillColor() == sf::Color::Cyan) {
                living_cells += 1;
            }
            if (j + 1 < static_cast<int>(grid[i].size()) && grid[i][j + 1].getFillColor() == sf::Color::Cyan) {
                living_cells += 1;
            }
            // Bottom
            if (i + 1 < static_cast<int>(grid.size())) {
                if (j - 1 >= 0 && grid[i + 1][j - 1].getFillColor() == sf::Color::Cyan) {
                    living_cells += 1;
                }
                if (grid[i + 1][j].getFillColor() == sf::Color::Cyan) {
                    living_cells += 1;
                }
                if (j + 1 < static_cast<int>(grid[i].size()) && grid[i + 1][j + 1].getFillColor() == sf::Color::Cyan) {
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
}

int main() {
    sf::RenderWindow window(sf::VideoMode({width, height}), "Game of life", sf::Style::None);

    std::vector<std::vector<sf::RectangleShape>> grid(y_cells, std::vector<sf::RectangleShape>(x_cells));
    init_grid(grid);
    set_start_pos(grid);

    std::vector<std::vector<sf::RectangleShape>> next_grid(y_cells, std::vector<sf::RectangleShape>(x_cells));
    init_grid(next_grid);
    set_start_pos(next_grid);

    auto last_time = std::chrono::high_resolution_clock::now();
    int frame_count = 0;
    float fps = 0.0f;

    while (window.isOpen()) {
        auto start_time = std::chrono::high_resolution_clock::now();
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        calc_next_grid(grid, next_grid);

        window.clear();
        for (const auto& s : grid) {
            for (const auto& j : s) {
                window.draw(j);
            }
        }
        window.display();

        for (size_t i = 0; i < grid.size(); ++i) {
            for (size_t j = 0; j < grid[i].size(); ++j) {
                grid = next_grid;
            }
        }

        frame_count++;
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> duration = end_time - start_time;

        if (std::chrono::duration<float>(end_time - last_time).count() >= 1.0f) {
            fps = frame_count;
            frame_count = 0;
            last_time = end_time;
            std::cout << "FPS: " << fps << std::endl;
        }
    }

    return 0;
}

