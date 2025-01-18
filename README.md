# Space Shooter Game

A simple space shooter game built with C++ and SDL2. This was my first C++ project, coming from a Flutter development background.

![Gameplay Demo](gameplay.gif)

## 🎮 Gameplay

In this space shooter game, you control a spaceship and defend against incoming meteors. The game features:

- Player movement using WASD keys
- Shooting mechanics with spacebar
- Different types of meteors with varying sizes and speeds
- Score tracking system
- Background music and sound effects
- Game over state with restart option

[Watch Full Gameplay](gameplay.mp4)

## 🛠️ Built With

* C++
* SDL2 Libraries:
  * SDL2 - Base library
  * SDL2_image - Image loading
  * SDL2_mixer - Audio handling
  * SDL2_ttf - Text rendering

## 📁 Project Structure

```
SpaceShooter/
├── assets/
│   ├── playerShip.png
│   ├── meteorBrown_med1.png
│   ├── meteorGrey_med1.png
│   ├── black.png
│   ├── welcome.mp3
│   ├── laser.mp3
│   ├── gameOver.mp3
│   └── font.otf
├── SDL2/
│   └── [SDL2 library files]
├── src/
│   ├── main.cpp
│   ├── Game.cpp
│   └── Game.h
└── README.md
```

## 🚀 Getting Started

### Prerequisites

* A C++ compiler (e.g., MSVC, GCC, Clang)
* SDL2 development libraries (included in the project)

### Installation

1. Clone the repository
```bash
git clone https://github.com/yourusername/space-shooter.git
```

2. Open the project in your preferred IDE (e.g., Visual Studio)

3. Build and run the project
   - The SDL2 libraries are already included in the project
   - No additional configuration should be needed

### Controls

- `W` - Move Up
- `S` - Move Down
- `A` - Move Left
- `D` - Move Right
- `Spacebar` - Shoot
- `Spacebar` - Restart (when game over)

## 🎯 Game Features

- **Player Movement**: Smooth control with WASD keys
- **Shooting Mechanism**: Fire lasers with spacebar
- **Enemy System**: Random meteor spawning with varying:
  - Sizes
  - Speeds
  - Textures
- **Score System**: Earn points by destroying meteors
- **Sound Effects**:
  - Background music
  - Laser shooting sounds
  - Game over sound
- **Game States**:
  - Active gameplay
  - Game over screen with score display
  - Restart capability

## 🎨 Assets Credits

- Game assets from [Kenney.nl](https://kenney.nl/) (Space Shooter Redux)

## 📝 Learning Notes

This project served as my introduction to:
- C++ programming
- Game development concepts
- SDL2 library usage
- Real-time rendering
- Collision detection
- Game state management
- Resource management in C++


## 📄 License

This project is licensed under the MIT License - see the LICENSE file for details.

## 🤝 Acknowledgments

* SDL2 development team
* Kenney.nl for the game assets
* C++ and SDL2 community for learning resources
