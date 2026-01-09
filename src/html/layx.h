#ifndef LAYX_INCLUDE_HEADER
#define LAYX_INCLUDE_HEADER
#define LAYX_FLOAT 1

// Users of this library can define LAYX_IMPLEMENTATION in exactly one C or C++ file
// before you include layx.h.
//
// Your includes should look like this:
//   #define LAYX_IMPLEMENTATION
//   #include "layx.h"

#include <stdint.h>

#ifndef LAYX_EXPORT
#define LAYX_EXPORT extern
#endif

// Users of this library can define LAYX_ASSERT if they would like to use an
// assert other than the one from assert.h.
#ifndef LAYX_ASSERT
#include <assert.h>
#define LAYX_ASSERT assert
#endif

// 'static inline' for things we always want inlined
#if defined(__GNUC__) || defined(__clang__)
#define LAYX_STATIC_INLINE __attribute__((always_inline)) static inline
#elif defined(_MSC_VER)
#define LAYX_STATIC_INLINE __forceinline static
#else
#define LAYX_STATIC_INLINE inline static
#endif

typedef uint32_t layx_id;
#if LAYX_FLOAT == 1
typedef float layx_scalar;
#else
typedef int16_t layx_scalar;
#endif

#define LAYX_INVALID_ID UINT32_MAX

// Vector types
#if defined(__GNUC__) || defined(__clang__)
#ifdef LAYX_FLOAT
typedef float layx_vec4 __attribute__ ((__vector_size__ (16), aligned(4)));
typedef float layx_vec2 __attribute__ ((__vector_size__ (8), aligned(4)));
#else
typedef int16_t layx_vec4 __attribute__ ((__vector_size__ (8), aligned(2)));
typedef int16_t layx_vec2 __attribute__ ((__vector_size__ (4), aligned(2)));
#endif
#elif defined(_MSC_VER)
struct layx_vec4 {
    layx_scalar xyzw[4];
    const layx_scalar& operator[](int index) const
    { return xyzw[index]; }
    layx_scalar& operator[](int index)
    { return xyzw[index]; }
};
struct layx_vec2 {
    layx_scalar xy[2];
    const layx_scalar& operator[](int index) const
    { return xy[index]; }
    layx_scalar& operator[](int index)
    { return xy[index]; }
};
#endif

// Item structure
typedef struct layx_item_t {
    uint32_t flags;
    layx_id first_child;
    layx_id next_sibling;
    layx_vec4 margins;
    layx_vec4 padding;
    layx_vec4 border;
    layx_vec2 size;
    layx_vec2 min_size;
    layx_vec2 max_size;
    layx_scalar flex_grow;
    layx_scalar flex_shrink;
    layx_scalar flex_basis;
} layx_item_t;

// Context structure
typedef struct layx_context {
    layx_item_t *items;
    layx_vec4 *rects;
    layx_id capacity;
    layx_id count;
} layx_context;

// Display property
typedef enum layx_display {
    LAYX_DISPLAY_BLOCK,
    LAYX_DISPLAY_FLEX
} layx_display;

// Flex direction
typedef enum layx_flex_direction {
    LAYX_FLEX_DIRECTION_ROW,
    LAYX_FLEX_DIRECTION_COLUMN,
    LAYX_FLEX_DIRECTION_ROW_REVERSE,
    LAYX_FLEX_DIRECTION_COLUMN_REVERSE
} layx_flex_direction;

// Flex wrap (bits 3-4)
typedef enum layx_flex_wrap {
    LAYX_FLEX_WRAP_NOWRAP = 0 << 3,  // 0x0000
    LAYX_FLEX_WRAP_WRAP = 1 << 3,    // 0x0008
    LAYX_FLEX_WRAP_WRAP_REVERSE = 2 << 3  // 0x0010
} layx_flex_wrap;

// Justify content (bits 5-7)
typedef enum layx_justify_content {
    LAYX_JUSTIFY_FLEX_START = 0x000,
    LAYX_JUSTIFY_CENTER = 0x0020,
    LAYX_JUSTIFY_FLEX_END = 0x0040,
    LAYX_JUSTIFY_SPACE_BETWEEN = 0x0060,
    LAYX_JUSTIFY_SPACE_AROUND = 0x0080,
    LAYX_JUSTIFY_SPACE_EVENLY = 0x00A0
} layx_justify_content;

// Align items (bits 8-10)
typedef enum layx_align_items {
    LAYX_ALIGN_ITEMS_STRETCH = 0x0000,
    LAYX_ALIGN_ITEMS_FLEX_START = 0x0100,
    LAYX_ALIGN_ITEMS_CENTER = 0x0200,
    LAYX_ALIGN_ITEMS_FLEX_END = 0x0300,
    LAYX_ALIGN_ITEMS_BASELINE = 0x0400
} layx_align_items;

