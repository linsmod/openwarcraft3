#include "css_animation.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

/**
 * Parse time value (e.g., "2s", "500ms") to seconds
 */
static float parse_time_value(const char *time_str)
{
    if (!time_str) return 0.0f;
    
    float value = 0.0f;
    sscanf(time_str, "%f", &value);
    
    if (strstr(time_str, "ms")) {
        return value / 1000.0f;
    } else if (strstr(time_str, "s")) {
        return value;
    }
    
    return value;
}

/**
 * Parse percentage to float (0.0 to 1.0)
 */
static float parse_percentage(const char *percent_str)
{
    if (!percent_str) return 0.0f;
    
    float value = 0.0f;
    sscanf(percent_str, "%f", &value);
    
    if (strstr(percent_str, "%")) {
        return value / 100.0f;
    }
    
    return value;
}

int css_parse_keyframes(const char *keyframes_text,
                      char *name,
                      keyframe_t *keyframes,
                      int max_keyframes)
{
    if (!keyframes_text || !name || !keyframes || max_keyframes <= 0) {
        return -1;
    }
    
    printf("DEBUG: Parsing keyframes, input: '%s'\n", keyframes_text);
    
    // Find @keyframes keyword
    const char *keyframes_start = strstr(keyframes_text, "@keyframes");
    if (!keyframes_start) {
        printf("DEBUG: @keyframes not found in input\n");
        return -1;
    }
    printf("DEBUG: Found @keyframes at offset %ld\n", keyframes_start - keyframes_text);
    
    // Extract name
    const char *name_start = keyframes_start + 10;
    while (*name_start && isspace(*name_start)) name_start++;
    
    if (!name_start || *name_start == '\0' || *name_start == '{') {
        printf("DEBUG: No name found after @keyframes\n");
        return -1;
    }
    
    const char *name_end = strchr(name_start, '{');
    if (!name_end) {
        printf("DEBUG: No '{' found for keyframes block\n");
        return -1;
    }
    
    size_t name_len = name_end - name_start;
    if (name_len > 63) name_len = 63;
    
    printf("DEBUG: name_start='%.*s', name_len=%d\n", (int)name_len, name_start, (int)name_len);
    
    strncpy(name, name_start, name_len);
    name[name_len] = '\0';
    
    // Trim trailing whitespace from name
    while (name_len > 0 && isspace(name[name_len - 1])) {
        name_len--;
        name[name_len] = '\0';
    }
    
    printf("DEBUG: Extracted name='%s'\n", name);
    
    // Parse keyframes inside the braces
    const char *block_start = name_end + 1;
    
    // Find matching closing brace for @keyframes block (handles nested braces)
    int brace_count = 1;
    const char *block_end = block_start;
    while (*block_end && brace_count > 0) {
        if (*block_end == '{') brace_count++;
        else if (*block_end == '}') brace_count--;
        block_end++;
    }
    if (brace_count != 0) return -1;
    block_end--; // Move back to the final '}'
    
    size_t block_len = block_end - block_start;
    printf("DEBUG: block_len=%d\n", (int)block_len);
    char *block = malloc(block_len + 1);
    if (!block) {
        printf("DEBUG: malloc failed\n");
        return -1;
    }
    
    strncpy(block, block_start, block_len);
    block[block_len] = '\0';
    
    printf("DEBUG: block='%s'\n", block);
    
    // Parse individual keyframe selectors
    int keyframe_count = 0;
    const char *pos = block;
    
    printf("DEBUG: Starting keyframe parsing, max_keyframes=%d\n", max_keyframes);
    
    while (*pos && keyframe_count < max_keyframes) {
        printf("DEBUG: Parsing keyframe %d, pos='%s'\n", keyframe_count, pos);
        // Skip whitespace
        while (*pos && isspace(*pos)) pos++;
        if (!*pos) break;
        
        // Find selector (ends with '{')
        const char *selector_start = pos;
        const char *selector_end = strchr(pos, '{');
        if (!selector_end) break;
        
        // Parse selector offset
        float offset = 0.0f;
        char selector[32];
        strncpy(selector, selector_start, selector_end - selector_start < 31 ? 
               selector_end - selector_start : 31);
        selector[selector_end - selector_start < 31 ? selector_end - selector_start : 31] = '\0';
        
        css_parse_keyframe_selector(selector, &offset);
        
        // Find end of keyframe block
        const char *keyframe_end = strchr(selector_end + 1, '}');
        if (!keyframe_end) break;
        
        // Parse properties (simplified - just looking for common ones)
        const char *prop_start = selector_end + 1;
        char *prop_end = (char*)keyframe_end;
        
        // Initialize keyframe
        memset(&keyframes[keyframe_count], 0, sizeof(keyframe_t));
        keyframes[keyframe_count].offset = offset;
        
        // Parse opacity property
        const char *opacity = strstr(prop_start, "opacity:");
        printf("DEBUG: Looking for opacity, prop_start='%s'\n", prop_start);
        if (opacity) {
            printf("DEBUG: Found opacity at offset %ld\n", opacity - prop_start);
        }
        if (opacity && opacity < keyframe_end) {
            const char *value_start = opacity + 8;
            const char *value_end = strchr(value_start, ';');
            printf("DEBUG: value_start='%s'\n", value_start);
            if (value_end && value_end < keyframe_end) {
                char value_str[32];
                strncpy(value_str, value_start, 
                       value_end - value_start < 31 ? value_end - value_start : 31);
                value_str[value_end - value_start < 31 ? value_end - value_start : 31] = '\0';
                
                printf("DEBUG: value_str='%s'\n", value_str);
                float parsed_value = parse_percentage(value_str);
                printf("DEBUG: parsed_value=%f\n", parsed_value);
                
                keyframes[keyframe_count].prop_type = ANIM_PROP_NUMBER;
                keyframes[keyframe_count].value.number = parsed_value;
                keyframe_count++;
                printf("DEBUG: Incremented keyframe_count to %d\n", keyframe_count);
            } else {
                printf("DEBUG: value_end not found or out of range\n");
            }
        }
        
        // Parse transform property
        const char *transform = strstr(prop_start, "transform:");
        if (transform && transform < keyframe_end) {
            const char *value_start = transform + 10;
            const char *value_end = strchr(value_start, ';');
            if (value_end && value_end < keyframe_end) {
                // Simple translate parsing
                const char *translate = strstr(value_start, "translate(");
                if (translate) {
                    float x, y;
                    if (sscanf(translate + 10, "%f,%f)", &x, &y) == 2) {
                        keyframes[keyframe_count].prop_type = ANIM_PROP_TRANSFORM;
                        keyframes[keyframe_count].value.translate.x = x;
                        keyframes[keyframe_count].value.translate.y = y;
                        keyframe_count++;
                    }
                }
            }
        }
        
        pos = keyframe_end + 1;
    }
    
    free(block);
    printf("DEBUG: Returning keyframe_count=%d\n", keyframe_count);
    return keyframe_count;
}

