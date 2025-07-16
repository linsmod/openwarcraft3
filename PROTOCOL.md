# OpenWarcraft3 Client-Server Message Protocol Documentation

## Overview

The OpenWarcraft3 client-server protocol is a custom binary protocol designed for real-time strategy game communication. It uses a delta compression system to minimize bandwidth usage and supports both reliable and unreliable message delivery.

## Protocol Architecture

### Network Layer
- **Transport**: UDP-based with custom reliability layer
- **Max Message Size**: 256KB (`MAX_MSGLEN`)
- **Addressing**: IPv4/IPv6 support with port-based communication
- **Connection Types**: Loopback, Broadcast, and Direct IP

### Message Structure
All messages follow a packet-based structure with:
- **Message Type**: Single byte identifying the message type
- **Payload**: Variable-length binary data
- **Delta Compression**: Only changed fields are transmitted

## Message Types

### Server-to-Client Messages (SVC)

| Message ID | Name | Description |
|------------|------|-------------|
| `svc_bad` | Invalid | Reserved for error handling |
| `svc_temp_entity` | Temporary Entity | Short-lived visual effects |
| `svc_layout` | UI Layout | User interface updates |
| `svc_playerinfo` | Player Information | Player state updates |
| `svc_cursor` | Cursor Update | Mouse cursor state |
| `svc_configstring` | Config String | Static game configuration |
| `svc_spawnbaseline` | Spawn Baseline | Entity baseline data |
| `svc_packetentities` | Packet Entities | Entity state updates |
| `svc_frame` | Frame Update | Complete frame information |
| `svc_mirror` | Mirror Message | Echo client messages back |

### Client-to-Server Messages (CLC)

| Message ID | Name | Description |
|------------|------|-------------|
| `clc_bad` | Invalid | Reserved for error handling |
| `clc_move` | Move Command | Camera movement |
| `clc_stringcmd` | String Command | Console commands and actions |

## Data Serialization

### Basic Types
The protocol supports these fundamental data types:

- **Byte**: 8-bit unsigned integer
- **Short**: 16-bit signed integer
- **Long**: 32-bit signed integer
- **Float**: 32-bit IEEE floating point
- **String**: Null-terminated UTF-8 strings
- **Vector2**: Two floats (x, y)
- **Vector3**: Three shorts (x, y, z) with fixed-point precision
- **Quaternion**: Four shorts with normalized values

### Delta Compression System

#### Entity State Delta
Entity states use a bitfield-based delta compression:

```c
// Entity state fields tracked for delta compression
netField_t entityStateFields[] = {
    { "origin", NFT_VECTOR3 },      // Position
    { "angle", NFT_ANGLE },         // Rotation
    { "scale", NFT_PACKED_FLOAT },  // Scale factor
    { "frame", NFT_LONG },          // Animation frame
    { "model", NFT_SHORT },         // Model index
    { "player", NFT_BYTE },         // Owning player
    { "flags", NFT_LONG },          // Entity flags
    { "radius", NFT_ROUND },        // Collision radius
    { "stats", NFT_LONG },          // Game-specific stats
};
```

#### UI Frame Delta
UI elements use similar delta compression:

```c
netField_t uiFrameFields[] = {
    { "parent", NFT_SHORT },        // Parent frame
    { "points", NFT_LONG },         // Position data
    { "size", NFT_VECTOR2 },        // Width/height
    { "tex.index", NFT_LONG },      // Texture reference
    { "color", NFT_LONG },          // RGBA color
    { "text", NFT_TEXT },           // Display text
};
```

#### Player State Delta
Player-specific state:

```c
netField_t playerStateFields[] = {
    { "viewquat", NFT_QUATERNION }, // Camera rotation
    { "origin", NFT_VECTOR2 },      // Camera position
    { "fov", NFT_BYTE },            // Field of view
    { "distance", NFT_ROUND },      // Camera distance
    { "stats", NFT_LONG },          // Player statistics
    { "texts", NFT_DUPTEXT },       // UI text strings
};
```

## Message Flow

### Connection Establishment
1. Client initiates connection to server
2. Server sends initial game state via `svc_configstring` messages
3. Server sends entity baselines via `svc_spawnbaseline`
4. Normal update cycle begins

