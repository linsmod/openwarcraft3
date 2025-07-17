# OpenWarcraft3 Documentation

## Project Overview

OpenWarcraft3 is a complete open-source reimplementation of Warcraft III: Reign of Chaos and The Frozen Throne, written in C. The project aims to recreate the original game's functionality while providing modern enhancements and cross-platform compatibility.

## Architecture

The project follows a modular client-server architecture, even for single-player games, which enables seamless multiplayer support. The codebase is organized into distinct layers:

### Core Architecture Layers

```
┌─────────────────────────────────────────┐
│              Application Layer          │
├─────────────────────────────────────────┤
│  Game Logic  │  Rendering  │  Network  │
├─────────────────────────────────────────┤
│    Engine Core (Common Utilities)       │
├─────────────────────────────────────────┤
│        Platform Abstraction             │
└─────────────────────────────────────────┘
```

## Directory Structure

### `/src/common/` - Core Engine
- **main.c**: Application entry point and main game loop
- **net.c**: Network layer implementation (UDP sockets, packet handling)
- **msg.c**: Message serialization/deserialization protocol
- **world.c**: Map loading and world management
- **cmd.c**: Console command system
- **parser.c**: Configuration file parsing

### `/src/server/` - Server Logic
- **sv_main.c**: Server core (game loop, client management)
- **sv_game.c**: Game-specific server logic
- **sv_parse.c**: Server-side message parsing
- **sv_ents.c**: Entity state management
- **sv_user.c**: User command processing
- **sv_world.c**: Server-side world simulation

### `/src/client/` - Client Logic
- **cl_main.c**: Client core (rendering loop, input handling)
- **cl_parse.c**: Client-side message parsing
- **cl_input.c**: Input processing (keyboard, mouse)
- **cl_view.c**: Camera and viewport management
- **cl_scrn.c**: Screen rendering coordination
- **cl_tent.c**: Temporary entity effects

### `/src/renderer/` - Graphics Engine
- **r_main.c**: Renderer initialization and main render loop
- **r_draw.c**: 2D drawing primitives
- **r_ents.c**: Entity rendering
- **r_texture.c**: Texture loading and management
- **r_shader.c**: Shader system
- **r_font.c**: Font rendering
- **r_fogofwar.c**: Fog of war implementation

### `/src/game/` - Game Logic
- **g_main.c**: Game initialization and main game loop
- **g_spawn.c**: Entity spawning system
- **g_phys.c**: Physics and collision detection
- **g_ai.c**: AI system for units
- **g_events.c**: Event system for triggers
- **g_commands.c**: Game-specific console commands
- **g_items.c**: Item and inventory system

### `/src/game/jass/` - Scripting Engine
- **vm_main.c**: JASS virtual machine core
- **vm_compiler.c**: JASS script compilation
- **jass_parser.c**: JASS language parser
- **vm_public.h**: JASS API definitions

### `/src/game/api/` - Game API
- **api_unit.h**: Unit manipulation API
- **api_player.h**: Player management API
- **api_group.h**: Unit group management
- **api_trigger.h**: Trigger system API
- **api_misc.h**: Miscellaneous game functions (including GameCache)

### `/src/game/skills/` - Unit Abilities
- **s_move.c**: Movement system
- **s_attack.c**: Combat system
- **s_build.c**: Building construction
- **s_train.c**: Unit training
- **s_harvest_lumber.c**: Resource gathering
- **s_goldmine.c**: Gold mining

## Key Systems

### 1. Entity System
The entity system manages all game objects (units, buildings, items):

```c
// Entity structure in g_local.h
typedef struct edict_s {
    entity_state_t s;        // Network state
    gclient_t *client;       // Player data if applicable
    game_import_t *gi;       // Engine interface
    // ... additional game-specific data
} edict_t;
```

### 2. JASS Scripting
Full JASS (Warcraft III scripting language) implementation:

- **Variables**: Integer, real, string, boolean, handle types
- **Functions**: Native and custom functions
- **Triggers**: Event-driven scripting
- **GameCache**: Persistent data storage between maps
- **Multithreading**: Cooperative multitasking for scripts

### 3. Physics System
Collision detection and movement:

- **Collision Types**: Circle, rectangle, polygon
- **Movement Types**: Ground, air, amphibious
- **Pathfinding**: A* algorithm with terrain analysis
- **Collision Resolution**: Separating axis theorem

### 4. Rendering Pipeline
OpenGL-based 3D rendering:

- **Forward Rendering**: Traditional pipeline
- **Shadow Mapping**: Dynamic shadows
- **Fog of War**: Visibility system
- **UI Rendering**: Immediate mode GUI
- **Particle Effects**: Sprite-based particles

### 5. Network Protocol
Custom UDP-based protocol:

- **Delta Compression**: Only changed data transmitted
- **Reliable/Unreliable**: Configurable message delivery
- **Entity Interpolation**: Smooth movement between updates
- **Lag Compensation**: Client-side prediction

### 6. Map System
Warcraft III map format support:

- **MPQ Archive**: StormLib integration for .w3x files
- **Terrain**: Heightmap-based terrain
- **Doodads**: Decorative objects
- **Units**: Pre-placed units and buildings
- **Triggers**: Map script loading

## Build System

### Build Methods
1. **CMake**: Primary build system (`CMakeLists.txt`)
2. **Premake**: Alternative build system (`premake5.lua`)
3. **Vendor Build**: Pre-built dependencies (`build_vendor.sh`)