bool css_parse_animation_shorthand(const char *anim_str,
                                animation_props_t *props)
{
    if (!anim_str || !props) return false;
    
    // Initialize with defaults
    strcpy(props->name, "none");
    props->duration = 0.0f;
    props->delay = 0.0f;
    strcpy(props->timing_func, "ease");
    props->iteration_count = 1;
    strcpy(props->direction, "normal");
    strcpy(props->fill_mode, "none");
    
    // Parse tokens
    char str_copy[512];
    strncpy(str_copy, anim_str, 511);
    str_copy[511] = '\0';
    
    char *token = strtok(str_copy, " ");
    int token_count = 0;
    
    while (token && token_count < 6) {
        // Try to parse as name
        if (token_count == 0) {
            strncpy(props->name, token, 63);
            props->name[63] = '\0';
        }
        // Try to parse as duration
        else if (strstr(token, "s") || strstr(token, "ms")) {
            if (props->duration == 0.0f) {
                props->duration = parse_time_value(token);
            } else {
                props->delay = parse_time_value(token);
            }
        }
        // Try to parse as timing function
        else if (strstr(token, "ease") || strstr(token, "linear") || 
                 strstr(token, "cubic-bezier")) {
            strncpy(props->timing_func, token, 31);
            props->timing_func[31] = '\0';
        }
        // Try to parse as iteration count
        else if (strcmp(token, "infinite") == 0) {
            props->iteration_count = -1;
        } else if (strstr(token, "iteration-count")) {
            // Full property form
            const char *value = strchr(token, ':');
            if (value) {
                if (strcmp(value + 1, "infinite") == 0) {
                    props->iteration_count = -1;
                } else {
                    sscanf(value + 1, "%d", &props->iteration_count);
                }
            }
        }
        // Try to parse as direction
        else if (strstr(token, "normal") || strstr(token, "reverse") ||
                 strstr(token, "alternate")) {
            strncpy(props->direction, token, 15);
            props->direction[15] = '\0';
        }
        
        token = strtok(NULL, " ");
        token_count++;
    }
    
    return true;
}

