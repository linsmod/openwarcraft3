#define LAYX_IMPLEMENTATION
#include "layx.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef LAYX_REALLOC
#define LAYX_REALLOC(_block, _size) realloc(_block, _size)
#define LAYX_FREE(_block) free(_block)
#endif

#ifndef LAYX_MEMSET
#define LAYX_MEMSET(_dst, _val, _size) memset(_dst, _val, _size)
#endif

#if defined(__GNUC__) || defined(__clang__)
#define LAYX_FORCE_INLINE __attribute__((always_inline)) inline
#ifdef __cplusplus
#define LAYX_RESTRICT __restrict
#else
#define LAYX_RESTRICT restrict
#endif
#elif defined(_MSC_VER)
#define LAYX_FORCE_INLINE __forceinline
#define LAYX_RESTRICT __restrict
#else
#define LAYX_FORCE_INLINE inline
#ifdef __cplusplus
#define LAYX_RESTRICT
#else
#define LAYX_RESTRICT restrict
#endif
#endif

// Math utilities
static LAYX_FORCE_INLINE layx_scalar layx_scalar_max(layx_scalar a, layx_scalar b)
{ return a > b ? a : b; }
static LAYX_FORCE_INLINE layx_scalar layx_scalar_min(layx_scalar a, layx_scalar b)
{ return a < b ? a : b; }
static LAYX_FORCE_INLINE float layx_float_max(float a, float b)
{ return a > b ? a : b; }
static LAYX_FORCE_INLINE float layx_float_min(float a, float b)
{ return a < b ? a : b; }

// Helper functions to convert new enums to internal flags
static LAYX_FORCE_INLINE uint32_t layx_display_to_flags(layx_display display) {
    return (display == LAYX_DISPLAY_FLEX) ? LAYX_LAYOUT_MODEL_MASK : 0;
}

static LAYX_FORCE_INLINE uint32_t layx_flex_direction_to_flags(layx_flex_direction direction) {
    return (uint32_t)direction & LAYX_FLEX_DIRECTION_MASK;
}

static LAYX_FORCE_INLINE uint32_t layx_flex_wrap_to_flags(layx_flex_wrap wrap) {
    return (uint32_t)wrap & LAYX_FLEX_WRAP_MASK;
}

static LAYX_FORCE_INLINE uint32_t layx_justify_content_to_flags(layx_justify_content justify) {
    return (uint32_t)justify;
}

static LAYX_FORCE_INLINE uint32_t layx_align_items_to_flags(layx_align_items align) {
    return (uint32_t)align;
}

static LAYX_FORCE_INLINE uint32_t layx_align_content_to_flags(layx_align_content align) {
    return (uint32_t)align;
}

static LAYX_FORCE_INLINE uint32_t layx_align_self_to_flags(layx_align_self align) {
    return (uint32_t)align;
}

// Helper to check if flex container
static LAYX_FORCE_INLINE bool layx_is_flex_container(uint32_t flags) {
    return (flags & LAYX_LAYOUT_MODEL_MASK) != 0;
}

// Helper to get direction dimension
static LAYX_FORCE_INLINE int layx_get_direction_dim(uint32_t flags) {
    layx_flex_direction dir = (layx_flex_direction)(flags & LAYX_FLEX_DIRECTION_MASK);
    return (dir == LAYX_FLEX_DIRECTION_ROW || dir == LAYX_FLEX_DIRECTION_ROW_REVERSE) ? 0 : 1;
}

// Context management
void layx_init_context(layx_context *ctx)
{
    ctx->capacity = 0;
    ctx->count = 0;
    ctx->items = NULL;
    ctx->rects = NULL;
}

void layx_reserve_items_capacity(layx_context *ctx, layx_id count)
{
    if (count >= ctx->capacity) {
        ctx->capacity = count;
        const size_t item_size = sizeof(layx_item_t) + sizeof(layx_vec4);
        ctx->items = (layx_item_t*)LAYX_REALLOC(ctx->items, ctx->capacity * item_size);
        const layx_item_t *past_last = ctx->items + ctx->capacity;
        ctx->rects = (layx_vec4*)past_last;
    }
}

void layx_destroy_context(layx_context *ctx)
{
    if (ctx->items != NULL) {
        LAYX_FREE(ctx->items);
        ctx->items = NULL;
        ctx->rects = NULL;
    }
}

void layx_reset_context(layx_context *ctx)
{ ctx->count = 0; }

// Layout calculation declarations
static void layx_calc_size(layx_context *ctx, layx_id item, int dim);
static void layx_arrange(layx_context *ctx, layx_id item, int dim);

void layx_run_context(layx_context *ctx)
{
    LAYX_ASSERT(ctx != NULL);
    if (ctx->count > 0) {
        layx_run_item(ctx, 0);
    }
}

void layx_run_item(layx_context *ctx, layx_id item)
{
    LAYX_ASSERT(ctx != NULL);
    layx_calc_size(ctx, item, 0);
    layx_arrange(ctx, item, 0);
    layx_calc_size(ctx, item, 1);
    layx_arrange(ctx, item, 1);
}

void layx_clear_item_break(layx_context *ctx, layx_id item)
{
    LAYX_ASSERT(ctx != NULL);
    layx_item_t *pitem = layx_get_item(ctx, item);
    pitem->flags = pitem->flags & ~LAYX_BREAK;
}

layx_id layx_items_count(layx_context *ctx)
{
    LAYX_ASSERT(ctx != NULL);
    return ctx->count;
}

layx_id layx_items_capacity(layx_context *ctx)
{
    LAYX_ASSERT(ctx != NULL);
    return ctx->capacity;
}

layx_id layx_item(layx_context *ctx)
{
    layx_id idx = ctx->count++;
    if (idx >= ctx->capacity) {
        ctx->capacity = ctx->capacity < 1 ? 32 : (ctx->capacity * 4);
        const size_t item_size = sizeof(layx_item_t) + sizeof(layx_vec4);
        ctx->items = (layx_item_t*)LAYX_REALLOC(ctx->items, ctx->capacity * item_size);
        const layx_item_t *past_last = ctx->items + ctx->capacity;
        ctx->rects = (layx_vec4*)past_last;
    }
    layx_item_t *item = layx_get_item(ctx, idx);
    LAYX_MEMSET(item, 0, sizeof(layx_item_t));
    item->first_child = LAYX_INVALID_ID;
    item->next_sibling = LAYX_INVALID_ID;
    item->min_size[0] = 0; item->min_size[1] = 0;
    item->max_size[0] = 0; item->max_size[1] = 0;
    item->flex_grow = 0;
    item->flex_shrink = 1;  // CSS规范: flex-shrink默认为1
    item->flex_basis = 0;
    LAYX_MEMSET(&ctx->rects[idx], 0, sizeof(layx_vec4));
    return idx;
}