### Dependencies
- **StormLib**: MPQ archive handling
- **SDL2**: Cross-platform window/input
- **OpenGL**: 3D graphics rendering
- **OpenAL**: 3D audio (optional)

## Configuration Files

### Game Data
- **war3skins.txt**: Unit appearance definitions
- **AbilityData.slk**: Ability parameters
- **ItemData.slk**: Item properties
- **UnitData.slk**: Unit statistics
- **SplatData.slk**: Terrain effects

### Map Files
- **.w3x**: Warcraft III map archives
- **war3map.j**: Map script file
- **war3map.w3e**: Terrain data
- **war3map.doo**: Doodad placement
- **war3mapUnits.doo**: Unit placement

## API Reference

### JASS Functions
The game implements the complete Warcraft III JASS API:

#### Unit API
- `CreateUnit()`, `RemoveUnit()`, `KillUnit()`
- `SetUnitPosition()`, `SetUnitFacing()`, `SetUnitOwner()`
- `GetUnitX()`, `GetUnitY()`, `GetUnitFacing()`

#### Player API
- `GetPlayerId()`, `GetPlayerName()`, `GetPlayerColor()`
- `SetPlayerHandicap()`, `SetPlayerTechResearched()`
- `GetPlayerState()`, `SetPlayerState()`

#### GameCache API
- `InitGameCache()`, `SaveGameCache()`
- `StoreInteger()`, `StoreReal()`, `StoreBoolean()`, `StoreUnit()`
- `GetStoredInteger()`, `GetStoredReal()`, `GetStoredBoolean()`
- `RestoreUnit()`

#### Trigger API
- `CreateTrigger()`, `DestroyTrigger()`
- `TriggerRegisterUnitEvent()`, `TriggerRegisterPlayerEvent()`
- `TriggerAddAction()`, `TriggerAddCondition()`

## Development Workflow

### Adding New Features
1. **Define API**: Add to appropriate header in `/src/game/api/`
2. **Implement Logic**: Add implementation in `/src/game/` or `/src/game/skills/`
3. **Register Functions**: Add to `jass_funcs[]` in `api_module.c`
4. **Test**: Use JASS scripts in test maps

### Debugging
- **Console Commands**: Built-in debug console
- **Logging**: `Com_Printf()` for debug output
- **Profiling**: Built-in performance counters
- **Network Debugging**: `net_showpackets`, `net_graph`

### Testing
- **Unit Tests**: Test individual components
- **Integration Tests**: Test complete game scenarios
- **Map Testing**: Use custom test maps
- **Network Testing**: Multiplayer stress testing

## Performance Considerations

### Memory Management
- **Entity Pool**: Pre-allocated entity array
- **String Pool**: Shared string storage
- **Texture Cache**: LRU texture management
- **Model Cache**: Shared model instances

### Optimization Techniques
- **Spatial Partitioning**: Grid-based entity lookup
- **LOD System**: Level-of-detail for distant objects
- **Frustum Culling**: Skip off-screen rendering
- **Batch Rendering**: Combine similar draw calls

## Cross-Platform Support

### Supported Platforms
- **Windows**: DirectX/OpenGL rendering
- **Linux**: OpenGL rendering, X11/Wayland
- **macOS**: OpenGL rendering, Cocoa
- **Mobile**: iOS/Android (experimental)

### Platform Abstraction
- **Rendering**: OpenGL abstraction layer
- **Input**: SDL2 input handling
- **Audio**: OpenAL/OpenSL ES
- **File System**: Cross-platform file I/O

## Future Enhancements

### Planned Features
- **Vulkan Renderer**: Modern graphics API support
- **Enhanced AI**: Machine learning-based AI
- **Mod Support**: Steam Workshop integration
- **Replay System**: Match recording/playback
- **Spectator Mode**: Live game viewing

### Technical Improvements
- **Multithreading**: Parallel entity processing
- **Physics Engine**: Bullet physics integration
- **Audio Engine**: 3D spatial audio
- **UI Framework**: Modern immediate-mode GUI

## Getting Started

### Building from Source
```bash
# Clone repository
git clone https://github.com/linsmod/openwarcraft3.git
cd openwarcraft3

# Build with CMake
mkdir build && cd build
cmake ..
make

# Run the game
./openwarcraft3
```

### Running the Game
```bash
# Basic launch
./openwarcraft3

# With specific map
./openwarcraft3 +map "maps/(2)PlunderIsle.w3x"

# Development mode
./openwarcraft3 +devmap "maps/(2)PlunderIsle.w3x" +developer 1
```

### Development Setup
1. **Install Dependencies**: SDL2, OpenGL, StormLib
2. **Configure IDE**: CMake integration recommended
3. **Set Up Assets**: Copy Warcraft III data files
4. **Test Maps**: Use provided test maps or create custom ones

## Contributing

### Code Style
- **C99 Standard**: Modern C features allowed
- **Hungarian Notation**: Prefixes for variable types
- **CamelCase**: Function and type names
- **Comments**: Doxygen-style documentation

### Testing Guidelines
- **Unit Tests**: Test individual functions
- **Integration Tests**: Test complete systems
- **Map Testing**: Verify with actual game maps
- **Performance Tests**: Benchmark critical paths

### Documentation
- **API Documentation**: Doxygen comments for public APIs
- **Protocol Documentation**: Keep PROTOCOL.md updated
- **Architecture Docs**: Document major design decisions
- **User Guides**: Help new contributors get started

This documentation provides a comprehensive overview of the OpenWarcraft3 project structure and implementation details. For specific questions, refer to the source code comments and inline documentation.
