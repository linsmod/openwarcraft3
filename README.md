# OpenWarcraft3

Open source re-implementation of Warcraft III written in C.

<img width="1025" alt="Screenshot 2023-05-25 at 09 57 57" src="https://github.com/corepunch/openwarcraft3/assets/83646194/643c7aa7-2b91-469c-857e-0f6910c939af">

<img width="1026" alt="Screenshot 2023-05-25 at 09 58 15" src="https://github.com/corepunch/openwarcraft3/assets/83646194/a79e447d-e42c-4468-b4ca-3d212efe346a">

## Project Overview

OpenWarcraft3 is a comprehensive open-source reimplementation of Warcraft III, aiming to recreate the game's functionality while maintaining compatibility with original game assets. The project is written in C and implements most of the core game systems.

## Project Architecture

The project follows a classic game engine architecture with the following core components:

### Core Components

1. **Main Entry Point** (`src/common/main.c`)
   - Initializes the game
   - Loads maps
   - Implements the main game loop (server and client frames)

2. **Server** (`src/server/sv_main.c`)
   - Handles network messages
   - Loads and manages models
   - Runs game frames
   - Processes configuration strings

3. **Client** (`src/client/cl_main.c`)
   - Initializes the client
   - Handles network messages
   - Runs client frames
   - Processes user input
   - Prepares and updates the screen

4. **Renderer** (`src/renderer/r_main.c`)
   - Initializes OpenGL and SDL
   - Loads textures and models
   - Sets up viewport and scissor regions
   - Renders shadow maps and scenes
   - Handles frame buffers

5. **Map System** (`src/common/world.c`)
   - Reads map information
   - Loads heightmaps
   - Processes doodads and units
   - Reads string tables
   - Loads map scripts

6. **JASS Script VM** (`src/game/jass/vm_main.c`)
   - Implements variable and type systems
   - Handles function calls
   - Implements operators
   - Manages stack operations
   - Handles control flow
   - Supports threading

7. **Game Logic** (`src/game/g_main.c`)
   - Initializes the game
   - Runs game frames
   - Manages clients
   - Implements event systems
   - Provides player and entity lookup functions

## Dependencies

The project uses the following external libraries:
- **StormLib**: For reading Warcraft III MPQ files
- **SDL**: For cross-platform window management and input handling
- **OpenGL**: For graphics rendering

## Getting Started

### Cloning

```bash
git clone --recurse-submodules git@github.com:corepunch/openwarcraft3.git
```

### Creating Project Files

#### Windows
```bash
tools\bin\windows\premake5.exe vs2022
```

#### macOS
```bash
tools/bin/darwin/premake5 xcode4
```

or

```bash
tools/bin/darwin/premake5 --cc=clang gmake
```

#### Linux
```bash
tools/bin/linux/premake5 gmake
```

The project files will be created in the build folder.

### Building

After generating the project files, you can build the project using the corresponding IDE or build system:

- **Visual Studio**: Open the solution in the build folder and build
- **Xcode**: Open the Xcode project in the build folder and build
- **Make**: Run `make` in the build folder

## Contributing

Contributions to OpenWarcraft3 are welcome! Here are some ways you can contribute:

1. **Bug Reports**: Report bugs and issues
2. **Feature Requests**: Suggest new features or improvements
3. **Code Contributions**: Submit pull requests with bug fixes or new features
4. **Documentation**: Improve or add documentation

Before contributing, please:
- Check existing issues and pull requests
- Follow the existing code style
- Write clear commit messages
- Add tests for new features when possible

## License

[Add license information here]

## Acknowledgements

- The original Warcraft III game and Blizzard Entertainment for creating such an iconic game
- All contributors to the OpenWarcraft3 project