static LAYX_FORCE_INLINE
void layx_append_by_ptr(
        layx_item_t *LAYX_RESTRICT pearlier,
        layx_id later, layx_item_t *LAYX_RESTRICT plater)
{
    plater->next_sibling = pearlier->next_sibling;
    plater->flags |= LAYX_ITEM_INSERTED;
    pearlier->next_sibling = later;
}

layx_id layx_last_child(const layx_context *ctx, layx_id parent)
{
    layx_item_t *pparent = layx_get_item(ctx, parent);
    layx_id child = pparent->first_child;
    if (child == LAYX_INVALID_ID) return LAYX_INVALID_ID;
    layx_item_t *pchild = layx_get_item(ctx, child);
    layx_id result = child;
    for (;;) {
        layx_id next = pchild->next_sibling;
        if (next == LAYX_INVALID_ID) break;
        result = next;
        pchild = layx_get_item(ctx, next);
    }
    return result;
}

void layx_append(layx_context *ctx, layx_id earlier, layx_id later)
{
    LAYX_ASSERT(later != 0);
    LAYX_ASSERT(earlier != later);
    layx_item_t *LAYX_RESTRICT pearlier = layx_get_item(ctx, earlier);
    layx_item_t *LAYX_RESTRICT plater = layx_get_item(ctx, later);
    layx_append_by_ptr(pearlier, later, plater);
}

int layx_is_inserted(layx_context *ctx, layx_id child){
    LAYX_ASSERT(child != 0);
    layx_item_t *LAYX_RESTRICT pchild = layx_get_item(ctx, child);
    return pchild->flags & LAYX_ITEM_INSERTED;
}

void layx_insert(layx_context *ctx, layx_id parent, layx_id child)
{
    LAYX_ASSERT(child != 0);
    LAYX_ASSERT(parent != child);
    layx_item_t *LAYX_RESTRICT pparent = layx_get_item(ctx, parent);
    layx_item_t *LAYX_RESTRICT pchild = layx_get_item(ctx, child);
    LAYX_ASSERT(!(pchild->flags & LAYX_ITEM_INSERTED));
    if (pparent->first_child == LAYX_INVALID_ID) {
        pparent->first_child = child;
        pchild->flags |= LAYX_ITEM_INSERTED;
    } else {
        layx_id next = pparent->first_child;
        layx_item_t *LAYX_RESTRICT pnext = layx_get_item(ctx, next);
        for (;;) {
            next = pnext->next_sibling;
            if (next == LAYX_INVALID_ID) break;
            pnext = layx_get_item(ctx, next);
        }
        layx_append_by_ptr(pnext, child, pchild);
    }
}

void layx_push(layx_context *ctx, layx_id parent, layx_id new_child)
{
    LAYX_ASSERT(new_child != 0);
    LAYX_ASSERT(parent != new_child);
    layx_item_t *LAYX_RESTRICT pparent = layx_get_item(ctx, parent);
    layx_id old_child = pparent->first_child;
    layx_item_t *LAYX_RESTRICT pchild = layx_get_item(ctx, new_child);
    LAYX_ASSERT(!(pchild->flags & LAYX_ITEM_INSERTED));
    pparent->first_child = new_child;
    pchild->flags |= LAYX_ITEM_INSERTED;
    pchild->next_sibling = old_child;
}

// Display property
void layx_set_display(layx_context *ctx, layx_id item, layx_display display)
{
    layx_item_t *pitem = layx_get_item(ctx, item);
    uint32_t flags = pitem->flags;
    flags &= ~LAYX_LAYOUT_MODEL_MASK;
    flags |= layx_display_to_flags(display);
    pitem->flags = flags;
}

const char* layx_get_display_string(layx_display display) {
    switch (display) {
        case LAYX_DISPLAY_BLOCK: return "BLOCK";
        case LAYX_DISPLAY_FLEX: return "FLEX";
        default: return "UNKNOWN";
    }
}

// Helper to get display from flags
static LAYX_FORCE_INLINE layx_display layx_get_display_from_flags(uint32_t flags) {
    return (flags & LAYX_LAYOUT_MODEL_MASK) ? LAYX_DISPLAY_FLEX : LAYX_DISPLAY_BLOCK;
}

// Flex properties
void layx_set_flex_direction(layx_context *ctx, layx_id item, layx_flex_direction direction)
{
    layx_item_t *pitem = layx_get_item(ctx, item);
    uint32_t flags = pitem->flags;
    flags &= ~LAYX_FLEX_DIRECTION_MASK;
    flags |= layx_flex_direction_to_flags(direction);
    pitem->flags = flags;
}

void layx_set_flex_wrap(layx_context *ctx, layx_id item, layx_flex_wrap wrap)
{
    layx_item_t *pitem = layx_get_item(ctx, item);
    uint32_t flags = pitem->flags;
    flags &= ~LAYX_FLEX_WRAP_MASK;
    flags |= layx_flex_wrap_to_flags(wrap);
    pitem->flags = flags;
}

void layx_set_justify_content(layx_context *ctx, layx_id item, layx_justify_content justify)
{
    layx_item_t *pitem = layx_get_item(ctx, item);
    uint32_t flags = pitem->flags;
    flags &= ~LAYX_JUSTIFY_CONTENT_MASK;
    flags |= layx_justify_content_to_flags(justify);
    pitem->flags = flags;
}

void layx_set_align_items(layx_context *ctx, layx_id item, layx_align_items align)
{
    layx_item_t *pitem = layx_get_item(ctx, item);
    uint32_t flags = pitem->flags;
    flags &= ~LAYX_ALIGN_ITEMS_MASK;
    flags |= layx_align_items_to_flags(align);
    pitem->flags = flags;
}

void layx_set_align_content(layx_context *ctx, layx_id item, layx_align_content align)
{
    layx_item_t *pitem = layx_get_item(ctx, item);
    uint32_t flags = pitem->flags;
    flags &= ~LAYX_ALIGN_CONTENT_MASK;
    flags |= layx_align_content_to_flags(align);
    pitem->flags = flags;
}

void layx_set_flex(layx_context *ctx, layx_id item,
                    layx_flex_direction direction,
                    layx_flex_wrap wrap,
                    layx_justify_content justify,
                    layx_align_items align_items,
                    layx_align_content align_content)
{
    layx_set_display(ctx, item, LAYX_DISPLAY_FLEX);
    layx_set_flex_direction(ctx, item, direction);
    layx_set_flex_wrap(ctx, item, wrap);
    layx_set_justify_content(ctx, item, justify);
    layx_set_align_items(ctx, item, align_items);
    layx_set_align_content(ctx, item, align_content);
}

// String getters
const char* layx_get_flex_direction_string(layx_flex_direction direction) {
    switch (direction) {
        case LAYX_FLEX_DIRECTION_ROW: return "ROW";
        case LAYX_FLEX_DIRECTION_COLUMN: return "COLUMN";
        case LAYX_FLEX_DIRECTION_ROW_REVERSE: return "ROW_REVERSE";
        case LAYX_FLEX_DIRECTION_COLUMN_REVERSE: return "COLUMN_REVERSE";
        default: return "UNKNOWN";
    }
}