// Align content (bits 11-13)
typedef enum layx_align_content {
    LAYX_ALIGN_CONTENT_STRETCH = 0x0000,
    LAYX_ALIGN_CONTENT_FLEX_START = 0x0800,
    LAYX_ALIGN_CONTENT_CENTER = 0x1000,
    LAYX_ALIGN_CONTENT_FLEX_END = 0x1800,
    LAYX_ALIGN_CONTENT_SPACE_BETWEEN = 0x2000,
    LAYX_ALIGN_CONTENT_SPACE_AROUND = 0x2800
} layx_align_content;


// Align self (bits 14-16)
typedef enum layx_align_self {
    LAYX_ALIGN_SELF_AUTO = 0x0000,
    LAYX_ALIGN_SELF_FLEX_START = 0x04000,
    LAYX_ALIGN_SELF_CENTER = 0x08000,
    LAYX_ALIGN_SELF_FLEX_END = 0x0C000,
    LAYX_ALIGN_SELF_STRETCH = 0x10000
} layx_align_self;

// Style structure
typedef struct layx_style {
    layx_display display;
    layx_flex_direction flex_direction;
    layx_flex_wrap flex_wrap;
    layx_justify_content justify_content;
    layx_align_items align_items;
    layx_align_content align_content;
    layx_scalar width, height;
    layx_scalar min_width, min_height;
    layx_scalar max_width, max_height;
    layx_scalar margin_top, margin_right, margin_bottom, margin_left;
    layx_scalar padding_top, padding_right, padding_bottom, padding_left;
    layx_scalar border_top, border_right, border_bottom, border_left;
    layx_align_self align_self;
    layx_scalar flex_grow, flex_shrink, flex_basis;
} layx_style;

// Bit masks for internal use
// Bit layout:
// Bits 0-1: FLEX_DIRECTION (0x0003)
// Bit 2: LAYOUT_MODEL (0x0004)
// Bits 3-4: FLEX_WRAP (0x0018)
// Bits 5-7: JUSTIFY_CONTENT (0x00E0)
// Bits 8-10: ALIGN_ITEMS (0x0700)
// Bits 11-13: ALIGN_CONTENT (0x3800)
// Bits 14-16: ALIGN_SELF (0x1C000)
// Bit 17: ITEM_INSERTED (0x20000)
// Bit 18: SIZE_FIXED_WIDTH (0x40000)
// Bit 19: SIZE_FIXED_HEIGHT (0x80000)
// Bit 20: BREAK (0x100000)

#define LAYX_FLEX_DIRECTION_MASK    0x0003
#define LAYX_LAYOUT_MODEL_MASK      0x0004
#define LAYX_FLEX_WRAP_MASK         0x0018
#define LAYX_JUSTIFY_CONTENT_MASK   0x00E0
#define LAYX_ALIGN_ITEMS_MASK       0x0700
#define LAYX_ALIGN_CONTENT_MASK     0x3800
#define LAYX_ALIGN_SELF_MASK        0x1C000  // Use bits 14-16 for align-self

// Internal flags
enum {
    LAYX_ITEM_INSERTED = 0x20000,
    LAYX_SIZE_FIXED_WIDTH = 0x40000,
    LAYX_SIZE_FIXED_HEIGHT = 0x80000,
    LAYX_SIZE_FIXED_MASK = LAYX_SIZE_FIXED_WIDTH | LAYX_SIZE_FIXED_HEIGHT,
    LAYX_BREAK = 0x100000
};

// Internal alignment bit flags (for align-self implementation)
enum {
    LAYX_FILL_HORIZONTAL = 0x0A0,      // fill to both left and right (0x020 | 0x080)
    LAYX_FILL_VERTICAL = 0x140          // fill to both top and bottom (0x040 | 0x100)
};

// Context management
LAYX_EXPORT void layx_init_context(layx_context *ctx);
LAYX_EXPORT void layx_reserve_items_capacity(layx_context *ctx, layx_id count);
LAYX_EXPORT void layx_destroy_context(layx_context *ctx);
LAYX_EXPORT void layx_reset_context(layx_context *ctx);

// Layout calculation
LAYX_EXPORT void layx_run_context(layx_context *ctx);
LAYX_EXPORT void layx_run_item(layx_context *ctx, layx_id item);
LAYX_EXPORT void layx_clear_item_break(layx_context *ctx, layx_id item);

// Item management
LAYX_EXPORT layx_id layx_items_count(layx_context *ctx);
LAYX_EXPORT layx_id layx_items_capacity(layx_context *ctx);
LAYX_EXPORT layx_id layx_item(layx_context *ctx);
LAYX_EXPORT int layx_is_inserted(layx_context *ctx, layx_id child);
LAYX_EXPORT void layx_insert(layx_context *ctx, layx_id parent, layx_id child);
LAYX_EXPORT void layx_append(layx_context *ctx, layx_id earlier, layx_id later);
LAYX_EXPORT void layx_push(layx_context *ctx, layx_id parent, layx_id new_child);

