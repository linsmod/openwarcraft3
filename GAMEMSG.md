# OpenWarcraft3 Client-Server Message Protocol Documentation

## Overview

This document describes the message protocol used for communication between the OpenWarcraft3 client and server. The protocol uses a delta compression system to efficiently synchronize game state between clients and server.

## Protocol Architecture

### Message Types
- **Entity State Updates**: Synchronize entity properties (units, buildings, etc.)
- **UI Frame Updates**: Synchronize UI element states
- **Player State Updates**: Synchronize player-specific data
- **Command Messages**: Client-to-server commands (movement, actions, etc.)

### Field System

The protocol uses a flexible field-based system where each data structure is described by a set of fields that define:
- Field name and offset
- Data type
- Array length (for fixed-size arrays)

#### Field Types

| Type | Description | Size | Usage |
|------|-------------|------|-------|
| `NFT_BYTE` | 8-bit unsigned integer | 1 byte | Small values, flags |
| `NFT_SHORT` | 16-bit signed integer | 2 bytes | IDs, indices |
| `NFT_LONG` | 32-bit signed integer | 4 bytes | Large values, bitmasks |
| `NFT_FLOAT` | 32-bit floating point | 4 bytes | Precise values |
| `NFT_ROUND` | Float rounded to short | 2 bytes | Position coordinates |
| `NFT_PACKED_FLOAT` | Float scaled to short | 2 bytes | Scale values |
| `NFT_QUATERNION` | 4D rotation | 8 bytes | Rotations |
| `NFT_VECTOR2` | 2D vector | 8 bytes | 2D positions |
| `NFT_VECTOR3` | 3D vector | 6 bytes | 3D positions |
| `NFT_ANGLE` | Angle in radians | 2 bytes | Rotation angles |
| `NFT_TEXT` | Null-terminated string | Variable | Static strings |
| `NFT_DUPTEXT` | Null-terminated string | Variable | Dynamic strings |
| `NFT_COLOR` | Color array | Variable | Indicator colors |

#### Field Definitions

Fields are defined using macros:

```c
// Single field
#define NETF(type, x) #x,((uint8_t *)&((type*)0)->x - (uint8_t *)NULL), 1

// Array field
#define NETF_ARRAY(type, x, len) #x,((uint8_t *)&((type*)0)->x - (uint8_t *)NULL), len
```

## Entity State Protocol

### Entity State Structure

```c
typedef struct entityState_s {
    VECTOR3 origin;           // Position in 3D space
    VECTOR3 angle;            // Rotation angles
    float scale;              // Scale factor
    int frame;                // Animation frame
    short model;              // Model index
    short model2;             // Secondary model index
    short image;              // Image/texture index
    byte player;              // Owning player ID
    int flags;                // Entity flags
    int radius;               // Collision radius
    int splat;                // Splat/decal info
    int stats;                // Entity statistics
    byte renderfx;            // Rendering effects (selection state)
    byte color_nums;          // Number of active indicator colors
    COLOR32 indicatorColors[10]; // Array of indicator colors
} entityState_t;
```

### Entity State Fields

| Field | Type | Array Length | Description |
|-------|------|--------------|-------------|
| `origin` | NFT_VECTOR3 | 1 | 3D position (x,y,z) |
| `angle` | NFT_ANGLE | 1 | Rotation angles |
| `scale` | NFT_PACKED_FLOAT | 1 | Scale factor (compressed) |
| `frame` | NFT_LONG | 1 | Animation frame index |
| `model` | NFT_SHORT | 1 | Primary model ID |
| `model2` | NFT_SHORT | 1 | Secondary model ID |
| `image` | NFT_SHORT | 1 | Texture/image ID |
| `player` | NFT_BYTE | 1 | Owning player ID |
| `flags` | NFT_LONG | 1 | Entity state flags |
| `radius` | NFT_ROUND | 1 | Collision radius |
| `splat` | NFT_LONG | 1 | Decal/splat information |
| `stats` | NFT_LONG | 1 | Entity statistics |
| `renderfx` | NFT_BYTE | 1 | Rendering effects (selection state) |
| `color_nums` | NFT_BYTE | 1 | Number of active indicator colors |
| `indicatorColors` | NFT_COLOR | 10 | Array of indicator colors |

## Delta Compression

### How Delta Compression Works