const char* layx_get_flex_wrap_string(layx_flex_wrap wrap) {
    switch (wrap) {
        case LAYX_FLEX_WRAP_NOWRAP: return "NOWRAP";
        case LAYX_FLEX_WRAP_WRAP: return "WRAP";
        case LAYX_FLEX_WRAP_WRAP_REVERSE: return "WRAP_REVERSE";
        default: return "UNKNOWN";
    }
}

const char* layx_get_justify_content_string(layx_justify_content justify) {
    switch (justify) {
        case LAYX_JUSTIFY_FLEX_START: return "FLEX_START";
        case LAYX_JUSTIFY_CENTER: return "CENTER";
        case LAYX_JUSTIFY_FLEX_END: return "FLEX_END";
        case LAYX_JUSTIFY_SPACE_BETWEEN: return "SPACE_BETWEEN";
        case LAYX_JUSTIFY_SPACE_AROUND: return "SPACE_AROUND";
        case LAYX_JUSTIFY_SPACE_EVENLY: return "SPACE_EVENLY";
        default: return "UNKNOWN";
    }
}

const char* layx_get_align_items_string(layx_align_items align) {
    switch (align) {
        case LAYX_ALIGN_ITEMS_STRETCH: return "STRETCH";
        case LAYX_ALIGN_ITEMS_FLEX_START: return "FLEX_START";
        case LAYX_ALIGN_ITEMS_CENTER: return "CENTER";
        case LAYX_ALIGN_ITEMS_FLEX_END: return "FLEX_END";
        case LAYX_ALIGN_ITEMS_BASELINE: return "BASELINE";
        default: return "UNKNOWN";
    }
}

const char* layx_get_align_content_string(layx_align_content align) {
    switch (align) {
        case LAYX_ALIGN_CONTENT_STRETCH: return "STRETCH";
        case LAYX_ALIGN_CONTENT_FLEX_START: return "FLEX_START";
        case LAYX_ALIGN_CONTENT_CENTER: return "CENTER";
        case LAYX_ALIGN_CONTENT_FLEX_END: return "FLEX_END";
        case LAYX_ALIGN_CONTENT_SPACE_BETWEEN: return "SPACE_BETWEEN";
        case LAYX_ALIGN_CONTENT_SPACE_AROUND: return "SPACE_AROUND";
        default: return "UNKNOWN";
    }
}

// Size properties
void layx_set_width(layx_context *ctx, layx_id item, layx_scalar width)
{
    layx_item_t *pitem = layx_get_item(ctx, item);
    pitem->size[0] = width;
    uint32_t flags = pitem->flags;
    if (width == 0)
        flags &= ~LAYX_SIZE_FIXED_WIDTH;
    else
        flags |= LAYX_SIZE_FIXED_WIDTH;
    pitem->flags = flags;
}

void layx_set_height(layx_context *ctx, layx_id item, layx_scalar height)
{
    layx_item_t *pitem = layx_get_item(ctx, item);
    pitem->size[1] = height;
    uint32_t flags = pitem->flags;
    if (height == 0)
        flags &= ~LAYX_SIZE_FIXED_HEIGHT;
    else
        flags |= LAYX_SIZE_FIXED_HEIGHT;
    pitem->flags = flags;
}

void layx_set_min_width(layx_context *ctx, layx_id item, layx_scalar min_width)
{
    layx_item_t *pitem = layx_get_item(ctx, item);
    pitem->min_size[0] = min_width;
}

void layx_set_min_height(layx_context *ctx, layx_id item, layx_scalar min_height)
{
    layx_item_t *pitem = layx_get_item(ctx, item);
    pitem->min_size[1] = min_height;
}

void layx_set_max_width(layx_context *ctx, layx_id item, layx_scalar max_width)
{
    layx_item_t *pitem = layx_get_item(ctx, item);
    pitem->max_size[0] = max_width;
}

void layx_set_max_height(layx_context *ctx, layx_id item, layx_scalar max_height)
{
    layx_item_t *pitem = layx_get_item(ctx, item);
    pitem->max_size[1] = max_height;
}

void layx_set_size(layx_context *ctx, layx_id item, layx_scalar width, layx_scalar height)
{
    layx_set_width(ctx, item, width);
    layx_set_height(ctx, item, height);
}

layx_vec2 layx_get_size(layx_context *ctx, layx_id item)
{
    layx_item_t *pitem = layx_get_item(ctx, item);
    return pitem->size;
}

// Flex item properties
void layx_set_flex_grow(layx_context *ctx, layx_id item, layx_scalar grow)
{
    layx_item_t *pitem = layx_get_item(ctx, item);
    pitem->flex_grow = grow;
}

void layx_set_flex_shrink(layx_context *ctx, layx_id item, layx_scalar shrink)
{
    layx_item_t *pitem = layx_get_item(ctx, item);
    pitem->flex_shrink = shrink;
}

void layx_set_flex_basis(layx_context *ctx, layx_id item, layx_scalar basis)
{
    layx_item_t *pitem = layx_get_item(ctx, item);
    pitem->flex_basis = basis;
}

void layx_set_flex_properties(layx_context *ctx, layx_id item,
                                 layx_scalar grow, layx_scalar shrink, layx_scalar basis)
{
    layx_set_flex_grow(ctx, item, grow);
    layx_set_flex_shrink(ctx, item, shrink);
    layx_set_flex_basis(ctx, item, basis);
}

// Align self
void layx_set_align_self(layx_context *ctx, layx_id item, layx_align_self align)
{
    layx_item_t *pitem = layx_get_item(ctx, item);
    uint32_t flags = pitem->flags;
    flags &= ~LAYX_ALIGN_SELF_MASK;
    flags |= layx_align_self_to_flags(align);
    pitem->flags = flags;
}

// Box model properties
void layx_set_margin(layx_context *ctx, layx_id item, layx_scalar value)
{
    layx_item_t *pitem = layx_get_item(ctx, item);
    pitem->margins[0] = value;
    pitem->margins[1] = value;
    pitem->margins[2] = value;
    pitem->margins[3] = value;
}

void layx_set_margin_top(layx_context *ctx, layx_id item, layx_scalar top)
{
    layx_item_t *pitem = layx_get_item(ctx, item);
    pitem->margins[1] = top;
}

void layx_set_margin_right(layx_context *ctx, layx_id item, layx_scalar right)
{
    layx_item_t *pitem = layx_get_item(ctx, item);
    pitem->margins[2] = right;
}

void layx_set_margin_bottom(layx_context *ctx, layx_id item, layx_scalar bottom)
{
    layx_item_t *pitem = layx_get_item(ctx, item);
    pitem->margins[3] = bottom;
}

void layx_set_margin_left(layx_context *ctx, layx_id item, layx_scalar left)
{
    layx_item_t *pitem = layx_get_item(ctx, item);
    pitem->margins[0] = left;
}