// Display property
LAYX_EXPORT void layx_set_display(layx_context *ctx, layx_id item, layx_display display);
LAYX_EXPORT const char* layx_get_display_string(layx_display display);

// Flex properties
LAYX_EXPORT void layx_set_flex_direction(layx_context *ctx, layx_id item, layx_flex_direction direction);
LAYX_EXPORT void layx_set_flex_wrap(layx_context *ctx, layx_id item, layx_flex_wrap wrap);
LAYX_EXPORT void layx_set_justify_content(layx_context *ctx, layx_id item, layx_justify_content justify);
LAYX_EXPORT void layx_set_align_items(layx_context *ctx, layx_id item, layx_align_items align);
LAYX_EXPORT void layx_set_align_content(layx_context *ctx, layx_id item, layx_align_content align);
LAYX_EXPORT void layx_set_flex(layx_context *ctx, layx_id item,
                                layx_flex_direction direction,
                                layx_flex_wrap wrap,
                                layx_justify_content justify,
                                layx_align_items align_items,
                                layx_align_content align_content);

// String getters for flex properties
LAYX_EXPORT const char* layx_get_flex_direction_string(layx_flex_direction direction);
LAYX_EXPORT const char* layx_get_flex_wrap_string(layx_flex_wrap wrap);
LAYX_EXPORT const char* layx_get_justify_content_string(layx_justify_content justify);
LAYX_EXPORT const char* layx_get_align_items_string(layx_align_items align);
LAYX_EXPORT const char* layx_get_align_content_string(layx_align_content align);

// Size properties
LAYX_EXPORT void layx_set_width(layx_context *ctx, layx_id item, layx_scalar width);
LAYX_EXPORT void layx_set_height(layx_context *ctx, layx_id item, layx_scalar height);
LAYX_EXPORT void layx_set_min_width(layx_context *ctx, layx_id item, layx_scalar min_width);
LAYX_EXPORT void layx_set_min_height(layx_context *ctx, layx_id item, layx_scalar min_height);
LAYX_EXPORT void layx_set_max_width(layx_context *ctx, layx_id item, layx_scalar max_width);
LAYX_EXPORT void layx_set_max_height(layx_context *ctx, layx_id item, layx_scalar max_height);
LAYX_EXPORT void layx_set_size(layx_context *ctx, layx_id item, layx_scalar width, layx_scalar height);
LAYX_EXPORT layx_vec2 layx_get_size(layx_context *ctx, layx_id item);

// Flex item properties
LAYX_EXPORT void layx_set_flex_grow(layx_context *ctx, layx_id item, layx_scalar grow);
LAYX_EXPORT void layx_set_flex_shrink(layx_context *ctx, layx_id item, layx_scalar shrink);
LAYX_EXPORT void layx_set_flex_basis(layx_context *ctx, layx_id item, layx_scalar basis);
LAYX_EXPORT void layx_set_flex_properties(layx_context *ctx, layx_id item,
                                         layx_scalar grow, layx_scalar shrink, layx_scalar basis);

// Align self
LAYX_EXPORT void layx_set_align_self(layx_context *ctx, layx_id item, layx_align_self align);

// Box model properties
LAYX_EXPORT void layx_set_margin(layx_context *ctx, layx_id item, layx_scalar value);
LAYX_EXPORT void layx_set_margin_top(layx_context *ctx, layx_id item, layx_scalar top);
LAYX_EXPORT void layx_set_margin_right(layx_context *ctx, layx_id item, layx_scalar right);
LAYX_EXPORT void layx_set_margin_bottom(layx_context *ctx, layx_id item, layx_scalar bottom);
LAYX_EXPORT void layx_set_margin_left(layx_context *ctx, layx_id item, layx_scalar left);
LAYX_EXPORT void layx_set_margin_trbl(layx_context *ctx, layx_id item,
                                layx_scalar top, layx_scalar right,
                                layx_scalar bottom, layx_scalar left);
LAYX_EXPORT void layx_set_margin_ltrb(layx_context *ctx, layx_id item,
                                layx_scalar left, layx_scalar top,
                                layx_scalar right, layx_scalar bottom);

