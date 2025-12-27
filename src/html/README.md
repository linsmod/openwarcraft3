# HTML/CSS Renderer Enhancements

This directory contains enhancements to the HTML/CSS rendering system for OpenWarcraft3.

## Directory Structure

```
src/html/
├── css/                        # CSS parsing and processing
│   ├── css_gradient.h/c           # Linear gradient parser and generator
│   └── css_animation.h/c         # CSS animation (@keyframes) parser
├── renderer/                    # Rendering utilities
│   ├── render_utils.h/c           # Rendering utility functions
│   └── render_gradient.h/c        # Gradient texture rendering
└── animation/                   # Animation state management
    └── anim_state.h/c           # Animation runtime and state
```

## Module Overview

### 1. CSS Gradient Parser (`css_gradient.h/c`)

**Features:**
- Parse `linear-gradient()` CSS syntax
- Support direction keywords (to top, to bottom, to left, to right, angle in deg)
- Parse color stops with percentages
- Generate gradient textures
- Interpolate colors at any position (0.0 to 1.0)

**Usage Example:**
```c
#include "css/css_gradient.h"

linear_gradient_t gradient;
if (css_parse_linear_gradient("linear-gradient(to bottom, #1a1a2e, #0f3460)", &gradient)) {
    // Create texture
    uint8_t texture_data[width * height * 4];
    css_gradient_create_texture(&gradient, width, height, texture_data);

    // Get color at position
    uint8_t r, g, b, a;
    css_gradient_get_color(&gradient, 0.5f, &r, &g, &b, &a);
}
```

**Supported Syntax:**
- `linear-gradient(to bottom, #color1, #color2)`
- `linear-gradient(to top right, #color1, #color2, #color3)`
- `linear-gradient(135deg, #color1 0%, #color2 50%, #color3 100%)`

**Color Formats:**
- Hex: `#RGB`, `#RRGGBB`, `#RRGGBBAA`
- RGB: `rgb(255, 0, 0)`
- RGBA: `rgba(255, 0, 0, 0.5)`

### 2. Rendering Utilities (`render_utils.h/c`)

**Features:**
- Rounded rectangle rendering (placeholder, needs OpenGL integration)
- Alpha blending support
- Color blending functions
- Opacity application

**Functions:**
```c
// Draw rounded rectangle
render_rounded_rect(x, y, width, height, radius, color);

// Apply opacity to color
uint32_t new_color = apply_opacity(color, 0.5f);

// Blend two colors
uint32_t blended = blend_colors(color1, color2, 0.5f);
```

**Status:**
- Basic functions implemented
- OpenGL integration needed for actual rendering
- Rounded corners need proper vertex generation

### 3. Gradient Renderer (`render_gradient.h/c`)

**Features:**
- Cache gradient textures to avoid regeneration
- Support up to 32 cached gradients
- Automatic texture creation and management

**Functions:**
```c
// Cache gradient as texture
uint32_t tex_id = cache_gradient_texture(&gradient, width, height);

// Render gradient rect
render_gradient_rect(x, y, width, height, &gradient, tex_id);

// Clear cache
clear_gradient_cache();
```

**Status:**
- Basic caching implemented
- OpenGL texture generation implemented
- Rendering integration needed

### 4. CSS Animation Parser (`css_animation.h/c`)

**Features:**
- Parse `@keyframes` rules
- Parse animation shorthand properties
- Parse individual animation properties

**Supported Properties:**
- `animation-name`
- `animation-duration` (e.g., `2s`, `500ms`)
- `animation-timing-function` (ease, linear, ease-in, ease-out, ease-in-out)
- `animation-delay`
- `animation-iteration-count` (number or `infinite`)
- `animation-direction` (normal, reverse, alternate, alternate-reverse)
- `animation-fill-mode` (forwards, backwards, both, none)

**Usage Example:**
```c
#include "css/css_animation.h"

// Parse keyframes
keyframe_t keyframes[16];
int count = css_parse_keyframes(keyframes_text, name, keyframes, 16);

// Parse animation
animation_props_t props;
css_parse_animation_shorthand("fadein 2s ease-in-out infinite", &props);
```

**Status:**
- Keyframe parsing implemented (opacity only)
- Animation property parsing implemented
- Transform parsing simplified (translate only)
- Full property parsing needed

### 5. Animation State Manager (`anim_state.h/c`)

**Features:**
- Manage up to 64 concurrent animations
- Update all animations with easing
- Support for infinite iterations
- Pause/resume functionality
- Alternating direction support

**Functions:**
```c
// Initialize manager
anim_manager_init(&manager);

// Start animation
uint32_t anim_id = anim_manager_start(&manager, &props, keyframes, count);

// Update (call in game loop)
anim_manager_update(&manager, delta_time);

// Get current animated value
anim_value_t value;
anim_manager_get_value(&manager, anim_id, 0, &value);

// Stop animation
anim_manager_stop(&manager, anim_id);
```

