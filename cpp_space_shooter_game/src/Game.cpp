// Game.cpp
#include "Game.h"
#include <iostream>

#include <random> // for random color
#include <ctime> // enemy spawn 

Game::Game() : isRunning(false), window(nullptr), renderer(nullptr),
enemySpwanTimer(0), score(0), backgroundMusic(nullptr), laserSound(nullptr),
playerTexture(nullptr), backgroundTexture(nullptr), isGameOver(false),
font(nullptr), gameOverSound(nullptr), scoreTexture(nullptr)
{
    // Set initial player position and size
    player = { 300, 400, 50, 50 };  // x, y, width, height
    meteorTextures.fill(nullptr);    // Initialize all texture pointers to nullptr
}

// Destructor - Clean up all allocated resources
Game::~Game() {
    // Free audio resources
    if (backgroundMusic) Mix_FreeMusic(backgroundMusic);
    if (laserSound) Mix_FreeChunk(laserSound);
    if (gameOverSound) Mix_FreeChunk(gameOverSound);

    // Free font resources
    if (font) TTF_CloseFont(font);
    if (scoreTexture) SDL_DestroyTexture(scoreTexture);

    // Quit SDL subsystems
    Mix_CloseAudio();
    Mix_Quit();

    // Free texture resources
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyTexture(backgroundTexture);
    for (auto texture : meteorTextures) {
        if (texture) SDL_DestroyTexture(texture);
    }

    // Quit remaining SDL subsystems
    IMG_Quit();
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// Initialize game systems and resources
bool Game::init(const char* title, int x, int y, int w, int h) {
    // Initialize SDL with all subsystems
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cout << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return false;
    }

    // Create game window
    window = SDL_CreateWindow(title, x, y, w, h, SDL_WINDOW_SHOWN);
    if (!window) return false;

    // Create renderer for the window
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) return false;

    // Initialize image loading
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) return false;

    // Initialize audio system
    if (Mix_Init(MIX_INIT_MP3) == 0) return false;
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) return false;

    // Load audio files
    backgroundMusic = Mix_LoadMUS("assets/welcome.mp3");
    laserSound = Mix_LoadWAV("assets/laser.mp3");
    if (!backgroundMusic || !laserSound) return false;

    // Set music volume and start playing
    Mix_VolumeMusic(MIX_MAX_VOLUME / 3);
    Mix_PlayMusic(backgroundMusic, -1);  // -1 means loop indefinitely

    // Load game textures
    playerTexture = IMG_LoadTexture(renderer, "assets/playerShip.png");
    backgroundTexture = IMG_LoadTexture(renderer, "assets/black.png");
    if (!playerTexture || !backgroundTexture) return false;

    // Load meteor textures
    if (!loadTextures()) return false;

    // Initialize text rendering
    if (TTF_Init() == -1) return false;
    font = TTF_OpenFont("assets/font.otf", 28);
    if (!font) return false;

    // Load game over sound
    gameOverSound = Mix_LoadWAV("assets/gameOver.mp3");
    if (!gameOverSound) return false;

    // Initialize score display
    updateScoreTexture();

    isRunning = true;
    return true;
}

// Helper function to render text to the screen
void Game::renderText(const char* text, SDL_Color color, int x, int y, int* outWidth, int* outHeight) {
    // Create a surface from the text
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    if (!surface) {
        std::cout << "Failed to render text: " << TTF_GetError() << std::endl;
        return;
    }

    // Convert surface to texture for rendering
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        std::cout << "Failed to create texture: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(surface);
        return;
    }

    // Set up destination rectangle for rendering
    SDL_Rect rect = { x, y, surface->w, surface->h };
    if (outWidth) *outWidth = surface->w;
    if (outHeight) *outHeight = surface->h;

    // Render the text texture
    SDL_RenderCopy(renderer, texture, NULL, &rect);

    // Clean up resources
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

// Update the score display texture
void Game::updateScoreTexture() {
    std::string scoreText = "Score: " + std::to_string(score);
    SDL_Color color = { 255, 255, 255, 255 }; // White color

    // Position score in top-right corner
    scoreRect = { 650, 10, 0, 0 };
    renderText(scoreText.c_str(), color, scoreRect.x, scoreRect.y, &scoreRect.w, &scoreRect.h);
}

// Handle all input events
void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:  // Window close button clicked
            isRunning = false;
            break;

        case SDL_KEYDOWN:
            if (isGameOver) {
                // If game is over, space restarts the game
                if (event.key.keysym.sym == SDLK_SPACE) {
                    resetGame();
                }
            }
            else {
                // During gameplay, space creates a new bullet
                if (event.key.keysym.sym == SDLK_SPACE) {
                    SDL_Rect bullet = {
                        player.x + player.w / 2 - 2,  // Center bullet on player
                        player.y,
                        4,  // bullet width
                        10  // bullet height
                    };
                    Mix_PlayChannel(-1, laserSound, 0);  // Play sound effect
                    bullets.push_back(bullet);
                }
            }
            break;
        }
    }

    // Handle continuous keyboard input for player movement
    const Uint8* state = SDL_GetKeyboardState(nullptr);
    if (state[SDL_SCANCODE_A]) {
        player.x = std::max(-25, player.x - 5);  // Move left with boundary check
    }
    if (state[SDL_SCANCODE_D]) {
        player.x = std::min(775, player.x + 5);  // Move right with boundary check
    }
    if (state[SDL_SCANCODE_W]) {
        player.y = std::max(0, player.y - 5);    // Move up with boundary check
    }
    if (state[SDL_SCANCODE_S]) {
        player.y = std::min(550, player.y + 5);  // Move down with boundary check
    }
}

