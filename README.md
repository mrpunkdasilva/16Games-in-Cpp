# 16 Classic Games in C++

A collection of 16 classic games implemented in C++ using the SFML (Simple and Fast Multimedia Library) graphics framework.

## 🎮 Games Included

1. **Tetris** - The classic falling blocks puzzle game
2. **Doodle Jump** - Endless jumping platform game
3. **Arkanoid** - Break the bricks with a bouncing ball
4. **Snake** - Control a growing snake to eat food
5. **Minesweeper** - Find all mines without triggering them
6. **Fifteen Puzzle** - Slide numbered tiles to solve the puzzle
7. **Racing (Top Down)** - Arcade-style top-down racing game
8. **Outrun** - 3D perspective racing game
9. **Xonix** - Territory claiming puzzle game
10. **Bejeweled** - Match-3 puzzle game with gems
11. **NetWalk (Pipe Puzzle)** - Connect pipes to complete the network
12. **Mahjong Solitaire** - Traditional tile-matching solitaire
13. **Tron** - Light cycle racing game
14. **Chess** - Full implementation of the classic board game
15. **Volleyball** - Simple volleyball simulation
16. **Asteroids** - Space shooter avoiding and destroying asteroids

## 🛠️ Prerequisites

- **C++ Compiler** (supporting C++11 or later)
- **SFML Library** (Simple and Fast Multimedia Library)
- **CMake** (optional, for building)

### Installing SFML

#### Ubuntu/Debian:
```bash
sudo apt-get install libsfml-dev
```

#### macOS (with Homebrew):
```bash
brew install sfml
```

#### Windows:
- Download SFML from the [official website](https://www.sfml-dev.org/download.php)
- Follow the installation guide for your compiler

## 🚀 How to Build and Run

Each game is contained in its own directory and can be compiled independently.

### Using g++ directly:
```bash
cd "01 Tetris"
g++ -o tetris main.cpp -lsfml-graphics -lsfml-window -lsfml-system
./tetris
```

### Using CMake (if CMakeLists.txt is available):
```bash
mkdir build
cd build
cmake ..
make
```

## 🎯 Game Controls

Controls vary by game, but common patterns include:
- **Arrow Keys** - Movement/Navigation
- **Space** - Action/Shoot
- **Mouse** - Selection/Interaction
- **ESC** - Pause/Menu

Specific controls for each game are typically displayed in the game window or can be found in the source code comments.

## 📁 Project Structure

```
16Games-in-Cpp/
├── 01 Tetris/
│   ├── main.cpp
│   └── images/
├── 02 Doodle Jump/
│   ├── main.cpp
│   └── images/
├── ...
└── README.md
```

Each game directory contains:
- `main.cpp` - Main game source code
- `images/` - Game assets (sprites, textures)
- Additional headers/source files (where applicable)

## 🔧 Development Notes

- All games are implemented using **SFML** for graphics, audio, and input handling
- Games follow a similar structure with game loops, event handling, and rendering
- Most games include sprite-based graphics with image assets
- Some games may require additional files (fonts, sounds, etc.)

## 🤝 Contributing

Feel free to contribute by:
- Improving existing games
- Adding new features
- Fixing bugs
- Optimizing performance
- Adding sound effects or music

## 📝 License

This project is open source. Please check individual game files for specific licensing information.

## 🙏 Acknowledgments

- **SFML Team** for the excellent multimedia library
- Classic game developers for the original concepts
- Gaming community for inspiration and feedback

---

**Enjoy playing these classic games! 🎮**
