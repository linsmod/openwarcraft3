#include "anim_state.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

/**
 * Calculate easing function value
 * @param timing_func Timing function name
 * @param t Input value (0.0 to 1.0)
 * @return Eased output value
 */
static float apply_easing(const char *timing_func, float t)
{
    if (!timing_func) return t;
    
    // Clamp input
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;
    
    if (strcmp(timing_func, "linear") == 0) {
        return t;
    } else if (strcmp(timing_func, "ease") == 0) {
        // Ease (cubic-bezier(0.25, 0.1, 0.25, 1.0))
        return t * t * (3.0f - 2.0f * t);
    } else if (strcmp(timing_func, "ease-in") == 0) {
        return t * t;
    } else if (strcmp(timing_func, "ease-out") == 0) {
        return t * (2.0f - t);
    } else if (strcmp(timing_func, "ease-in-out") == 0) {
        if (t < 0.5f) {
            return 2.0f * t * t;
        } else {
            return -1.0f + (4.0f - 2.0f * t) * t;
        }
    }
    
    // Default to linear
    return t;
}

/**
 * Interpolate between two animation values
 * @param from Starting value
 * @param to Ending value
 * @param prop_type Property type
 * @param t Interpolation factor (0.0 to 1.0)
 * @param result Output interpolated value
 */
static void interpolate_values(const anim_value_t *from, const anim_value_t *to,
                           anim_property_type_t prop_type, float t,
                           anim_value_t *result)
{
    
    switch (prop_type) {
        case ANIM_PROP_COLOR: {
            result->color.r = (uint8_t)(from->color.r + (to->color.r - from->color.r) * t);
            result->color.g = (uint8_t)(from->color.g + (to->color.g - from->color.g) * t);
            result->color.b = (uint8_t)(from->color.b + (to->color.b - from->color.b) * t);
            result->color.a = (uint8_t)(from->color.a + (to->color.a - from->color.a) * t);
            break;
        }
        case ANIM_PROP_LENGTH:
        case ANIM_PROP_NUMBER:
        case ANIM_PROP_PERCENTAGE: {
            result->number = from->number + (to->number - from->number) * t;
            result->length.unit = from->length.unit;  // Preserve unit
            break;
        }
        case ANIM_PROP_TRANSFORM: {
            result->translate.x = from->translate.x + (to->translate.x - from->translate.x) * t;
            result->translate.y = from->translate.y + (to->translate.y - from->translate.y) * t;
            result->rotate.x = from->rotate.x + (to->rotate.x - from->rotate.x) * t;
            result->rotate.y = from->rotate.y + (to->rotate.y - from->rotate.y) * t;
            result->rotate.z = from->rotate.z + (to->rotate.z - from->rotate.z) * t;
            result->scale.x = from->scale.x + (to->scale.x - from->scale.x) * t;
            result->scale.y = from->scale.y + (to->scale.y - from->scale.y) * t;
            break;
        }
    }
}

void anim_manager_init(animation_manager_t *manager)
{
    if (!manager) return;
    
    memset(manager, 0, sizeof(animation_manager_t));
    manager->next_id = 1;
}

