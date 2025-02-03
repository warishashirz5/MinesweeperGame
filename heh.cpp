#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>

class Minesweeper {
private:
    int gridSize;
    int cellSize;
    int bombCount;
    std::vector<std::vector<int> > grid;
    std::vector<std::vector<bool> > revealed;
    bool gameOver;
    sf::Font font;
    sf::Texture bg_texture;
	sf::Sprite bg;
public:
    Minesweeper(int size, int bombs)
        : gridSize(size), bombCount(bombs), gameOver(false), cellSize(40) {
        	bg_texture.loadFromFile("bg.jpg");
        	bg.setTexture(bg_texture);
        grid.resize(gridSize, std::vector<int>(gridSize, 0));
        revealed.resize(gridSize, std::vector<bool>(gridSize, false));
        if (!font.loadFromFile("arial.ttf")) {
            std::cerr << "Failed to load font!" << std::endl;
        }
        generateBombs();
    }

    void generateBombs() {
        std::srand(static_cast<unsigned>(std::time(NULL)));
        for (int i = 0; i < bombCount; ++i) {
            int x, y;
            do {
                x = std::rand() % gridSize;
                y = std::rand() % gridSize;
            } while (grid[y][x] == -1);
            grid[y][x] = -1;
            // Update numbers around the bomb
            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    int nx = x + dx, ny = y + dy;
                    if (nx >= 0 && ny >= 0 && nx < gridSize && ny < gridSize && grid[ny][nx] != -1) {
                        grid[ny][nx]++;
                    }
                }
            }
        }
    }

    void handleClick(int x, int y) {
        if (grid[y][x] == -1) {
            gameOver = true;
        } else {
            revealCell(x, y);
        }
    }

    void revealCell(int x, int y) {
        if (x < 0 || y < 0 || x >= gridSize || y >= gridSize || revealed[y][x]) return;
        revealed[y][x] = true;
        if (grid[y][x] == 0) {
            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    revealCell(x + dx, y + dy);
                }
            }
        }
    }

    void draw(sf::RenderWindow& window) {
        for (int y = 0; y < gridSize; ++y) {
            for (int x = 0; x < gridSize; ++x) {
                sf::RectangleShape cell(sf::Vector2f(cellSize - 4, cellSize - 4));
                cell.setPosition(x * cellSize, y * cellSize);

                if (revealed[y][x]) {
                    if (grid[y][x] == -1) {
                        cell.setFillColor(sf::Color::Red); // Bomb
                    } else {
                        cell.setFillColor(sf::Color::Transparent); // Revealed cells with black background
                        if (grid[y][x] > 0) {
                            sf::Text number;
                            number.setFont(font);
                            number.setString(intToString(grid[y][x])); // Display the number
                            number.setCharacterSize(24); // Font size adjusted
                            number.setFillColor(sf::Color::White); // White number for contrast

                            // Center the text in the middle of the cell
                            sf::FloatRect textBounds = number.getLocalBounds();
                            number.setOrigin(textBounds.left + textBounds.width / 2.0f,
                                             textBounds.top + textBounds.height / 2.0f);
                            number.setPosition(x * cellSize + cellSize / 2, y * cellSize + cellSize / 2); // Centered position
                            window.draw(number);
                        }
                    }
                } else {
                    cell.setFillColor(sf::Color(100, 200, 100)); // Lighter green for covered cells
                }

                window.draw(cell);
            }
        }
    }

    bool isGameOver() const { return gameOver; }

    void reset(int size, int bombs) {
        gridSize = size;
        bombCount = bombs;
        grid.assign(gridSize, std::vector<int>(gridSize, 0));
        revealed.assign(gridSize, std::vector<bool>(gridSize, false));
        gameOver = false;
        generateBombs();
    }

    int getGridSize() const { return gridSize; }
    int getBombCount() const { return bombCount; }

private:
    // Helper function to convert integers to strings
    std::string intToString(int value) {
        std::stringstream ss;
        ss << value;
        return ss.str();
    }
};