**Status:**
- ✅ Core framework implemented
- ✅ Easing functions implemented (linear, ease, ease-in, ease-out, ease-in-out)
- ✅ Interpolation for colors, numbers, transforms implemented
- ✅ Integrated with HTML rendering system (animation manager initialized in HTML context)
- ✅ Animation update loop properly integrated with delta_time

## Integration Steps

### 1. Include Files in Build System

✅ **Already done in `CMakeLists.txt`**
```cmake
file(GLOB SOURCES 
    "src/html/css/*.c"
    "src/html/renderer/*.c"
    "src/html/animation/*.c"
)
```

### 2. Initialize Gradient System (Optional)

```c
#include "html/renderer/render_gradient.h"

// Call once at startup
// (Optional: cache system auto-initializes on first use)
```

### 3. Initialize Animation Manager

✅ **COMPLETED** - Animation manager has been fully integrated into the HTML rendering system.

**Implementation Details:**

1. **Context Structure Enhanced** (`src/html/html.c`)
   - Added `animation_manager_t* anim_mgr` field to the `context` structure
   - Includes `animation/anim_state.h` header

2. **Initialization** (`create_context()` in `src/html/html.c`)
   - Allocates memory for animation manager
   - Calls `anim_manager_init()` to initialize the manager

3. **Update Loop** (`html_update_and_layout()` in `src/html/html.c`)
   - Function signature updated to accept `float delta_time` parameter
   - Calls `anim_manager_update()` every frame when delta_time > 0

4. **Cleanup** (`destroy_context()` in `src/html/html.c`)
   - Calls `anim_manager_cleanup()` to stop all animations
   - Frees animation manager memory

5. **Scene Integration:**
   - `src/splash/splash_scene.c`: Calls `html_update_and_layout(delta_time)` with proper delta_time (msec → seconds)
   - `src/client/cl_scrn.c`: Updated function declaration for `html_update_and_layout(float delta_time)`

6. **Recent Fixes:**
   - ✅ Added `stdio.h` header to `anim_state.c`
   - ✅ Fixed missing closing brace in `anim_manager_start()` function
   - ✅ Updated `html_update_and_layout()` calls to pass `delta_time` parameter
   - ✅ Verified animation values update correctly (opacity: 0.0 → 0.999963)

**Usage Example:**
```c
// Animation manager is automatically initialized when HTML context is created
// via html_init() or html_render_init()

// In scene update loop (e.g., SplashScene_Update):
float delta_time = (float)msec / 1000.0f;  // Convert milliseconds to seconds
html_update_and_layout(delta_time);

// Animation updates are handled automatically within html_update_and_layout()
```

### 4. Parse CSS Animations in HTML

✅ **FULLY COMPLETED** - @keyframes parsing and application fully implemented.

**Implemented:**
1. ✅ Parse `<style>` tags for `@keyframes` rules
   - Added `keyframes_store` to context structure (stores up to 32 keyframe animations)
   - Implemented `html_scan_and_store_keyframes()` function
   - Recursively scans DOM tree for `<style>` tags
   - Extracts and parses `@keyframes` using `css_parse_keyframes()`
   - Stored in `g_html_render_context->keyframes_store[]`
   - Automatically called after HTML parsing in `html_init()`

2. ✅ Parse `animation` properties on elements
   - Added `animation_id` field to `userdata` structure
   - Implemented `apply_animation_attribute()` function
   - Parses animation shorthand using `css_parse_animation_shorthand()`
   - Looks up keyframes by name from stored keyframes_store
   - Starts animation using `anim_manager_start()`
   - Stores animation ID in element's userdata
   - Automatically called in `add_attributes()` when `animation` attribute is found

3. ✅ Store animation ID with element
   - Animation ID stored in `userdata->animation_id`
   - Can be accessed via `((userdata*)node->_private)->animation_id`

4. ✅ Apply animated values during rendering
   - Animated opacity successfully applied to HTML elements
   - Animation values interpolated correctly (0.0 → 0.999963)
   - Verified with splash screen fade-in animation

**Implementation Details:**

- **Context Enhancement** (`src/html/html.c`):
  - Added `num_keyframes` counter
  - Added `keyframes_store[MAX_KEYFRAMES]` array
  - Each entry contains: name, keyframes[], keyframe_count

- **Userdata Enhancement** (`src/html/html.c`):
  - Added `uint32_t animation_id` field
  - Stores the active animation instance ID

- **Scanning Function** (`html_scan_and_store_keyframes()`):
  - Resets keyframes storage
  - Recursively traverses DOM tree
  - Finds `<style>` elements
  - Extracts CSS content
  - Scans for `@keyframes` directives
  - Extracts animation name (between `@keyframes` and `{`)
  - Extracts complete block content (matching braces)
  - Parses using `css_parse_keyframes()`
  - Stores in context's keyframes_store

- **Animation Attribute Handler** (`apply_animation_attribute()`):
  - Parses animation shorthand (e.g., "fadein 2s ease-in-out infinite")
  - Looks up keyframes by name in `context->keyframes_store[]`
  - Starts animation with `anim_manager_start()`
  - Stores animation instance ID in element's userdata
  - Reports parsing errors and keyframes not found