void layx_set_margin_trbl(layx_context *ctx, layx_id item,
                            layx_scalar top, layx_scalar right,
                            layx_scalar bottom, layx_scalar left)
{
    layx_item_t *pitem = layx_get_item(ctx, item);
    pitem->margins[1] = top;
    pitem->margins[2] = right;
    pitem->margins[3] = bottom;
    pitem->margins[0] = left;
}

void layx_set_margin_ltrb(layx_context *ctx, layx_id item,
                            layx_scalar left, layx_scalar top,
                            layx_scalar right, layx_scalar bottom)
{
    layx_item_t *pitem = layx_get_item(ctx, item);
    pitem->margins[0] = left;
    pitem->margins[1] = top;
    pitem->margins[2] = right;
    pitem->margins[3] = bottom;
}

void layx_set_padding(layx_context *ctx, layx_id item, layx_scalar value)
{
    layx_item_t *pitem = layx_get_item(ctx, item);
    pitem->padding[0] = value;
    pitem->padding[1] = value;
    pitem->padding[2] = value;
    pitem->padding[3] = value;
}

void layx_set_padding_top(layx_context *ctx, layx_id item, layx_scalar top)
{
    layx_item_t *pitem = layx_get_item(ctx, item);
    pitem->padding[1] = top;
}

void layx_set_padding_right(layx_context *ctx, layx_id item, layx_scalar right)
{
    layx_item_t *pitem = layx_get_item(ctx, item);
    pitem->padding[2] = right;
}

void layx_set_padding_bottom(layx_context *ctx, layx_id item, layx_scalar bottom)
{
    layx_item_t *pitem = layx_get_item(ctx, item);
    pitem->padding[3] = bottom;
}

void layx_set_padding_left(layx_context *ctx, layx_id item, layx_scalar left)
{
    layx_item_t *pitem = layx_get_item(ctx, item);
    pitem->padding[0] = left;
}

void layx_set_padding_trbl(layx_context *ctx, layx_id item,
                             layx_scalar top, layx_scalar right,
                             layx_scalar bottom, layx_scalar left)
{
    layx_item_t *pitem = layx_get_item(ctx, item);
    pitem->padding[1] = top;
    pitem->padding[2] = right;
    pitem->padding[3] = bottom;
    pitem->padding[0] = left;
}

void layx_set_padding_ltrb(layx_context *ctx, layx_id item,
                             layx_scalar left, layx_scalar top,
                             layx_scalar right, layx_scalar bottom)
{
    layx_item_t *pitem = layx_get_item(ctx, item);
    pitem->padding[0] = left;
    pitem->padding[1] = top;
    pitem->padding[2] = right;
    pitem->padding[3] = bottom;
}

void layx_set_border(layx_context *ctx, layx_id item, layx_scalar value)
{
    layx_item_t *pitem = layx_get_item(ctx, item);
    pitem->border[0] = value;
    pitem->border[1] = value;
    pitem->border[2] = value;
    pitem->border[3] = value;
}

void layx_set_border_top(layx_context *ctx, layx_id item, layx_scalar top)
{
    layx_item_t *pitem = layx_get_item(ctx, item);
    pitem->border[1] = top;
}

void layx_set_border_right(layx_context *ctx, layx_id item, layx_scalar right)
{
    layx_item_t *pitem = layx_get_item(ctx, item);
    pitem->border[2] = right;
}

void layx_set_border_bottom(layx_context *ctx, layx_id item, layx_scalar bottom)
{
    layx_item_t *pitem = layx_get_item(ctx, item);
    pitem->border[3] = bottom;
}

void layx_set_border_left(layx_context *ctx, layx_id item, layx_scalar left)
{
    layx_item_t *pitem = layx_get_item(ctx, item);
    pitem->border[0] = left;
}

void layx_set_border_trbl(layx_context *ctx, layx_id item,
                             layx_scalar top, layx_scalar right,
                             layx_scalar bottom, layx_scalar left)
{
    layx_item_t *pitem = layx_get_item(ctx, item);
    pitem->border[1] = top;
    pitem->border[2] = right;
    pitem->border[3] = bottom;
    pitem->border[0] = left;
}

void layx_set_border_ltrb(layx_context *ctx, layx_id item,
                             layx_scalar left, layx_scalar top,
                             layx_scalar right, layx_scalar bottom)
{
    layx_item_t *pitem = layx_get_item(ctx, item);
    pitem->border[0] = left;
    pitem->border[1] = top;
    pitem->border[2] = right;
    pitem->border[3] = bottom;
}


// Getters for box model
void layx_get_margin_ltrb(layx_context *ctx, layx_id item, layx_scalar *left, layx_scalar *top, layx_scalar *right, layx_scalar *bottom)
{
    layx_item_t *pitem = layx_get_item(ctx, item);
    layx_vec4 margins = pitem->margins;
    *left = margins[0];
    *top = margins[1];
    *right = margins[2];
    *bottom = margins[3];
}

void layx_get_padding_ltrb(layx_context *ctx, layx_id item, layx_scalar *left, layx_scalar *top, layx_scalar *right, layx_scalar *bottom)
{
    layx_item_t *pitem = layx_get_item(ctx, item);
    layx_vec4 padding = pitem->padding;
    *left = padding[0];
    *top = padding[1];
    *right = padding[2];
    *bottom = padding[3];
}

void layx_get_border_ltrb(layx_context *ctx, layx_id item, layx_scalar *left, layx_scalar *top, layx_scalar *right, layx_scalar *bottom)
{
    layx_item_t *pitem = layx_get_item(ctx, item);
    layx_vec4 border = pitem->border;
    *left = border[0];
    *top = border[1];
    *right = border[2];
    *bottom = border[3];
}

// Style application
void layx_style_reset(layx_style *style)
{
    LAYX_MEMSET(style, 0, sizeof(layx_style));
}

