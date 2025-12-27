#ifndef ANIM_STATE_H
#define ANIM_STATE_H

#include <stdint.h>
#include <stdbool.h>

/**
 * CSS Animation State Management
 * Handles animation lifecycle, updates, and state tracking
 */

// Animation properties
typedef struct {
    char name[64];
    float duration;        // Animation duration in seconds
    float delay;           // Animation delay in seconds
    char timing_func[32];  // ease, linear, ease-in, ease-out, ease-in-out, cubic-bezier(...)
    int iteration_count;    // Number of iterations, or -1 for infinite
    char direction[16];    // normal, reverse, alternate, alternate-reverse
    char fill_mode[16];    // forwards, backwards, both, none
    bool running;
    bool paused;
} animation_props_t;

// Animation target property
typedef enum {
    ANIM_PROP_COLOR,          // Color properties (color, background-color, etc.)
    ANIM_PROP_LENGTH,        // Length properties (width, height, margin, padding, etc.)
    ANIM_PROP_PERCENTAGE,     // Percentage-based properties
    ANIM_PROP_NUMBER,         // Numeric properties (opacity, z-index, etc.)
    ANIM_PROP_TRANSFORM        // Transform properties (translate, rotate, scale)
} anim_property_type_t;

// Animation value
typedef union {
    struct { uint8_t r, g, b, a; } color;
    struct { float value; float unit; } length;
    float percentage;
    float number;
    struct { float x, y; } translate;
    struct { float x, y, z; } rotate;
    struct { float x, y; } scale;
} anim_value_t;

// Keyframe definition
typedef struct {
    float offset;              // 0.0 to 1.0
    anim_property_type_t prop_type;
    anim_value_t value;
} keyframe_t;

// Active animation instance
typedef struct {
    uint32_t id;              // Unique animation ID
    animation_props_t props;
    keyframe_t keyframes[32];  // Max 32 keyframes per animation
    int keyframe_count;
    
    // Current state
    float elapsed_time;        // Time since animation started
    int current_iteration;     // Current iteration number
    
    // Computed current values
    anim_value_t current_values[16];  // Current animated property values
    int prop_count;
    
    bool finished;
} animation_instance_t;

// Animation manager
typedef struct {
    animation_instance_t instances[64];  // Max concurrent animations
    int instance_count;
    uint32_t next_id;
    float global_time;
} animation_manager_t;

/**
 * Initialize animation manager
 * @param manager Animation manager to initialize
 */
void anim_manager_init(animation_manager_t *manager);

/**
 * Update all active animations
 * @param manager Animation manager
 * @param delta_time Time elapsed since last update (in seconds)
 */
void anim_manager_update(animation_manager_t *manager, float delta_time);

/**
 * Start a new animation
 * @param manager Animation manager
 * @param props Animation properties
 * @param keyframes Array of keyframes
 * @param keyframe_count Number of keyframes
 * @return Animation instance ID, or 0 on error
 */
uint32_t anim_manager_start(animation_manager_t *manager,
                        const animation_props_t *props,
                        const keyframe_t *keyframes,
                        int keyframe_count);

/**
 * Stop an animation
 * @param manager Animation manager
 * @param anim_id Animation instance ID to stop
 */
void anim_manager_stop(animation_manager_t *manager, uint32_t anim_id);

/**
 * Pause/resume an animation
 * @param manager Animation manager
 * @param anim_id Animation instance ID
 * @param paused True to pause, false to resume
 */
void anim_manager_set_paused(animation_manager_t *manager, uint32_t anim_id, bool paused);

/**
 * Get current value of an animated property
 * @param manager Animation manager
 * @param anim_id Animation instance ID
 * @param prop_index Property index within the animation
 * @param value Output value
 * @return true if value exists, false otherwise
 */
bool anim_manager_get_value(const animation_manager_t *manager,
                           uint32_t anim_id, int prop_index,
                           anim_value_t *value);

/**
 * Clean up animation manager
 * @param manager Animation manager
 */
void anim_manager_cleanup(animation_manager_t *manager);

#endif // ANIM_STATE_H