1. **State Comparison**: The server compares the current state with the last known state for each client
2. **Bit Mask Generation**: A bit mask indicates which fields have changed
3. **Selective Transmission**: Only changed fields are transmitted
4. **Client Reconstruction**: Clients apply the delta to reconstruct the current state

### Bit Mask Format

Each field corresponds to a bit in a 32-bit mask:
- Bit 0: First field
- Bit 1: Second field
- ...
- Bit 31: 32nd field (maximum)

### Example Delta Update

```c
// Server determines what changed
DWORD bits = MSG_GetBits(oldState, newState, entityStateFields);

// Only send changed fields
if (bits != 0) {
    MSG_WriteEntityBits(msg, bits, entityNumber);
    MSG_WriteFields(msg, newState, entityStateFields, bits);
}
```

## Message Flow

### Server to Client

1. **Entity State Updates**: Periodic updates for all visible entities
2. **UI Frame Updates**: UI element state changes
3. **Player State Updates**: Player-specific data (camera, stats, etc.)

### Client to Server

1. **Command Messages**: Player actions (movement, abilities, etc.)
2. **UI Interactions**: Button clicks, menu selections
3. **State Requests**: Explicit state synchronization requests

## Data Serialization

### Numeric Values

- **Floats**: Direct 32-bit IEEE 754 or compressed formats
- **Angles**: Compressed to 16-bit (0-65535 maps to 0-360 degrees)
- **Vectors**: Component-wise compression based on precision needs

### Strings

- **Static Strings**: Null-terminated, included directly
- **Dynamic Strings**: Copied and managed by the receiver

### Arrays

- **Fixed Arrays**: Length specified in field definition
- **Color Arrays**: Each color is 32-bit RGBA

## Usage Examples

### Defining New Entity Types

```c
// Define a new entity type
typedef struct myEntity_s {
    entityState_t base;  // Inherit standard fields
    float customValue;   // Custom field
    int customArray[5];  // Custom array
} myEntity_t;

// Define fields for the new type
netField_t myEntityFields[] = {
    { NETF(myEntity_t, base), NFT_VECTOR3 },  // Standard fields
    { NETF(myEntity_t, customValue), NFT_FLOAT },
    { NETF_ARRAY(myEntity_t, customArray, 5), NFT_LONG },
    { NULL }
};
```

### Sending Entity Updates

```c
// Server side
void SendEntityUpdate(entity_t *ent, client_t *client) {
    entityState_t state;
    // Fill state structure
    
    MSG_WriteDeltaEntity(&client->msg, 
                        &client->lastKnownState[ent->number], 
                        &state, 
                        false);
}
```

### Receiving Entity Updates

```c
// Client side
void ReceiveEntityUpdate(msg_t *msg) {
    int bits, number;
    number = MSG_ReadEntityBits(msg, &bits);
    
    entityState_t state;
    MSG_ReadDeltaEntity(msg, &state, number, bits);
    
    // Apply state to local entity
    entities[number].currentState = state;
}
```

## Performance Considerations

### Bandwidth Optimization

- **Delta compression** reduces bandwidth by 60-80%
- **Field-level granularity** minimizes update sizes
- **Type-specific compression** balances precision vs. size

### Memory Usage

- **Fixed-size arrays** avoid dynamic allocation
- **Shared string pool** for common strings
- **State caching** reduces comparison overhead

## Error Handling

### Protocol Robustness

- **Bounds checking** on all reads
- **Graceful degradation** for unknown fields
- **Version compatibility** through field evolution

### Debugging

- **Field name tracking** for debugging
- **Bit mask inspection** for update analysis
- **State validation** on both client and server

## Future Extensions

### Planned Features

- **Variable-length arrays** for dynamic collections
- **Custom compression** for specific data types
- **Delta prediction** for smoother interpolation
- **Partial state updates** for large entities

### Backward Compatibility

- **Field versioning** through type evolution
- **Optional fields** for new features
- **Graceful fallback** for older clients

## Implementation Notes

### Key Files

- `src/common/msg.c`: Core protocol implementation
- `src/common/shared.h`: Shared data structures
- `src/server/sv_parse.c`: Server-side parsing
- `src/client/cl_parse.c`: Client-side parsing

### Integration Points

- **Game logic** uses the protocol for state synchronization
- **Rendering system** consumes entity state updates
- **UI system** processes frame updates
- **Network layer** handles message transmission

This protocol provides a robust foundation for real-time multiplayer synchronization while maintaining flexibility for future game features and optimizations.