void layx_apply_style(layx_context *ctx, layx_id item, const layx_style *style)
{
    // Display
    if (style->display != 0) {
        layx_set_display(ctx, item, style->display);
    }
    
    // Flex properties
    if (style->flex_direction != 0) {
        layx_set_flex_direction(ctx, item, style->flex_direction);
    }
    if (style->flex_wrap != 0) {
        layx_set_flex_wrap(ctx, item, style->flex_wrap);
    }
    if (style->justify_content != 0) {
        layx_set_justify_content(ctx, item, style->justify_content);
    }
    if (style->align_items != 0) {
        layx_set_align_items(ctx, item, style->align_items);
    }
    if (style->align_content != 0) {
        layx_set_align_content(ctx, item, style->align_content);
    }
    
    // Size properties
    if (style->width != 0 || style->height != 0) {
        layx_set_size(ctx, item, style->width, style->height);
    }
    if (style->min_width != 0) {
        layx_set_min_width(ctx, item, style->min_width);
    }
    if (style->min_height != 0) {
        layx_set_min_height(ctx, item, style->min_height);
    }
    if (style->max_width != 0) {
        layx_set_max_width(ctx, item, style->max_width);
    }
    if (style->max_height != 0) {
        layx_set_max_height(ctx, item, style->max_height);
    }
    
    // Box model
    if (style->margin_top != 0 || style->margin_right != 0 || 
        style->margin_bottom != 0 || style->margin_left != 0) {
        layx_set_margin_trbl(ctx, item, 
                            style->margin_top, style->margin_right,
                            style->margin_bottom, style->margin_left);
    }
    if (style->padding_top != 0 || style->padding_right != 0 || 
        style->padding_bottom != 0 || style->padding_left != 0) {
        layx_set_padding_trbl(ctx, item,
                             style->padding_top, style->padding_right,
                             style->padding_bottom, style->padding_left);
    }
    if (style->border_top != 0 || style->border_right != 0 || 
        style->border_bottom != 0 || style->border_left != 0) {
        layx_set_border_trbl(ctx, item,
                             style->border_top, style->border_right,
                             style->border_bottom, style->border_left);
    }
    
    // Flex item properties
    if (style->flex_grow != 0 || style->flex_shrink != 0 || style->flex_basis != 0) {
        layx_set_flex_properties(ctx, item, style->flex_grow, style->flex_shrink, style->flex_basis);
    }
    
    // Align self
    if (style->align_self != 0) {
        layx_set_align_self(ctx, item, style->align_self);
    }
}

layx_id layx_create_item_with_style(layx_context *ctx, const layx_style *style)
{
    layx_id item = layx_item(ctx);
    layx_apply_style(ctx, item, style);
    return item;
}

// Helper function to get internal space available for children
static LAYX_FORCE_INLINE
layx_scalar layx_get_internal_space(
        layx_context *ctx, layx_id item, int dim)
{
    layx_item_t *pitem = layx_get_item(ctx, item);
    layx_vec4 rect = ctx->rects[item];
    return rect[2 + dim] - pitem->padding[dim] - pitem->border[dim] 
                           - pitem->padding[dim + 2] - pitem->border[dim + 2];
}

// Helper function to get offset where children should be positioned
static LAYX_FORCE_INLINE
layx_scalar layx_get_content_offset(
        layx_context *ctx, layx_id item, int dim)
{
    layx_item_t *pitem = layx_get_item(ctx, item);
    layx_vec4 rect = ctx->rects[item];
    return rect[dim] + pitem->margins[dim] + pitem->padding[dim] + pitem->border[dim];
}

// Helper to calculate overlayed size
static LAYX_FORCE_INLINE
layx_scalar layx_calc_overlayed_size(
        layx_context *ctx, layx_id item, int dim)
{
    const int wdim = dim + 2;
    layx_item_t *LAYX_RESTRICT pitem = layx_get_item(ctx, item);
    layx_scalar need_size = 0;
    layx_id child = pitem->first_child;
    while (child != LAYX_INVALID_ID) {
        layx_item_t *pchild = layx_get_item(ctx, child);
        layx_vec4 rect = ctx->rects[child];
        layx_scalar child_size = rect[dim] + rect[2 + dim] + pchild->margins[wdim];
        need_size = layx_scalar_max(need_size, child_size);
        child = pchild->next_sibling;
    }
    return need_size;
}

// Helper to calculate stacked size
static LAYX_FORCE_INLINE
layx_scalar layx_calc_stacked_size(
        layx_context *ctx, layx_id item, int dim)
{
    const int wdim = dim + 2;
    layx_item_t *LAYX_RESTRICT pitem = layx_get_item(ctx, item);
    layx_scalar need_size = 0;
    layx_id child = pitem->first_child;
    while (child != LAYX_INVALID_ID) {
        layx_item_t *pchild = layx_get_item(ctx, child);
        layx_vec4 rect = ctx->rects[child];
        need_size += rect[dim] + rect[2 + dim] + pchild->margins[wdim];
        child = pchild->next_sibling;
    }
    return need_size;
}

// Helper to calculate wrapped overlayed size
static LAYX_FORCE_INLINE
layx_scalar layx_calc_wrapped_overlayed_size(
        layx_context *ctx, layx_id item, int dim)
{
    const int wdim = dim + 2;
    layx_item_t *LAYX_RESTRICT pitem = layx_get_item(ctx, item);
    layx_scalar need_size = 0;
    layx_scalar need_size2 = 0;
    layx_id child = pitem->first_child;
    while (child != LAYX_INVALID_ID) {
        layx_item_t *pchild = layx_get_item(ctx, child);
        layx_vec4 rect = ctx->rects[child];
        if (pchild->flags & LAYX_BREAK) {
            need_size2 += need_size;
            need_size = 0;
        }
        layx_scalar child_size = rect[dim] + rect[2 + dim] + pchild->margins[wdim];
        need_size = layx_scalar_max(need_size, child_size);
        child = pchild->next_sibling;
    }
    return need_size2 + need_size;
}

// Helper to calculate wrapped stacked size
static LAYX_FORCE_INLINE
layx_scalar layx_calc_wrapped_stacked_size(
        layx_context *ctx, layx_id item, int dim)
{
    const int wdim = dim + 2;
    layx_item_t *LAYX_RESTRICT pitem = layx_get_item(ctx, item);
    layx_scalar need_size = 0;
    layx_scalar need_size2 = 0;
    layx_id child = pitem->first_child;
    while (child != LAYX_INVALID_ID) {
        layx_item_t *pchild = layx_get_item(ctx, child);
        layx_vec4 rect = ctx->rects[child];
        if (pchild->flags & LAYX_BREAK) {
            need_size2 = layx_scalar_max(need_size2, need_size);
            need_size = 0;
        }
        need_size += rect[dim] + rect[2 + dim] + pchild->margins[wdim];
        child = pchild->next_sibling;
    }
    return layx_scalar_max(need_size2, need_size);
}