// Reset game state for new game
void Game::resetGame() {
    isGameOver = false;
    score = 0;
    player = { 300, 400, 50, 50 };  // Reset player position
    bullets.clear();                 // Remove all bullets
    enemies.clear();                 // Remove all enemies
    enemySpwanTimer = 0;
    Mix_PlayMusic(backgroundMusic, -1);  // Restart background music
}

// Load meteor textures
bool Game::loadTextures() {
    const char* meteorPaths[] = {
        "assets/meteorBrown_med1.png",
        "assets/meteorGrey_med1.png",
    };

    // Load each meteor texture
    for (int i = 0; i < 2; i++) {
        meteorTextures[i] = IMG_LoadTexture(renderer, meteorPaths[i]);
        if (!meteorTextures[i]) {
            std::cout << "Failed to load meteor texture" << i << ": " << IMG_GetError() << std::endl;
            return false;
        }
    }
    return true;
}

// Create a new enemy with random properties
void Game::spawnEnemy() {
    // Set up random number generators
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> xDist(0, 800);  // Random x position
    static std::uniform_real_distribution<> scaleDist(0.5f, 1.5f);  // Random size
    static std::uniform_real_distribution<> speedDist(1.0f, 3.0f);  // Random speed
    static std::uniform_int_distribution<> textureDist(0, 1);  // Random meteor texture

    Enemy enemy;
    enemy.x = xDist(gen);
    enemy.y = -50;  // Start above screen
    enemy.scale = scaleDist(gen);
    enemy.speed = speedDist(gen);

    // Select random meteor texture
    int textureIndex = textureDist(gen);
    enemy.texture = meteorTextures[textureIndex];

    // Get texture size and set rectangle size based on scale
    int textureW, textureH;
    SDL_QueryTexture(enemy.texture, nullptr, nullptr, &textureW, &textureH);
    enemy.rect.w = static_cast<int>(textureW * enemy.scale);
    enemy.rect.h = static_cast<int>(textureH * enemy.scale);
    enemy.rect.x = static_cast<int>(enemy.x);
    enemy.rect.y = static_cast<int>(enemy.y);

    enemies.push_back(enemy);
}

// Check collision between rectangle and enemy
bool Game::checkCollision(const SDL_Rect& rect, const Enemy& enemy) {
    return SDL_HasIntersection(&rect, &enemy.rect);
}

// Check all game collisions
void Game::checkCollisions() {
    if (isGameOver) return;

    // Check bullet-enemy collisions
    for (auto bulletIt = bullets.begin(); bulletIt != bullets.end();) {
        bool bulletHit = false;

        for (auto enemyIt = enemies.begin(); enemyIt != enemies.end();) {
            if (checkCollision(*bulletIt, *enemyIt)) {
                // Collision detected - increase score and remove both objects
                score += 50;
                enemyIt = enemies.erase(enemyIt);
                bulletHit = true;
                break;
            }
            else {
                ++enemyIt;
            }
        }

        if (bulletHit) {
            bulletIt = bullets.erase(bulletIt);
        }
        else {
            ++bulletIt;
        }
    }

    // Check player-enemy collisions
    for (const auto& enemy : enemies) {
        if (checkCollision(player, enemy)) {
            if (!isGameOver) {
                isGameOver = true;
                Mix_HaltMusic();
                Mix_PlayChannel(-1, gameOverSound, 0);
            }
            break;
        }
    }
}

// Update game state
void Game::update() {
    if (isGameOver) return;

    // Update bullets
    for (auto it = bullets.begin(); it != bullets.end();) {
        it->y -= 7;  // Move bullet up
        if (it->y < 0) {  // Remove if off screen
            it = bullets.erase(it);
        }
        else {
            ++it;
        }
    }

    // Spawn enemies periodically
    enemySpwanTimer++;
    if (enemySpwanTimer > 40) {  // Spawn enemy every 40 frames
        spawnEnemy();
        enemySpwanTimer = 0;
    }

    // Update enemy positions
    for (auto it = enemies.begin(); it != enemies.end();) {
        it->y += it->speed;  // Move enemy down at its speed
        it->rect.x = static_cast<int>(it->x);
        it->rect.y = static_cast<int>(it->y);

        if (it->y > 620) {  // Remove if off screen
            it = enemies.erase(it);
        }
        else {
            ++it;
        }
    }

    checkCollisions();
}

// Render game state
void Game::render() {
    // Clear screen with black background
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (!isGameOver) {
        // Draw background and player
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
        SDL_RenderCopy(renderer, playerTexture, NULL, &player);

        // Draw bullets
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Red color
        for (const auto& bullet : bullets) {
            SDL_RenderFillRect(renderer, &bullet);
        }

        // Draw enemies
        for (const auto& enemy : enemies) {
            SDL_RenderCopy(renderer, enemy.texture, NULL, &enemy.rect);
        }

        // Draw score
        SDL_Color white = { 255, 255, 255, 255 };
        std::string scoreText = "Score: " + std::to_string(score);
        renderText(scoreText.c_str(), white, 650, 10, nullptr, nullptr);
    }
    else {
        // Game over screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_Color red = { 255, 0, 0, 255 };
        SDL_Color white = { 255, 255, 255, 255 };

        renderText("GAME OVER", red, 320, 200, nullptr, nullptr);
        std::string finalScore = "Final Score: " + std::to_string(score);
        renderText(finalScore.c_str(), white, 300, 300, nullptr, nullptr);
        renderText("Press SPACE to restart", white, 250, 400, nullptr, nullptr);
    }

    // Update screen
    SDL_RenderPresent(renderer);
}