LAYX_EXPORT void layx_set_padding(layx_context *ctx, layx_id item, layx_scalar value);
LAYX_EXPORT void layx_set_padding_top(layx_context *ctx, layx_id item, layx_scalar top);
LAYX_EXPORT void layx_set_padding_right(layx_context *ctx, layx_id item, layx_scalar right);
LAYX_EXPORT void layx_set_padding_bottom(layx_context *ctx, layx_id item, layx_scalar bottom);
LAYX_EXPORT void layx_set_padding_left(layx_context *ctx, layx_id item, layx_scalar left);
LAYX_EXPORT void layx_set_padding_trbl(layx_context *ctx, layx_id item,
                                 layx_scalar top, layx_scalar right,
                                 layx_scalar bottom, layx_scalar left);
LAYX_EXPORT void layx_set_padding_ltrb(layx_context *ctx, layx_id item,
                                 layx_scalar left, layx_scalar top,
                                 layx_scalar right, layx_scalar bottom);

LAYX_EXPORT void layx_set_border(layx_context *ctx, layx_id item, layx_scalar value);
LAYX_EXPORT void layx_set_border_top(layx_context *ctx, layx_id item, layx_scalar top);
LAYX_EXPORT void layx_set_border_right(layx_context *ctx, layx_id item, layx_scalar right);
LAYX_EXPORT void layx_set_border_bottom(layx_context *ctx, layx_id item, layx_scalar bottom);
LAYX_EXPORT void layx_set_border_left(layx_context *ctx, layx_id item, layx_scalar left);
LAYX_EXPORT void layx_set_border_trbl(layx_context *ctx, layx_id item,
                                 layx_scalar top, layx_scalar right,
                                 layx_scalar bottom, layx_scalar left);
LAYX_EXPORT void layx_set_border_ltrb(layx_context *ctx, layx_id item,
                                 layx_scalar left, layx_scalar top,
                                 layx_scalar right, layx_scalar bottom);


// Getters for box model
LAYX_EXPORT void layx_get_margin_ltrb(layx_context *ctx, layx_id item, layx_scalar *left, layx_scalar *top, layx_scalar *right, layx_scalar *bottom);
LAYX_EXPORT void layx_get_padding_ltrb(layx_context *ctx, layx_id item, layx_scalar *left, layx_scalar *top, layx_scalar *right, layx_scalar *bottom);
LAYX_EXPORT void layx_get_border_ltrb(layx_context *ctx, layx_id item, layx_scalar *left, layx_scalar *top, layx_scalar *right, layx_scalar *bottom);

// Style application
LAYX_EXPORT void layx_style_reset(layx_style *style);
LAYX_EXPORT void layx_apply_style(layx_context *ctx, layx_id item, const layx_style *style);
LAYX_EXPORT layx_id layx_create_item_with_style(layx_context *ctx, const layx_style *style);

// Inline helpers
LAYX_STATIC_INLINE layx_vec4 layx_vec4_xyzw(layx_scalar x, layx_scalar y, layx_scalar z, layx_scalar w)
{
#if (defined(__GNUC__) || defined(__clang__)) && !defined(__cplusplus)
    return (layx_vec4){x, y, z, w};
#else
    layx_vec4 result;
    result[0] = x;
    result[1] = y;
    result[2] = z;
    result[3] = w;
    return result;
#endif
}

LAYX_STATIC_INLINE layx_item_t *layx_get_item(const layx_context *ctx, layx_id id)
{
    LAYX_ASSERT(id != LAYX_INVALID_ID && id < ctx->count);
    return ctx->items + id;
}

LAYX_STATIC_INLINE layx_id layx_first_child(const layx_context *ctx, layx_id id)
{
    const layx_item_t *pitem = layx_get_item(ctx, id);
    return pitem->first_child;
}

LAYX_STATIC_INLINE layx_id layx_next_sibling(const layx_context *ctx, layx_id id)
{
    const layx_item_t *pitem = layx_get_item(ctx, id);
    return pitem->next_sibling;
}

LAYX_STATIC_INLINE layx_vec4 layx_get_rect(const layx_context *ctx, layx_id id)
{
    LAYX_ASSERT(id != LAYX_INVALID_ID && id < ctx->count);
    return ctx->rects[id];
}

LAYX_STATIC_INLINE void layx_get_rect_xywh(
        const layx_context *ctx, layx_id id,
        layx_scalar *x, layx_scalar *y, layx_scalar *width, layx_scalar *height)
{
    LAYX_ASSERT(id != LAYX_INVALID_ID && id < ctx->count);
    layx_vec4 rect = ctx->rects[id];
    *x = rect[0];
    *y = rect[1];
    *width = rect[2];
    *height = rect[3];
}

// Debug functions
LAYX_EXPORT const char* layx_get_layout_properties_string(layx_context *ctx, layx_id item);
LAYX_EXPORT const char* layx_get_item_alignment_string(layx_context *ctx, layx_id item);

#undef LAYX_EXPORT
#undef LAYX_STATIC_INLINE

#endif // LAYX_INCLUDE_HEADER