// PHASE 1: Calculate size (first pass)
static void layx_calc_size(layx_context *ctx, layx_id item, int dim)
{
    layx_item_t *pitem = layx_get_item(ctx, item);
    uint32_t flags = pitem->flags;

    layx_id child = pitem->first_child;
    while (child != LAYX_INVALID_ID) {
        layx_calc_size(ctx, child, dim);
        layx_item_t *pchild = layx_get_item(ctx, child);
        child = pchild->next_sibling;
    }

    ctx->rects[item][dim] = pitem->margins[dim];

    layx_scalar result_size;
    if (pitem->size[dim] != 0) {
        result_size = pitem->size[dim];
    } else {
        layx_scalar cal_size;
        layx_flex_direction direction = (layx_flex_direction)(flags & LAYX_FLEX_DIRECTION_MASK);
        uint32_t model = flags & LAYX_LAYOUT_MODEL_MASK;
        layx_flex_wrap wrap = (layx_flex_wrap)(flags & LAYX_FLEX_WRAP_MASK);
        
        if (model != 0) {
            bool is_wrapped = (wrap != LAYX_FLEX_WRAP_NOWRAP);
            bool is_row_direction = (direction == LAYX_FLEX_DIRECTION_ROW || direction == LAYX_FLEX_DIRECTION_ROW_REVERSE);
            
            if (is_wrapped) {
                if (is_row_direction) {
                    if (dim == 0)
                        cal_size = layx_calc_wrapped_stacked_size(ctx, item, 0);
                    else
                        cal_size = layx_calc_wrapped_overlayed_size(ctx, item, 1);
                } else {
                    if (dim == 1)
                        cal_size = layx_calc_wrapped_stacked_size(ctx, item, 1);
                    else
                        cal_size = layx_calc_wrapped_overlayed_size(ctx, item, 0);
                }
            } else {
                if ((is_row_direction && dim == 0) || (!is_row_direction && dim == 1)) {
                    cal_size = layx_calc_stacked_size(ctx, item, dim);
                } else {
                    cal_size = layx_calc_overlayed_size(ctx, item, dim);
                }
            }
        } else {
            cal_size = layx_calc_overlayed_size(ctx, item, dim);
        }

        cal_size += pitem->padding[dim] + pitem->border[dim] 
                 + pitem->padding[dim + 2] + pitem->border[dim + 2];
        result_size = cal_size;
    }

    // Apply min/max size constraints
    if (dim == 0) { // width
        if (pitem->min_size[0] > 0 && result_size < pitem->min_size[0]) {
            result_size = pitem->min_size[0];
        }
        if (pitem->max_size[0] > 0 && result_size > pitem->max_size[0]) {
            result_size = pitem->max_size[0];
        }
    } else { // height
        if (pitem->min_size[1] > 0 && result_size < pitem->min_size[1]) {
            result_size = pitem->min_size[1];
        }
        if (pitem->max_size[1] > 0 && result_size > pitem->max_size[1]) {
            result_size = pitem->max_size[1];
        }
    }

    ctx->rects[item][2 + dim] = result_size;
}

// Helper to arrange stacked items
static LAYX_FORCE_INLINE
void layx_arrange_stacked(
            layx_context *ctx, layx_id item, int dim, bool wrap)
{
    const int wdim = dim + 2;
    layx_item_t *pitem = layx_get_item(ctx, item);

    const uint32_t item_flags = pitem->flags;
    layx_scalar space = layx_get_internal_space(ctx, item, dim);
    layx_scalar content_offset = layx_get_content_offset(ctx, item, dim);

    float max_x2 = (float)(content_offset + space);

    layx_id start_child = pitem->first_child;
    while (start_child != LAYX_INVALID_ID) {
        layx_scalar used = 0;
        uint32_t count = 0;
        uint32_t squeezed_count = 0;
        uint32_t total = 0;
        bool hardbreak = false;
        
        layx_id child = start_child;
        layx_id end_child = LAYX_INVALID_ID;
        while (child != LAYX_INVALID_ID) {
            layx_item_t *pchild = layx_get_item(ctx, child);
            const uint32_t child_flags = pchild->flags;
            const uint32_t fflags = (child_flags & LAYX_SIZE_FIXED_MASK) >> dim;
            const layx_vec4 child_margins = pchild->margins;
            layx_vec4 child_rect = ctx->rects[child];
            
            // Check if item has flex-grow (should fill remaining space)
            int has_flex_grow = (dim == 0) ? (pchild->flex_grow > 0) : (pchild->flex_grow > 0);
            
            layx_scalar extend = used;
            if (has_flex_grow) {
                ++count;
                extend += child_rect[dim] + child_margins[wdim];
            } else {
                if ((fflags & LAYX_SIZE_FIXED_WIDTH) != LAYX_SIZE_FIXED_WIDTH)
                    ++squeezed_count;
                extend += child_rect[dim] + child_rect[2 + dim] + child_margins[wdim];
            }
            
            if (wrap && (total && ((extend > space) || (child_flags & LAYX_BREAK)))) {
                end_child = child;
                hardbreak = (child_flags & LAYX_BREAK) == LAYX_BREAK;
                pchild->flags = child_flags | LAYX_BREAK;
                break;
            } else {
                used = extend;
                child = pchild->next_sibling;
            }
            ++total;
        }

        layx_scalar extra_space = space - used;
        float filler = 0.0f;
        float spacer = 0.0f;
        float extra_margin = 0.0f;
        float eater = 0.0f;

        if (extra_space > 0) {
            if (count > 0)
                filler = (float)extra_space / (float)count;
            else if (total > 0) {
                layx_justify_content justify = (layx_justify_content)(item_flags & LAYX_JUSTIFY_CONTENT_MASK);
                switch (justify) {
                case LAYX_JUSTIFY_SPACE_BETWEEN:
                    if (!wrap || ((end_child != LAYX_INVALID_ID) && !hardbreak)) {
                        // For non-wrapped or single-line wrapped: distribute space between items
                        if (total > 1) {
                            spacer = (float)extra_space / (float)(total - 1);
                        } else {
                            // Single item: place at start (FLEX_START)
                            spacer = 0.0f;
                        }
                    }
                    break;
                case LAYX_JUSTIFY_FLEX_START:
                    break;
                case LAYX_JUSTIFY_FLEX_END:
                    extra_margin = extra_space;
                    break;
                case LAYX_JUSTIFY_CENTER:
                    extra_margin = extra_space / 2.0f;
                    break;
                case LAYX_JUSTIFY_SPACE_AROUND:
                    if (total > 0)
                        spacer = (float)extra_space / (float)total;
                    break;
                case LAYX_JUSTIFY_SPACE_EVENLY:
                    if (total > 0)
                        spacer = (float)extra_space / (float)(total + 1);
                    break;
                }
            }
        }
        
#ifdef LAYX_FLOAT
        else if (!wrap && (squeezed_count > 0))
#else
        else if (!wrap && (extra_space < 0))
#endif
            eater = (float)extra_space / (float)squeezed_count;

        float x = (float)content_offset;
        float x1;
        
        child = start_child;
        while (child != end_child) {
            // Apply spacer for first item too (for SPACE_AROUND/EVENLY with single item)
            if (spacer != 0 && child == start_child) {
                // For SPACE_AROUND: apply half spacer (space distributed to both sides)
                // For SPACE_EVENLY: apply full spacer (first gap of total+1 gaps)
                layx_justify_content justify = (layx_justify_content)(item_flags & LAYX_JUSTIFY_CONTENT_MASK);
                if (justify == LAYX_JUSTIFY_SPACE_AROUND) {
                    x += spacer / 2.0f;
                } else if (justify == LAYX_JUSTIFY_SPACE_EVENLY) {
                    x += spacer;
                }
                // For SPACE_BETWEEN: don't apply initial spacer, let extra_margin handle it
            }
            layx_scalar ix0, ix1;
            layx_item_t *pchild = layx_get_item(ctx, child);
            const uint32_t child_flags = pchild->flags;
            const uint32_t fflags = (child_flags & LAYX_SIZE_FIXED_MASK) >> dim;
            const layx_vec4 child_margins = pchild->margins;
            layx_vec4 child_rect = ctx->rects[child];

            // Check if item has flex-grow
            int has_flex_grow = (dim == 0) ? (pchild->flex_grow > 0) : (pchild->flex_grow > 0);
            
            x += (float)child_rect[dim] + extra_margin;
            if (has_flex_grow)
                x1 = x + filler;
            else if ((fflags & LAYX_SIZE_FIXED_WIDTH) == LAYX_SIZE_FIXED_WIDTH)
                x1 = x + (float)child_rect[2 + dim];
            else
                x1 = x + layx_float_max(0.0f, (float)child_rect[2 + dim] + eater);

            ix0 = (layx_scalar)x;
            if (wrap)
                ix1 = (layx_scalar)layx_float_min(max_x2 - (float)child_margins[wdim], x1);
            else
                ix1 = (layx_scalar)x1;
            child_rect[dim] = ix0;
            child_rect[dim + 2] = ix1 - ix0;
            ctx->rects[child] = child_rect;
            x = x1 + (float)child_margins[wdim];
            child = pchild->next_sibling;
            extra_margin = spacer;
        }

        start_child = end_child;
    }
}