**Usage Example:**
```html
<!-- Define keyframes -->
<style>
  @keyframes fadein {
    from { opacity: 0; }
    to { opacity: 1; }
  }

  @keyframes fadeInOut {
    0% { opacity: 0; }
    8.33% { opacity: 1; }
    91.67% { opacity: 1; }
    100% { opacity: 0; }
  }
</style>

<!-- Apply animation -->
<div animation="fadein 2s ease-in-out infinite">
  This element will fade in
</div>
```

**Process Flow:**
1. HTML parsing encounters `<style>` tag → Extracts CSS content
2. `html_scan_and_store_keyframes()` finds `@keyframes` → Parses and stores
3. HTML parsing encounters `<div animation="...">` → Calls `apply_animation_attribute()`
4. `apply_animation_attribute()` parses animation, finds keyframes, starts animation
5. Animation ID stored in `userdata->animation_id`
6. `html_update_and_layout()` calls `anim_manager_update()` every frame with delta_time
7. Animated values retrieved and applied during element rendering

### 5. Apply Gradient Backgrounds

Enhance `render_html_element()` in `html.c` to:
1. Check for `background-image` with `linear-gradient()`
2. Parse gradient using `css_parse_linear_gradient()`
3. Generate texture using `cache_gradient_texture()`
4. Render gradient rectangle

## Testing

### Test Files

Create test HTML files to verify features:

1. **gradient_test.html** - Test gradient parsing and rendering
2. **animation_test.html** - Test animation system
3. **combined_test.html** - Test gradients + animations together

### Manual Testing Checklist

**Animation System:**
- [x] Animation manager integrated into HTML context
- [x] Animation updates called in game loop (via html_update_and_layout)
- [x] @keyframes scanning implemented
- [x] Animation attribute parsing implemented
- [x] Animations started when elements loaded
- [x] Apply animated values during rendering
- [x] Animation timing functions work
- [x] Delta_time properly passed to update loop
- [x] Syntax errors fixed (stdio.h, missing braces)
- [ ] Animation timing functions verified (ease, linear, ease-in, etc.)
- [ ] Infinite iteration works
- [ ] Pause/resume works
- [ ] Multiple concurrent animations work
- [ ] Performance is acceptable (60 FPS)

**Splash Screen Animation:**
- [x] Fade-in animation implemented (0.5s)
- [x] Display phase configured (5s)
- [x] Fade-out animation implemented (0.5s)
- [x] Total animation cycle: 6 seconds
- [ ] Full fade-in-out cycle verified in runtime

**Gradient System:**
- [ ] Gradient background renders correctly
- [ ] Multiple color stops work
- [ ] Angle-based gradients work

## Future Enhancements

### Short Term
1. ✅ Complete animation manager integration
2. ✅ Enhance HTML renderer to parse @keyframes rules
3. ✅ Enhance HTML renderer to parse animation properties
4. ✅ Apply animated values during rendering
5. [ ] Apply gradient backgrounds (background-image: linear-gradient)
6. [ ] Implement border-radius rendering
7. [ ] Implement box-shadow rendering
8. [ ] Clean up debug printf statements

### Medium Term
1. Add more keyframe properties (transform full syntax, margin, padding, etc.)
2. Add cubic-bezier easing support
3. Optimize gradient texture caching
4. Add animation event callbacks (onstart, onend, oniteration)
5. Implement auto-scene transition after animation completion

### Long Term
1. CSS transitions support
2. Web Animation API
3. Animation composition and blending
4. Performance profiling and optimization

## Recent Changes (2024)

### Bug Fixes
- ✅ Fixed missing `stdio.h` header in `anim_state.c`
- ✅ Fixed missing closing brace in `anim_manager_start()` function
- ✅ Updated `html_update_and_layout()` to accept `delta_time` parameter
- ✅ Fixed function declarations in `splash_scene.c` and `cl_scrn.c`
- ✅ Added fade-in-out animation to `splash.html`

### Verification
- ✅ Animation system working correctly (opacity: 0.0 → 0.999963)
- ✅ Animation manager update loop properly integrated
- ✅ 6 animation instances running successfully in splash screen

## Notes

- All modules use C99 compatible code
- No external dependencies beyond OpenGL
- Memory management uses standard malloc/free
- Thread-safety: NOT thread-safe (single-threaded rendering assumed)

## Troubleshooting

### Common Issues

**Problem:** Animation doesn't play
**Solution:** Check if animation manager is being updated, verify delta_time calculation

**Problem:** Animated values are always 0
**Solution:**
- Verify `html_update_and_layout()` is called with non-zero delta_time
- Check animation keyframes are properly parsed
- Ensure animation properties are correctly set (duration, delay)

**Problem:** Gradient looks wrong
**Solution:** Check if gradient parsing succeeded, verify color format

**Problem:** Performance issues
**Solution:** Reduce number of concurrent animations, check texture cache size

## References

- CSS Gradients: https://developer.mozilla.org/en-US/docs/Web/CSS/gradient
- CSS Animations: https://developer.mozilla.org/en-US/docs/Web/CSS/CSS_Animations
- Easing Functions: https://easings.net/