void anim_manager_update(animation_manager_t *manager, float delta_time)
{
    if (!manager) return;
    
    manager->global_time += delta_time;
    
    for (int i = 0; i < manager->instance_count; i++) {
        animation_instance_t *anim = &manager->instances[i];
        
        if (!anim->props.running || anim->finished) continue;
        if (anim->props.paused) continue;
        
        // Apply delay
        if (anim->elapsed_time < anim->props.delay) {
            anim->elapsed_time += delta_time;
            continue;
        }
        
        // Calculate animation time (excluding delay)
        float anim_time = anim->elapsed_time - anim->props.delay;
        
        // Check if animation is complete
        if (anim_time >= anim->props.duration) {
            // Check iterations
            if (anim->props.iteration_count == -1 || 
                anim->current_iteration < anim->props.iteration_count - 1) {
                // Next iteration
                anim->current_iteration++;
                anim->elapsed_time = anim->props.delay;
                
                // Handle alternate direction
                if (strcmp(anim->props.direction, "alternate") == 0 ||
                    strcmp(anim->props.direction, "alternate-reverse") == 0) {
                    // Reverse keyframes order
                    // TODO: Implement keyframe reversal
                }
            } else {
                // Animation finished
                anim->finished = true;
                anim->props.running = false;
            }
            continue;
        }
        
        // Calculate normalized time (0.0 to 1.0) within current iteration
        float t = anim_time / anim->props.duration;
        
        // Apply easing
        float eased_t = apply_easing(anim->props.timing_func, t);
        
        // Interpolate values between keyframes
        // Find which keyframes to interpolate between
        for (int j = 0; j < anim->prop_count; j++) {
            int lower_idx = 0;
            int upper_idx = anim->keyframe_count - 1;
            
            for (int k = 0; k < anim->keyframe_count - 1; k++) {
                if (eased_t >= anim->keyframes[k].offset &&
                    eased_t <= anim->keyframes[k + 1].offset) {
                    lower_idx = k;
                    upper_idx = k + 1;
                    break;
                }
            }
            
            // Calculate interpolation factor
            float range = anim->keyframes[upper_idx].offset - anim->keyframes[lower_idx].offset;
            float interp_t = range > 0.0f ? 
                (eased_t - anim->keyframes[lower_idx].offset) / range : 0.0f;
            
            // Interpolate between lower and upper keyframes
            interpolate_values(&anim->keyframes[lower_idx].value, &anim->keyframes[upper_idx].value,
                             anim->keyframes[lower_idx].prop_type, interp_t,
                             &anim->current_values[j]);
        }
        
        anim->elapsed_time += delta_time;
    }
}

uint32_t anim_manager_start(animation_manager_t *manager,
                        const animation_props_t *props,
                        const keyframe_t *keyframes,
                        int keyframe_count)
{
    if (!manager || !props || !keyframes || keyframe_count == 0) return 0;
    
if (manager->instance_count >= 64) return 0;  // Max instances reached
    
    // Find free slot
    int slot = -1;
    for (int i = 0; i < 64; i++) {
        if (!manager->instances[i].props.running) {
            slot = i;
            break;
        }
    }

    if (slot < 0) return 0;
    
    animation_instance_t *anim = &manager->instances[slot];
    
    // Copy properties
    memcpy(&anim->props, props, sizeof(animation_props_t));
    
    // Copy keyframes
    if (keyframe_count > 32) keyframe_count = 32;
    memcpy(anim->keyframes, keyframes, sizeof(keyframe_t) * keyframe_count);
    anim->keyframe_count = keyframe_count;
    
    // Initialize state
    anim->id = manager->next_id++;
    anim->elapsed_time = 0.0f;
    anim->current_iteration = 0;
    anim->finished = false;
    anim->props.running = true;
    anim->props.paused = false;
    
// Initialize current values from first keyframe
    anim->prop_count = 1;
    memcpy(&anim->current_values[0], &keyframes[0].value, sizeof(anim_value_t));
    
    manager->instance_count++;
    
    return anim->id;
}

void anim_manager_stop(animation_manager_t *manager, uint32_t anim_id)
{
    if (!manager) return;
    
    for (int i = 0; i < manager->instance_count; i++) {
        if (manager->instances[i].id == anim_id) {
            manager->instances[i].finished = true;
            manager->instances[i].props.running = false;
            return;
        }
    }
}

void anim_manager_set_paused(animation_manager_t *manager, uint32_t anim_id, bool paused)
{
    if (!manager) return;
    
    for (int i = 0; i < manager->instance_count; i++) {
        if (manager->instances[i].id == anim_id) {
            manager->instances[i].props.paused = paused;
            return;
        }
    }
}

bool anim_manager_get_value(const animation_manager_t *manager,
                           uint32_t anim_id, int prop_index,
                           anim_value_t *value)
{
    if (!manager || !value) return false;
    
    for (int i = 0; i < manager->instance_count; i++) {
        if (manager->instances[i].id == anim_id) {
            if (prop_index < manager->instances[i].prop_count) {
                *value = manager->instances[i].current_values[prop_index];
                return true;
            }
            return false;
        }
    }
    
    return false;
}

void anim_manager_cleanup(animation_manager_t *manager)
{
    if (!manager) return;
    
    // Stop all animations
    for (int i = 0; i < manager->instance_count; i++) {
        manager->instances[i].finished = true;
        manager->instances[i].props.running = false;
    }
    
    manager->instance_count = 0;
}