// Helper to arrange overlay items (cross-axis alignment)
static LAYX_FORCE_INLINE
void layx_arrange_overlay(layx_context *ctx, layx_id item, int dim)
{
    const int wdim = dim + 2;
    layx_item_t *pitem = layx_get_item(ctx, item);
    const layx_scalar offset = layx_get_content_offset(ctx, item, dim);
    const layx_scalar space = layx_get_internal_space(ctx, item, dim);

    // Get align-items for cross-axis alignment
    layx_align_items align_items = (layx_align_items)(pitem->flags & LAYX_ALIGN_ITEMS_MASK);

    layx_id child = pitem->first_child;
    while (child != LAYX_INVALID_ID) {
        layx_item_t *pchild = layx_get_item(ctx, child);
        const layx_vec4 child_margins = pchild->margins;
        layx_vec4 child_rect = ctx->rects[child];

        // Check if child has explicit align-self
        layx_align_self align_self = (layx_align_self)(pchild->flags & LAYX_ALIGN_SELF_MASK);

        // Determine effective align (use child's align-self if set, otherwise use container's align-items)
        uint32_t align;
        if (align_self != LAYX_ALIGN_SELF_AUTO) {
            align = align_self;
        } else {
            align = (uint32_t)align_items;
        }

        // Apply alignment
        if (align == LAYX_ALIGN_ITEMS_CENTER || align == LAYX_ALIGN_SELF_CENTER) {
            // CENTER: center in available space
            child_rect[dim] += (space - child_rect[2 + dim]) / 2 - child_margins[wdim];
        } else if (align == LAYX_ALIGN_ITEMS_FLEX_END || align == LAYX_ALIGN_SELF_FLEX_END) {
            // FLEX_END: align to bottom/right
            child_rect[dim] += space - child_rect[2 + dim] - child_margins[dim] - child_margins[wdim];
        } else if (align == LAYX_ALIGN_ITEMS_STRETCH || align == LAYX_ALIGN_SELF_STRETCH) {
            // STRETCH: fill available space
            child_rect[2 + dim] = layx_scalar_max(0, space - child_rect[dim] - child_margins[wdim]);
        }
        // FLEX_START and BASELINE use default position (already at 0 relative to offset)

        child_rect[dim] += offset;
        ctx->rects[child] = child_rect;
        child = pchild->next_sibling;
    }
}

// Helper to arrange overlay squeezed range
static LAYX_FORCE_INLINE
void layx_arrange_overlay_squeezed_range(
        layx_context *ctx, int dim,
        layx_id start_item, layx_id end_item,
        layx_scalar offset, layx_scalar space)
{
    int wdim = dim + 2;
    layx_id item = start_item;
    while (item != end_item) {
        layx_item_t *pitem = layx_get_item(ctx, item);
        const layx_vec4 margins = pitem->margins;
        layx_vec4 rect = ctx->rects[item];
        layx_scalar min_size = layx_scalar_max(0, space - rect[dim] - margins[wdim]);
        rect[2 + dim] = layx_scalar_min(rect[2 + dim], min_size);
        rect[dim] += offset;
        ctx->rects[item] = rect;
        item = pitem->next_sibling;
    }
}

// Helper to arrange wrapped overlay squeezed
static LAYX_FORCE_INLINE
layx_scalar layx_arrange_wrapped_overlay_squeezed(
        layx_context *ctx, layx_id item, int dim)
{
    const int wdim = dim + 2;
    layx_item_t *pitem = layx_get_item(ctx, item);
    layx_scalar offset = layx_get_content_offset(ctx, item, dim);
    layx_scalar need_size = 0;
    layx_id child = pitem->first_child;
    layx_id start_child = child;
    while (child != LAYX_INVALID_ID) {
        layx_item_t *pchild = layx_get_item(ctx, child);
        if (pchild->flags & LAYX_BREAK) {
            layx_arrange_overlay_squeezed_range(ctx, dim, start_child, child, offset, need_size);
            offset += need_size;
            start_child = child;
            need_size = 0;
        }
        const layx_vec4 rect = ctx->rects[child];
        layx_scalar child_size = rect[dim] + rect[2 + dim] + pchild->margins[wdim];
        need_size = layx_scalar_max(need_size, child_size);
        child = pchild->next_sibling;
    }
    layx_arrange_overlay_squeezed_range(ctx, dim, start_child, LAYX_INVALID_ID, offset, need_size);
    offset += need_size;
    return offset;
}

// PHASE 2: Arrange items (second pass)
static void layx_arrange(layx_context *ctx, layx_id item, int dim)
{
    layx_item_t *pitem = layx_get_item(ctx, item);

    const uint32_t flags = pitem->flags;
    uint32_t model = flags & LAYX_LAYOUT_MODEL_MASK;
    layx_flex_direction direction = (layx_flex_direction)(flags & LAYX_FLEX_DIRECTION_MASK);
    layx_flex_wrap wrap = (layx_flex_wrap)(flags & LAYX_FLEX_WRAP_MASK);
    
    if (model != 0) {
        bool is_row_direction = (direction == LAYX_FLEX_DIRECTION_ROW || direction == LAYX_FLEX_DIRECTION_ROW_REVERSE);
        bool is_wrapped = (wrap != LAYX_FLEX_WRAP_NOWRAP);
        
        if (is_wrapped) {
            if (is_row_direction) {
                if (dim == 0) {
                    layx_arrange_stacked(ctx, item, 0, true);
                } else {
                    layx_arrange_wrapped_overlay_squeezed(ctx, item, 1);
                }
            } else {
                if (dim == 1) {
                    layx_arrange_stacked(ctx, item, 1, true);
                    layx_arrange_wrapped_overlay_squeezed(ctx, item, 0);
                } else {
                    layx_arrange_wrapped_overlay_squeezed(ctx, item, 0);
                }
            }
        } else {
            if ((is_row_direction && dim == 0) || (!is_row_direction && dim == 1)) {
                layx_arrange_stacked(ctx, item, dim, false);
            } else {
                // Use layx_arrange_overlay for cross-axis alignment (align-items)
                layx_arrange_overlay(ctx, item, dim);
            }
        }
    } else {
        layx_arrange_overlay(ctx, item, dim);
    }
    
    layx_id child = pitem->first_child;
    while (child != LAYX_INVALID_ID) {
        layx_arrange(ctx, child, dim);
        layx_item_t *pchild = layx_get_item(ctx, child);
        child = pchild->next_sibling;
    }
}