// Draw menu with difficulty level options
void drawMenu(sf::RenderWindow& window, sf::Font& font, sf::RectangleShape& lowButton, sf::RectangleShape& mediumButton, sf::RectangleShape& highButton, sf::Sprite& bg) {
    // Window dimensions
    sf::Vector2u windowSize = window.getSize();
    float windowWidth = windowSize.x;
    float windowHeight = windowSize.y;

    // Title
    sf::Text title("Choose Difficulty", font, 36);
    title.setFillColor(sf::Color::White);
    title.setPosition(windowWidth / 2 - title.getGlobalBounds().width / 2, 50);

    // Button dimensions
    sf::Vector2f buttonSize(300, 50);
    float buttonX = windowWidth / 2 - buttonSize.x / 2;

    // Low difficulty button
    lowButton.setSize(buttonSize);
    lowButton.setFillColor(sf::Color::White);
    lowButton.setPosition(buttonX, windowHeight / 2 - 150);

    sf::Text lowText("Low (10x10)", font, 24);
    lowText.setFillColor(sf::Color::Black);
    lowText.setPosition(lowButton.getPosition().x + buttonSize.x / 2 - lowText.getGlobalBounds().width / 2,
                        lowButton.getPosition().y + buttonSize.y / 4);

    // Medium difficulty button
    mediumButton.setSize(buttonSize);
    mediumButton.setFillColor(sf::Color::White);
    mediumButton.setPosition(buttonX, windowHeight / 2 - 50);

    sf::Text mediumText("Medium (20x20)", font, 24);
    mediumText.setFillColor(sf::Color::Black);
    mediumText.setPosition(mediumButton.getPosition().x + buttonSize.x / 2 - mediumText.getGlobalBounds().width / 2,
                           mediumButton.getPosition().y + buttonSize.y / 4);

    // High difficulty button
    highButton.setSize(buttonSize);
    highButton.setFillColor(sf::Color::White);
    highButton.setPosition(buttonX, windowHeight / 2 + 50);

    sf::Text highText("High (30x30)", font, 24);
    highText.setFillColor(sf::Color::Black);
    highText.setPosition(highButton.getPosition().x + buttonSize.x / 2 - highText.getGlobalBounds().width / 2,
                         highButton.getPosition().y + buttonSize.y / 4);

    // Draw everything
    window.clear(sf::Color::Black);
    window.draw(bg); // Draw background sprite
    window.draw(title);
    window.draw(lowButton);
    window.draw(lowText);
    window.draw(mediumButton);
    window.draw(mediumText);
    window.draw(highButton);
    window.draw(highText);
    window.display();
}



// Draw game over screen with restart/quit options
void drawGameOverScreen(sf::RenderWindow& window, sf::Font& font, sf::RectangleShape& restartButton, sf::RectangleShape& quitButton) {
    // Title
    sf::Text gameOverText("Game Over!", font, 48);
    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setPosition(300, 100);

    // Restart button
    restartButton.setSize(sf::Vector2f(200, 50));
    restartButton.setFillColor(sf::Color::White);
    restartButton.setPosition(300, 300);

    sf::Text restartText("Restart.", font, 26);
    restartText.setFillColor(sf::Color::Black);
    restartText.setPosition(340, 310);

    // Quit button
    quitButton.setSize(sf::Vector2f(200, 50));
    quitButton.setFillColor(sf::Color::Red);
    quitButton.setPosition(305, 400);

    sf::Text quitText("Quit.", font, 26);
    quitText.setFillColor(sf::Color::Black);
    quitText.setPosition(380, 410);

    // Draw everything
    window.clear(sf::Color::Black);
    window.draw(gameOverText);
    window.draw(restartButton);
    window.draw(restartText);
    window.draw(quitButton);
    window.draw(quitText);
    window.display();
}

int main() {
    sf::RenderWindow window(sf::VideoMode(900, 600), "Minesweeper");
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
        return -1;
    }

    // Load the background texture and sprite
    sf::Texture bg_texture;
    if (!bg_texture.loadFromFile("bg.jpg")) {
        std::cerr << "Failed to load background image!" << std::endl;
        return -1;
    }
    sf::Sprite bg(bg_texture);

    sf::RectangleShape lowButton, mediumButton, highButton;
    sf::RectangleShape restartButton, quitButton;

    int gridSize = 10, bombCount = 10;
    bool gameStarted = false;

    Minesweeper game(gridSize, bombCount);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return 0;
            }

            if (!gameStarted) {
                // Difficulty selection logic
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                        if (lowButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                            gridSize = 10;
                            bombCount = 10;
                            game.reset(gridSize, bombCount);
                            gameStarted = true;
                        }
                        if (mediumButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                            gridSize = 20;
                            bombCount = 40;
                            game.reset(gridSize, bombCount);
                            gameStarted = true;
                        }
                        if (highButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                            gridSize = 30;
                            bombCount = 90;
                            game.reset(gridSize, bombCount);
                            gameStarted = true;
                        }
                    }
                }
            } else {
                // Game logic
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                        if (mousePos.x >= 0 && mousePos.x < game.getGridSize() * 40 &&
                            mousePos.y >= 0 && mousePos.y < game.getGridSize() * 40) {
                            int x = mousePos.x / 40;
                            int y = mousePos.y / 40;
                            game.handleClick(x, y);
                        }
                    }
                }
            }

            if (game.isGameOver()) {
                drawGameOverScreen(window, font, restartButton, quitButton);
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                        if (restartButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                            game.reset(gridSize, bombCount);
                            gameStarted = true;
                        }
                        if (quitButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                            window.close();
                        }
                    }
                }
            } else {
                window.clear();
                game.draw(window);
                window.display();
            }
        }

        if (!gameStarted) {
            drawMenu(window, font, lowButton, mediumButton, highButton, bg);
        }
    }

    return 0;
}




