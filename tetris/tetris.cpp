#include <SFML/Graphics.hpp>

constexpr int blockSize = 32;
constexpr int gridWidth = 10, gridHeight = 20;
constexpr int windowWidth = gridWidth * blockSize, windowHeight = gridHeight * blockSize;

// Each tetromino is in a 4x4 grid
// The 4x4 grid can be represent as a 16 bit integer
// For example, the I tetromino has the following directions:
//
//     UP           RIGHT         DOWN          LEFT
//
// 0  1  0  0    0  0  0  0    0  0  1  0    0  0  0  0
// 0  1  0  0    1  1  1  1    0  0  1  0    0  0  0  0
// 0  1  0  0    0  0  0  0    0  0  1  0    1  1  1  1
// 0  1  0  0    0  0  0  0    0  0  1  0    0  0  0  0
//
// UP     0100010001000100  0x4444
// RIGHT  0000111100000000  0x0F00
// DOWN   0010001000100010  0x2222
// LEFT   0000000011110000  0x00F0
//
// Tetrominos has different centre of rotation
// So each one needs to be hard coded

const uint16_t tetrominoes[7][4] = {
    //  UP     RIGHT   DOWN    LEFT
    //  0       1       2       3
    {0x4444, 0x0F00, 0x2222, 0x00F0}, // 0 I
    {0x44C0, 0x8E00, 0x6440, 0x0E20}, // 1 J
    {0x4460, 0x0E80, 0xC440, 0x2E00}, // 2 L
    {0xCC00, 0xCC00, 0xCC00, 0xCC00}, // 3 O
    {0x06C0, 0x8C40, 0x6C00, 0x4620}, // 4 S
    {0x0E40, 0x4C40, 0x4E00, 0x4640}, // 5 T
    {0x0C60, 0x4C80, 0xC600, 0x2640}, // 6 Z
};

// Each cell in the 4x4 grid which contain the tetromino is either occupied or not
bool cellIsOccupied(const uint16_t tetrominoWithDirection, int cellIndex)
{
    int bitIndex = 15 - cellIndex;
    return (tetrominoWithDirection & (1 << bitIndex)) >> bitIndex;
}

// Tetromino's index match the color in tetures/blocks.png
// [0, 1, 2, 3, 4, 5, 6]
// [I, J, L, O, S, T, Z]
void drawBlock(const sf::Texture &blockTextures,
               const sf::Vector2f &position,
               int colorIndex,
               sf::RenderWindow *window)
{
    sf::Sprite blockSprite(blockTextures);
    blockSprite.setTextureRect(sf::IntRect(blockSize * colorIndex, 0, // Offset
                                           blockSize, blockSize));   // Size
    blockSprite.setPosition(position);
    window->draw(blockSprite);
}

sf::Vector2f getBlockPosition(const sf::Vector2f &tetrominoPosition,
                              int blockIndexInTetromino)
{
    sf::Vector2f blockPosition;
    blockPosition.x = tetrominoPosition.x + (blockIndexInTetromino % 4) * blockSize;
    blockPosition.y = tetrominoPosition.x + (blockIndexInTetromino / 4) * blockSize;
    return blockPosition;
}

void drawTetromino(const sf::Texture &blockTexture,
                   const sf::Vector2f &position,
                   int tetromino, int direction,
                   sf::RenderWindow *window)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            int blockIndex = i * 4 + j;

            if (cellIsOccupied(tetrominoes[tetromino][direction], blockIndex))
            {
                drawBlock(blockTexture, getBlockPosition(position, blockIndex),
                          tetromino, window);
            }
        }
    }
}

int main()
{
    // Declare and create a new window
    sf::RenderWindow mainWindow(sf::VideoMode(windowWidth, windowHeight),
                                "Tetris",
                                sf::Style::Close | sf::Style::Titlebar);

    // Limit the framerate to 60 frames per second
    mainWindow.setFramerateLimit(60);

    sf::Texture blockTextures;
    blockTextures.loadFromFile("textures/blocks.png");

    // The main loop - ends as soon as the window is closed
    while (mainWindow.isOpen())
    {
        // Event processing
        sf::Event windowEvent;

        while (mainWindow.pollEvent(windowEvent))
        {
            // Request for closing the window
            if (windowEvent.type == windowEvent.Closed)
            {
                mainWindow.close();
            }
        }
        
        // Activate the window for OpenGL rendering
        mainWindow.setActive();
        
        int x = 0, y = 0;
        sf::Vector2f position(x, y);

        mainWindow.clear(sf::Color::White);
        drawTetromino(blockTextures, position, 6, 0, &mainWindow);
        mainWindow.display();
    }

    return 0;
}