// Debug functions
const char* layx_get_layout_properties_string(layx_context *ctx, layx_id item)
{
    layx_item_t *pitem = layx_get_item(ctx, item);
    uint32_t flags = pitem->flags;
    
    static char buf[256];
    int len = 0;
    
    layx_display display = layx_get_display_from_flags(flags);
    switch (display) {
        case LAYX_DISPLAY_BLOCK: len += snprintf(buf + len, sizeof(buf) - len, "BLOCK"); break;
        case LAYX_DISPLAY_FLEX: len += snprintf(buf + len, sizeof(buf) - len, "FLEX"); break;
    }
    
    if (display == LAYX_DISPLAY_FLEX) {
        layx_flex_direction dir = (layx_flex_direction)(flags & LAYX_FLEX_DIRECTION_MASK);
        switch (dir) {
            case LAYX_FLEX_DIRECTION_ROW: len += snprintf(buf + len, sizeof(buf) - len, "|ROW"); break;
            case LAYX_FLEX_DIRECTION_COLUMN: len += snprintf(buf + len, sizeof(buf) - len, "|COLUMN"); break;
            case LAYX_FLEX_DIRECTION_ROW_REVERSE: len += snprintf(buf + len, sizeof(buf) - len, "|ROW_REVERSE"); break;
            case LAYX_FLEX_DIRECTION_COLUMN_REVERSE: len += snprintf(buf + len, sizeof(buf) - len, "|COLUMN_REVERSE"); break;
        }
        
        layx_flex_wrap wrap = (layx_flex_wrap)(flags & LAYX_FLEX_WRAP_MASK);
        switch (wrap) {
            case LAYX_FLEX_WRAP_WRAP: len += snprintf(buf + len, sizeof(buf) - len, "|WRAP"); break;
            case LAYX_FLEX_WRAP_WRAP_REVERSE: len += snprintf(buf + len, sizeof(buf) - len, "|WRAP_REVERSE"); break;
            case LAYX_FLEX_WRAP_NOWRAP: break;
        }
        
        layx_justify_content justify = (layx_justify_content)(flags & LAYX_JUSTIFY_CONTENT_MASK);
        switch (justify) {
            case LAYX_JUSTIFY_CENTER: len += snprintf(buf + len, sizeof(buf) - len, "|JUSTIFY_CENTER"); break;
            case LAYX_JUSTIFY_FLEX_END: len += snprintf(buf + len, sizeof(buf) - len, "|JUSTIFY_FLEX_END"); break;
            case LAYX_JUSTIFY_SPACE_BETWEEN: len += snprintf(buf + len, sizeof(buf) - len, "|JUSTIFY_SPACE_BETWEEN"); break;
            case LAYX_JUSTIFY_SPACE_AROUND: len += snprintf(buf + len, sizeof(buf) - len, "|JUSTIFY_SPACE_AROUND"); break;
            case LAYX_JUSTIFY_SPACE_EVENLY: len += snprintf(buf + len, sizeof(buf) - len, "|JUSTIFY_SPACE_EVENLY"); break;
            case LAYX_JUSTIFY_FLEX_START: break;
        }
        
        layx_align_items align_items = (layx_align_items)(flags & LAYX_ALIGN_ITEMS_MASK);
        switch (align_items) {
            case LAYX_ALIGN_ITEMS_CENTER: len += snprintf(buf + len, sizeof(buf) - len, "|ALIGN_ITEMS_CENTER"); break;
            case LAYX_ALIGN_ITEMS_FLEX_END: len += snprintf(buf + len, sizeof(buf) - len, "|ALIGN_ITEMS_FLEX_END"); break;
            case LAYX_ALIGN_ITEMS_BASELINE: len += snprintf(buf + len, sizeof(buf) - len, "|ALIGN_ITEMS_BASELINE"); break;
            case LAYX_ALIGN_ITEMS_FLEX_START: break;
            case LAYX_ALIGN_ITEMS_STRETCH: break;
        }
        
        layx_align_content align_content = (layx_align_content)(flags & LAYX_ALIGN_CONTENT_MASK);
        switch (align_content) {
            case LAYX_ALIGN_CONTENT_CENTER: len += snprintf(buf + len, sizeof(buf) - len, "|ALIGN_CONTENT_CENTER"); break;
            case LAYX_ALIGN_CONTENT_FLEX_END: len += snprintf(buf + len, sizeof(buf) - len, "|ALIGN_CONTENT_FLEX_END"); break;
            case LAYX_ALIGN_CONTENT_SPACE_BETWEEN: len += snprintf(buf + len, sizeof(buf) - len, "|ALIGN_CONTENT_SPACE_BETWEEN"); break;
            case LAYX_ALIGN_CONTENT_SPACE_AROUND: len += snprintf(buf + len, sizeof(buf) - len, "|ALIGN_CONTENT_SPACE_AROUND"); break;
            case LAYX_ALIGN_CONTENT_FLEX_START: break;
            case LAYX_ALIGN_CONTENT_STRETCH: break;
        }
    }
    
    if (len == 0) return "BLOCK";
    return buf;
}

const char* layx_get_item_alignment_string(layx_context *ctx, layx_id item)
{
    layx_item_t *pitem = layx_get_item(ctx, item);
    uint32_t flags = pitem->flags;

    static char buf[128];
    int len = 0;
    int first = 1;

    if (flags & LAYX_SIZE_FIXED_WIDTH) {
        len += snprintf(buf + len, sizeof(buf) - len, "%sWIDTH_FIXED", first ? "" : "|");
        first = 0;
    }
    if (flags & LAYX_SIZE_FIXED_HEIGHT) {
        len += snprintf(buf + len, sizeof(buf) - len, "%sHEIGHT_FIXED", first ? "" : "|");
        first = 0;
    }
    if (flags & LAYX_FILL_HORIZONTAL) {
        len += snprintf(buf + len, sizeof(buf) - len, "%sFILL_H", first ? "" : "|");
        first = 0;
    }
    if (flags & LAYX_FILL_VERTICAL) {
        len += snprintf(buf + len, sizeof(buf) - len, "%sFILL_V", first ? "" : "|");
        first = 0;
    }

    if (len == 0) return "default";
    return buf;
}