bool css_parse_keyframe_selector(const char *selector, float *offset)
{
    if (!selector || !offset) return false;
    
    // Check for "from" and "to" keywords
    if (strcmp(selector, "from") == 0) {
        *offset = 0.0f;
        return true;
    } else if (strcmp(selector, "to") == 0) {
        *offset = 1.0f;
        return true;
    }
    
    // Parse percentage
    *offset = parse_percentage(selector);
    return true;
}

bool css_parse_keyframe_property(const char *property_str, const char *value_str,
                               keyframe_t *keyframe)
{
    if (!property_str || !value_str || !keyframe) return false;
    
    // Parse opacity
    if (strcmp(property_str, "opacity") == 0) {
        keyframe->prop_type = ANIM_PROP_NUMBER;
        keyframe->value.number = parse_percentage(value_str);
        return true;
    }
    
    // Parse transform (simplified)
    if (strcmp(property_str, "transform") == 0) {
        const char *translate = strstr(value_str, "translate(");
        if (translate) {
            keyframe->prop_type = ANIM_PROP_TRANSFORM;
            sscanf(translate + 10, "%f,%f)", 
                   &keyframe->value.translate.x,
                   &keyframe->value.translate.y);
            return true;
        }
    }
    
    return false;
}

// Stub implementations for individual property parsers
bool css_parse_animation_name(const char *name_str, animation_props_t *props) {
    if (!name_str || !props) return false;
    strncpy(props->name, name_str, 63);
    props->name[63] = '\0';
    return true;
}

bool css_parse_animation_duration(const char *duration_str, animation_props_t *props) {
    if (!duration_str || !props) return false;
    props->duration = parse_time_value(duration_str);
    return true;
}

bool css_parse_animation_timing_function(const char *timing_str, animation_props_t *props) {
    if (!timing_str || !props) return false;
    strncpy(props->timing_func, timing_str, 31);
    props->timing_func[31] = '\0';
    return true;
}

bool css_parse_animation_delay(const char *delay_str, animation_props_t *props) {
    if (!delay_str || !props) return false;
    props->delay = parse_time_value(delay_str);
    return true;
}

bool css_parse_animation_iteration_count(const char *iter_str, animation_props_t *props) {
    if (!iter_str || !props) return false;
    if (strcmp(iter_str, "infinite") == 0) {
        props->iteration_count = -1;
    } else {
        sscanf(iter_str, "%d", &props->iteration_count);
    }
    return true;
}

bool css_parse_animation_direction(const char *dir_str, animation_props_t *props) {
    if (!dir_str || !props) return false;
    strncpy(props->direction, dir_str, 15);
    props->direction[15] = '\0';
    return true;
}

bool css_parse_animation_fill_mode(const char *fill_str, animation_props_t *props) {
    if (!fill_str || !props) return false;
    strncpy(props->fill_mode, fill_str, 15);
    props->fill_mode[15] = '\0';
    return true;
}
