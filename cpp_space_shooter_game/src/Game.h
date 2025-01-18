// Game.h
#pragma once
#include <SDL.h>
#include <SDL_mixer.h>  // For audio functionality
#include <SDL_image.h>  // For loading images
#include <SDL_ttf.h>    // For rendering text
#include <string>
#include <memory>       // For smart pointers
#include <vector>       // For dynamic arrays
#include <array>        // For fixed-size arrays

class Game {
private:
    bool isRunning;              // Controls the main game loop
    SDL_Window* window;          // Main game window
    SDL_Renderer* renderer;      // Renders graphics to the window

    // Audio components
    Mix_Music* backgroundMusic;  // Background music that loops
    Mix_Chunk* laserSound;       // Sound effect for shooting
    Mix_Chunk* gameOverSound;    // Sound effect for game over

    // Texture components
    SDL_Texture* playerTexture;      // Player ship image
    SDL_Texture* backgroundTexture;  // Background image
    std::array<SDL_Texture*, 2> meteorTextures;  // Array of meteor textures for variety
    SDL_Texture* scoreTexture;       // Texture for score display

    // Game objects
    SDL_Rect player;               // Player position and size
    std::vector<SDL_Rect> bullets; // Dynamic array of bullet positions
    SDL_Rect scoreRect;            // Position and size of score display

    TTF_Font* font;                // Font for text rendering

    // Helper functions
    void renderText(const char* text, SDL_Color color, int x, int y, int* outWidth = nullptr, int* outHeight = nullptr);
    void updateScoreTexture();

    // Enemy structure definition
    struct Enemy {
        float x, y;           // Position with floating-point precision for smooth movement
        SDL_Rect rect;        // Rectangle for collision detection and rendering
        SDL_Texture* texture; // Which meteor texture to use
        float scale;          // Size multiplier for variety
        float speed;          // Movement speed
    };

    std::vector<Enemy> enemies;    // Dynamic array of active enemies
    int enemySpwanTimer;           // Controls enemy spawn rate
    int score;                     // Player's current score
    bool isGameOver;               // Game state flag

public:
    Game();    // Constructor
    ~Game();   // Destructor - cleans up resources

    // Core game functions
    bool init(const char* title, int x, int y, int w, int h);  // Initialize game
    void handleEvents();   // Process input
    void update();         // Update game state
    void render();         // Draw game state
    bool running() const { return isRunning; }  // Check if game should continue

    // Game mechanics
    void spawnEnemy();     // Create new enemy
    bool checkCollision(const SDL_Rect& rect, const Enemy& enemy);  // Collision detection
    void checkCollisions();  // Check all game object collisions
    bool loadTextures();     // Load game textures
    void resetGame();        // Reset game state
};

// Game.cpp comments will continue in next section due to length...