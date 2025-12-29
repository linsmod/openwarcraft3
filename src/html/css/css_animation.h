#ifndef CSS_ANIMATION_H
#define CSS_ANIMATION_H

// #include <stdbool.h>
#include <stdint.h>

#include "../animation/anim_state.h"

/**
 * CSS Animation Parser
 * Parses @keyframes rules and animation properties
 */

/**
 * Parse CSS @keyframes rule
 * @param keyframes_text CSS text containing @keyframes
 * @param name Output keyframe animation name
 * @param keyframes Output array of keyframes
 * @param max_keyframes Maximum number of keyframes to parse
 * @return Number of keyframes parsed, or -1 on error
 */
int css_parse_keyframes(const char *keyframes_text, 
                      char *name,
                      keyframe_t *keyframes,
                      int max_keyframes);

/**
 * Parse animation shorthand property
 * @param anim_str Animation string (e.g., "fadein 2s ease-in-out infinite")
 * @param props Output animation properties
 * @return true if parsing succeeded
 */
bool css_parse_animation_shorthand(const char *anim_str, 
                                animation_props_t *props);

/**
 * Parse individual animation properties
 */
bool css_parse_animation_name(const char *name_str, animation_props_t *props);
bool css_parse_animation_duration(const char *duration_str, animation_props_t *props);
bool css_parse_animation_timing_function(const char *timing_str, animation_props_t *props);
bool css_parse_animation_delay(const char *delay_str, animation_props_t *props);
bool css_parse_animation_iteration_count(const char *iter_str, animation_props_t *props);
bool css_parse_animation_direction(const char *dir_str, animation_props_t *props);
bool css_parse_animation_fill_mode(const char *fill_str, animation_props_t *props);

/**
 * Parse keyframe selector (e.g., "0%", "50%", "to", "from")
 * @param selector Keyframe selector string
 * @param offset Output offset value (0.0 to 1.0)
 * @return true if parsing succeeded
 */
bool css_parse_keyframe_selector(const char *selector, float *offset);

/**
 * Parse keyframe property declarations
 * @param property_str Property name (e.g., "opacity")
 * @param value_str Property value (e.g., "1", "0")
 * @param keyframe Output keyframe to populate
 * @return true if parsing succeeded
 */
bool css_parse_keyframe_property(const char *property_str, const char *value_str,
                               keyframe_t *keyframe);

#endif // CSS_ANIMATION_H