### Update Cycle
1. **Client Input**: Client sends `clc_move` and `clc_stringcmd` messages
2. **Server Processing**: Server processes inputs and updates game state
3. **State Broadcast**: Server sends `svc_frame` followed by delta updates
4. **Client Rendering**: Client interpolates between received states

### Reliable vs Unreliable Messages
- **Reliable**: Config strings, baselines, critical game events
- **Unreliable**: Entity updates, temporary effects, cursor positions

## Entity Management

### Entity Identification
- **Entity Number**: 16-bit unique identifier per entity
- **Baseline System**: Initial full state, subsequent delta updates
- **Removal**: Special bit (`U_REMOVE`) marks entities for removal

### Update Process
```c
// Server-side update
MSG_WriteDeltaEntity(from_state, to_state, force_update);

// Client-side reconstruction
MSG_ReadDeltaEntity(current_state, entity_number, changed_bits);
```

## UI System Protocol

### Layout Updates
UI layouts are transmitted as compressed frame hierarchies:
1. **Layer Identification**: Byte indicating UI layer
2. **Frame Delta**: Compressed frame state changes
3. **Buffer Data**: Additional binary data (textures, scripts)

### Frame Structure
```c
typedef struct {
    DWORD parent;           // Parent frame reference
    DWORD flags;            // Visibility/enable flags
    VECTOR2 position;       // Screen position
    VECTOR2 size;          // Width and height
    DWORD texture;         // Texture reference
    COLOR32 color;         // Tint color
    LPCSTR text;           // Display text
    LPCSTR tooltip;        // Hover text
    LPCSTR onclick;        // Click handler
} uiFrame_t;
```

## Game-Specific Messages

### Camera Control
- **Movement**: `clc_move` with x,y coordinates
- **Zoom**: Player state updates with distance field
- **Rotation**: Quaternion updates in player state

### Unit Commands
Commands are sent as string commands:
```
"select <unit_id>"
"move <x> <y>"
"attack <target_id>"
"build <building_type> <x> <y>"
```

### Game Events
Special temporary entity messages for:
- Combat effects (damage, healing)
- Building construction
- Unit spawning/despawning
- Resource collection

## Performance Optimizations

### Bit Packing
- **Angle Quantization**: 8-bit angles (256 steps = 1.4° precision)
- **Position Compression**: 16-bit fixed-point coordinates
- **Scale Quantization**: 16-bit fixed-point scale values

### Update Prioritization
- **Distance-based**: Far entities update less frequently
- **Visibility**: Off-screen entities use reduced update rate
- **Importance**: Player units prioritized over decorative objects

### Caching
- **Config Strings**: Static data cached on client
- **Model Indices**: Persistent across map lifetime
- **Texture References**: Cached by index lookup

## Error Handling

### Protocol Errors
- **Unknown Message Type**: Logged and skipped
- **Buffer Overflow**: Graceful handling with size checks
- **Invalid Entity**: Skipped with error logging

### Recovery Mechanisms
- **Baseline Resync**: Full state refresh on major errors
- **Delta Reset**: Force next update to be full state
- **Connection Reset**: Re-establish connection on critical failures

## Security Considerations

### Message Validation
- **Bounds Checking**: All array indices validated
- **String Length**: Maximum string lengths enforced
- **Type Safety**: Strict type checking for all fields

### Rate Limiting
- **Command Throttling**: Prevent spam attacks
- **Update Coalescing**: Combine multiple small updates
- **Bandwidth Management**: Dynamic update frequency based on connection quality

## Implementation Notes

### Endianness
- **Network Byte Order**: All multi-byte values in little-endian
- **Platform Compatibility**: Handles both little and big-endian systems

### Thread Safety
- **Single-threaded Processing**: All message processing on main thread
- **Buffer Management**: Lock-free circular buffers for entity updates

### Debugging
- **Verbose Logging**: Optional detailed message logging
- **Replay System**: Message recording for debugging
- **Statistics**: Bandwidth usage and update frequency tracking

## File References
- `src/common/msg.c`: Core serialization/deserialization
- `src/common/net.h`: Network interface definitions
- `src/client/cl_parse.c`: Client message parsing
- `src/server/sv_parse.c`: Server message parsing
- `src/server/sv_game.c`: Game-specific message handling
