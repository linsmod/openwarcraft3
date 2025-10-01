/*
 * LibCSS - example1.c
 *
 * Compile this using a command such as:
 *  gcc -g -W -Wall -o example1 example1.c `pkg-config --cflags --libs libcss`
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>
/* HTML/CSS integration headers */
#include "common/shared.h"
#include <libcss/properties.h>
#include <libcss/types.h>

/* The entire API is available through this header. */
#include <libcss/libcss.h>
#include <libxml/tree.h>
#include <libxml/HTMLtree.h>
/* Forward declaration */
typedef struct context context;


/* This macro is used to silence compiler warnings about unused function
 * arguments. */
#define UNUSED(x) ((x) = (x))

/* CSS Parser Context Structure */
typedef struct {
    css_stylesheet *stylesheet;
    css_select_ctx *select_ctx;
    css_unit_ctx unit_ctx;
} css_parser_t;

/* Function declarations. */
static css_error resolve_url(void *pw,
		const char *base, lwc_string *rel, lwc_string **abs);
static void die(const char *text, css_error code);

static css_error node_name(void *pw, void *node,
		css_qname *qname);
static css_error node_classes(void *pw, void *node,
		lwc_string ***classes, uint32_t *n_classes);
static css_error node_id(void *pw, void *node,
		lwc_string **id);
static css_error named_ancestor_node(void *pw, void *node,
		const css_qname *qname,
		void **ancestor);
static css_error named_parent_node(void *pw, void *node,
		const css_qname *qname,
		void **parent);
static css_error named_sibling_node(void *pw, void *node,
		const css_qname *qname,
		void **sibling);
static css_error named_generic_sibling_node(void *pw, void *node,
		const css_qname *qname,
		void **sibling);
static css_error parent_node(void *pw, void *node, void **parent);
static css_error sibling_node(void *pw, void *node, void **sibling);
static css_error node_has_name(void *pw, void *node,
		const css_qname *qname,
		bool *match);
static css_error node_has_class(void *pw, void *node,
		lwc_string *name,
		bool *match);
static css_error node_has_id(void *pw, void *node,
		lwc_string *name,
		bool *match);
static css_error node_has_attribute(void *pw, void *node,
		const css_qname *qname,
		bool *match);
static css_error node_has_attribute_equal(void *pw, void *node,
		const css_qname *qname,
		lwc_string *value,
		bool *match);
static css_error node_has_attribute_dashmatch(void *pw, void *node,
		const css_qname *qname,
		lwc_string *value,
		bool *match);
static css_error node_has_attribute_includes(void *pw, void *node,
		const css_qname *qname,
		lwc_string *value,
		bool *match);
static css_error node_has_attribute_prefix(void *pw, void *node,
		const css_qname *qname,
		lwc_string *value,
		bool *match);
static css_error node_has_attribute_suffix(void *pw, void *node,
		const css_qname *qname,
		lwc_string *value,
		bool *match);
static css_error node_has_attribute_substring(void *pw, void *node,
		const css_qname *qname,
		lwc_string *value,
		bool *match);
static css_error node_is_root(void *pw, void *node, bool *match);
static css_error node_count_siblings(void *pw, void *node,
		bool same_name, bool after, int32_t *count);
static css_error node_is_empty(void *pw, void *node, bool *match);
static css_error node_is_link(void *pw, void *node, bool *match);
static css_error node_is_visited(void *pw, void *node, bool *match);
static css_error node_is_hover(void *pw, void *node, bool *match);
static css_error node_is_active(void *pw, void *node, bool *match);
static css_error node_is_focus(void *pw, void *node, bool *match);
static css_error node_is_enabled(void *pw, void *node, bool *match);
static css_error node_is_disabled(void *pw, void *node, bool *match);
static css_error node_is_checked(void *pw, void *node, bool *match);
static css_error node_is_target(void *pw, void *node, bool *match);
static css_error node_is_lang(void *pw, void *node,
		lwc_string *lang, bool *match);
static css_error node_presentational_hint(void *pw, void *node,
		uint32_t *nhints, css_hint **hints);
static css_error ua_default_for_property(void *pw, uint32_t property,
		css_hint *hint);
static css_error set_libcss_node_data(void *pw, void *n,
		void *libcss_node_data);
static css_error get_libcss_node_data(void *pw, void *n,
		void **libcss_node_data);

static css_unit_ctx unit_len_ctx = {
	.viewport_width    = 800 * (1 << CSS_RADIX_POINT),
	.viewport_height   = 600 * (1 << CSS_RADIX_POINT),
	.font_size_default =  16 * (1 << CSS_RADIX_POINT),
	.font_size_minimum =   6 * (1 << CSS_RADIX_POINT),
	.device_dpi        =  96 * (1 << CSS_RADIX_POINT),
	.root_style        = NULL, /* We don't have a root node yet. */
	.pw                = NULL, /* We're not implementing measure callback */
	.measure           = NULL, /* We're not implementing measure callback */
};
/* Table of function pointers for the LibCSS Select API. */
static css_select_handler select_handler = {
	CSS_SELECT_HANDLER_VERSION_1,

	node_name,
	node_classes,
	node_id,
	named_ancestor_node,
	named_parent_node,
	named_sibling_node,
	named_generic_sibling_node,
	parent_node,
	sibling_node,
	node_has_name,
	node_has_class,
	node_has_id,
	node_has_attribute,
	node_has_attribute_equal,
	node_has_attribute_dashmatch,
	node_has_attribute_includes,
	node_has_attribute_prefix,
	node_has_attribute_suffix,
	node_has_attribute_substring,
	node_is_root,
	node_count_siblings,
	node_is_empty,
	node_is_link,
	node_is_visited,
	node_is_hover,
	node_is_active,
	node_is_focus,
	node_is_enabled,
	node_is_disabled,
	node_is_checked,
	node_is_target,
	node_is_lang,
	node_presentational_hint,
	ua_default_for_property,
	set_libcss_node_data,
	get_libcss_node_data,
};

/* LWC leak callback */
void lwc_callback(lwc_string *str, void *pw)
{
	(void)(pw);

	fprintf(stderr, "Leaked string: %.*s\n",
			(int)lwc_string_length(str),
			lwc_string_data(str));
}

// int main(int argc, char **argv)
// {
// 	css_error code;
// 	css_stylesheet *sheet;
// 	size_t size;
// 	const char data[] = "h1 { color: red } "
// 		"h4 { color: #321; } "
// 		"h4, h5 { color: #123456; }";
// 	css_select_ctx *select_ctx;
// 	uint32_t count;
// 	unsigned int hh;
// 	css_stylesheet_params params;
// 	css_media media = {
// 		.type = CSS_MEDIA_SCREEN,
// 	};

// 	UNUSED(argc);
// 	UNUSED(argv);

// 	params.params_version = CSS_STYLESHEET_PARAMS_VERSION_1;
// 	params.level = CSS_LEVEL_21;
// 	params.charset = "UTF-8";
// 	params.url = "foo";
// 	params.title = "foo";
// 	params.allow_quirks = false;
// 	params.inline_style = false;
// 	params.resolve = resolve_url;
// 	params.resolve_pw = NULL;
// 	params.import = NULL;
// 	params.import_pw = NULL;
// 	params.color = NULL;
// 	params.color_pw = NULL;
// 	params.font = NULL;
// 	params.font_pw = NULL;

// 	/* create a stylesheet */
// 	code = css_stylesheet_create(&params, &sheet);
// 	if (code != CSS_OK)
// 		die("css_stylesheet_create", code);
// 	code = css_stylesheet_size(sheet, &size);
// 	if (code != CSS_OK)
// 		die("css_stylesheet_size", code);
// 	printf("created stylesheet, size %zu\n", size);


// 	/* parse some CSS source */
// 	code = css_stylesheet_append_data(sheet, (const uint8_t *) data,
// 			sizeof data);
// 	if (code != CSS_OK && code != CSS_NEEDDATA)
// 		die("css_stylesheet_append_data", code);
// 	code = css_stylesheet_data_done(sheet);
// 	if (code != CSS_OK)
// 		die("css_stylesheet_data_done", code);
// 	code = css_stylesheet_size(sheet, &size);
// 	if (code != CSS_OK)
// 		die("css_stylesheet_size", code);
// 	printf("appended data, size now %zu\n", size);


// 	/* prepare a selection context containing the stylesheet */
// 	code = css_select_ctx_create(&select_ctx);
// 	if (code != CSS_OK)
// 		die("css_select_ctx_create", code);
// 	code = css_select_ctx_append_sheet(select_ctx, sheet, CSS_ORIGIN_AUTHOR,
// 			NULL);
// 	if (code != CSS_OK)
// 		die("css_select_ctx_append_sheet", code);
// 	code = css_select_ctx_count_sheets(select_ctx, &count);
// 	if (code != CSS_OK)
// 		die("css_select_ctx_count_sheets", code);
// 	printf("created selection context with %i sheets\n", count);


// 	/* select style for each of h1 to h6 */
// 	for (hh = 1; hh != 7; hh++) {
// 		css_select_results *style;
// 		char element[20];
// 		lwc_string *element_name;
// 		uint8_t color_type;
// 		css_color color_shade;

// 		/* in this very simple example our "document tree" is just one
// 		 * node and is in fact a libwapcaplet string containing the
// 		 * element name */
// 		snprintf(element, sizeof element, "h%i", hh);
// 		lwc_intern_string(element, strlen(element), &element_name);

// 		code = css_select_style(select_ctx, element_name,
// 				&unit_len_ctx,
// 				&media, NULL,
// 				&select_handler, 0,
// 				&style);
// 		if (code != CSS_OK)
// 			die("css_select_style", code);

// 		lwc_string_unref(element_name);

// 		color_type = css_computed_color(
// 				style->styles[CSS_PSEUDO_ELEMENT_NONE],
// 				&color_shade);
// 		if (color_type == CSS_COLOR_INHERIT)
// 			printf("color of h%i is 'inherit'\n", hh);
// 		else
// 			printf("color of h%i is %x\n", hh, color_shade);

// 		code = css_select_results_destroy(style);
// 		if (code != CSS_OK)
// 			die("css_computed_style_destroy", code);
// 	}


// 	/* free everything and shut down libcss */
// 	code = css_select_ctx_destroy(select_ctx);
// 	if (code != CSS_OK)
// 		die("css_select_ctx_destroy", code);
// 	code = css_stylesheet_destroy(sheet);
// 	if (code != CSS_OK)
// 		die("css_stylesheet_destroy", code);

// 	lwc_iterate_strings(lwc_callback, NULL);
// 	return 0;
// }


css_error resolve_url(void *pw,
		const char *base, lwc_string *rel, lwc_string **abs)
{
	UNUSED(pw);
	UNUSED(base);

	/* About as useless as possible */
	*abs = lwc_string_ref(rel);

	return CSS_OK;
}


void die(const char *text, css_error code)
{
	fprintf(stderr, "ERROR: %s: %i: %s\n",
			text, code, css_error_to_string(code));
	exit(EXIT_FAILURE);
}



/* Select handlers. Our "document tree" is actually just a single node, which is
 * a libwapcaplet string containing the element name. Therefore all the
 * functions below except those getting or testing the element name return empty
 * data or false attributes. */
css_error node_name(void *pw, void *n, css_qname *qname)
{
	lwc_string *node = n;

	UNUSED(pw);

	qname->name = lwc_string_ref(node);

	return CSS_OK;
}

css_error node_classes(void *pw, void *n,
		lwc_string ***classes, uint32_t *n_classes)
{
	context *c = (context *)pw;
	xmlNode *node = (xmlNode *)n;
	
	UNUSED(pw);
	
	if (!node || !node->name) {
		*classes = NULL;
		*n_classes = 0;
		return CSS_OK;
	}
	
	/* Get class attribute from HTML element */
	xmlChar *class_attr = xmlGetProp(node, BAD_CAST "class");
	if (!class_attr) {
		*classes = NULL;
		*n_classes = 0;
		return CSS_OK;
	}
	
	/* Parse class string into individual class names */
	char *class_str = (char *)class_attr;
	char *token;
	char *saveptr;
	uint32_t count = 0;
	
	/* Count the number of classes */
	char *temp_str = strdup(class_str);
	char *temp_ptr = temp_str;
	while ((token = strtok_r(temp_ptr, " \t\n\r", &saveptr))) {
		count++;
		temp_ptr = NULL;
	}
	free(temp_str);
	
	if (count == 0) {
		xmlFree(class_attr);
		*classes = NULL;
		*n_classes = 0;
		return CSS_OK;
	}
	
	/* Allocate memory for class strings */
	*classes = malloc(count * sizeof(lwc_string *));
	if (!*classes) {
		xmlFree(class_attr);
		return CSS_NOMEM;
	}
	
	/* Parse individual classes */
	temp_str = strdup(class_str);
	temp_ptr = temp_str;
	uint32_t i = 0;
	while ((token = strtok_r(temp_ptr, " \t\n\r", &saveptr)) && i < count) {
		lwc_intern_string(token, strlen(token), &(*classes)[i]);
		if (!(*classes)[i]) {
			/* Clean up already allocated strings */
			for (uint32_t j = 0; j < i; j++) {
				lwc_string_unref((*classes)[j]);
			}
			free(*classes);
			xmlFree(class_attr);
			return CSS_NOMEM;
		}
		i++;
		temp_ptr = NULL;
	}
	
	*n_classes = count;
	xmlFree(class_attr);
	return CSS_OK;
}

css_error node_id(void *pw, void *n, lwc_string **id)
{
	UNUSED(pw);
	UNUSED(n);
	*id = NULL;
	return CSS_OK;
}

css_error named_ancestor_node(void *pw, void *n,
		const css_qname *qname,
		void **ancestor)
{
	UNUSED(pw);
	UNUSED(n);
	UNUSED(qname);
	*ancestor = NULL;
	return CSS_OK;
}

css_error named_parent_node(void *pw, void *n,
		const css_qname *qname,
		void **parent)
{
	UNUSED(pw);
	UNUSED(n);
	UNUSED(qname);
	*parent = NULL;
	return CSS_OK;
}

css_error named_generic_sibling_node(void *pw, void *n,
		const css_qname *qname,
		void **sibling)
{
	UNUSED(pw);
	UNUSED(n);
	UNUSED(qname);
	*sibling = NULL;
	return CSS_OK;
}

css_error named_sibling_node(void *pw, void *n,
		const css_qname *qname,
		void **sibling)
{
	UNUSED(pw);
	UNUSED(n);
	UNUSED(qname);
	*sibling = NULL;
	return CSS_OK;
}

css_error parent_node(void *pw, void *n, void **parent)
{
	UNUSED(pw);
	UNUSED(n);
	*parent = NULL;
	return CSS_OK;
}

css_error sibling_node(void *pw, void *n, void **sibling)
{
	UNUSED(pw);
	UNUSED(n);
	*sibling = NULL;
	return CSS_OK;
}

css_error node_has_name(void *pw, void *n,
		const css_qname *qname,
		bool *match)
{
	lwc_string *node = n;
	UNUSED(pw);
	assert(lwc_string_caseless_isequal(node, qname->name, match) ==
			lwc_error_ok);
	return CSS_OK;
}

css_error node_has_class(void *pw, void *n,
		lwc_string *name,
		bool *match)
{
	UNUSED(pw);
	UNUSED(n);
	UNUSED(name);
	*match = false;
	return CSS_OK;
}

css_error node_has_id(void *pw, void *n,
		lwc_string *name,
		bool *match)
{
	UNUSED(pw);
	UNUSED(n);
	UNUSED(name);
	*match = false;
	return CSS_OK;
}

css_error node_has_attribute(void *pw, void *n,
		const css_qname *qname,
		bool *match)
{
	UNUSED(pw);
	UNUSED(n);
	UNUSED(qname);
	*match = false;
	return CSS_OK;
}

css_error node_has_attribute_equal(void *pw, void *n,
		const css_qname *qname,
		lwc_string *value,
		bool *match)
{
	UNUSED(pw);
	UNUSED(n);
	UNUSED(qname);
	UNUSED(value);
	*match = false;
	return CSS_OK;
}

css_error node_has_attribute_dashmatch(void *pw, void *n,
		const css_qname *qname,
		lwc_string *value,
		bool *match)
{
	UNUSED(pw);
	UNUSED(n);
	UNUSED(qname);
	UNUSED(value);
	*match = false;
	return CSS_OK;
}

css_error node_has_attribute_includes(void *pw, void *n,
		const css_qname *qname,
		lwc_string *value,
		bool *match)
{
	UNUSED(pw);
	UNUSED(n);
	UNUSED(qname);
	UNUSED(value);
	*match = false;
	return CSS_OK;
}

css_error node_has_attribute_prefix(void *pw, void *n,
		const css_qname *qname,
		lwc_string *value,
		bool *match)
{
	UNUSED(pw);
	UNUSED(n);
	UNUSED(qname);
	UNUSED(value);
	*match = false;
	return CSS_OK;
}

css_error node_has_attribute_suffix(void *pw, void *n,
		const css_qname *qname,
		lwc_string *value,
		bool *match)
{
	UNUSED(pw);
	UNUSED(n);
	UNUSED(qname);
	UNUSED(value);
	*match = false;
	return CSS_OK;
}

css_error node_has_attribute_substring(void *pw, void *n,
		const css_qname *qname,
		lwc_string *value,
		bool *match)
{
	UNUSED(pw);
	UNUSED(n);
	UNUSED(qname);
	UNUSED(value);
	*match = false;
	return CSS_OK;
}

css_error node_is_first_child(void *pw, void *n, bool *match)
{
	UNUSED(pw);
	UNUSED(n);
	*match = false;
	return CSS_OK;
}

css_error node_is_root(void *pw, void *n, bool *match)
{
	UNUSED(pw);
	UNUSED(n);
	*match = false;
	return CSS_OK;
}

css_error node_count_siblings(void *pw, void *n,
		bool same_name, bool after, int32_t *count)
{
	UNUSED(pw);
	UNUSED(n);
	UNUSED(same_name);
	UNUSED(after);
	*count = 1;
	return CSS_OK;
}

css_error node_is_empty(void *pw, void *n, bool *match)
{
	UNUSED(pw);
	UNUSED(n);
	*match = false;
	return CSS_OK;
}

css_error node_is_link(void *pw, void *n, bool *match)
{
	UNUSED(pw);
	UNUSED(n);
	*match = false;
	return CSS_OK;
}

css_error node_is_visited(void *pw, void *n, bool *match)
{
	UNUSED(pw);
	UNUSED(n);
	*match = false;
	return CSS_OK;
}

css_error node_is_hover(void *pw, void *n, bool *match)
{
	UNUSED(pw);
	UNUSED(n);
	*match = false;
	return CSS_OK;
}

css_error node_is_active(void *pw, void *n, bool *match)
{
	UNUSED(pw);
	UNUSED(n);
	*match = false;
	return CSS_OK;
}

css_error node_is_focus(void *pw, void *n, bool *match)
{
	UNUSED(pw);
	UNUSED(n);
	*match = false;
	return CSS_OK;
}

css_error node_is_enabled(void *pw, void *n, bool *match)
{
	UNUSED(pw);
	UNUSED(n);
	*match = false;
	return CSS_OK;
}

css_error node_is_disabled(void *pw, void *n, bool *match)
{
	UNUSED(pw);
	UNUSED(n);
	*match = false;
	return CSS_OK;
}

css_error node_is_checked(void *pw, void *n, bool *match)
{
	UNUSED(pw);
	UNUSED(n);
	*match = false;
	return CSS_OK;
}

css_error node_is_target(void *pw, void *n, bool *match)
{
	UNUSED(pw);
	UNUSED(n);
	*match = false;
	return CSS_OK;
}


css_error node_is_lang(void *pw, void *n,
		lwc_string *lang,
		bool *match)
{
	UNUSED(pw);
	UNUSED(n);
	UNUSED(lang);
	*match = false;
	return CSS_OK;
}

css_error node_presentational_hint(void *pw, void *node,
		uint32_t *nhints, css_hint **hints)
{
	UNUSED(pw);
	UNUSED(node);
	*nhints = 0;
	*hints = NULL;
	return CSS_OK;
}

css_error ua_default_for_property(void *pw, uint32_t property, css_hint *hint)
{
	UNUSED(pw);

	if (property == CSS_PROP_COLOR) {
		hint->data.color = 0x00000000;
		hint->status = CSS_COLOR_COLOR;
	} else if (property == CSS_PROP_FONT_FAMILY) {
		hint->data.strings = NULL;
		hint->status = CSS_FONT_FAMILY_SANS_SERIF;
	} else if (property == CSS_PROP_QUOTES) {
		/* Not exactly useful :) */
		hint->data.strings = NULL;
		hint->status = CSS_QUOTES_NONE;
	} else if (property == CSS_PROP_VOICE_FAMILY) {
		/** \todo Fix this when we have voice-family done */
		hint->data.strings = NULL;
		hint->status = 0;
	} else {
		return CSS_INVALID;
	}

	return CSS_OK;
}

static css_error set_libcss_node_data(void *pw, void *n,
		void *libcss_node_data)
{
	UNUSED(pw);
	UNUSED(n);

	/* Since we're not storing it, ensure node data gets deleted */
	css_libcss_node_data_handler(&select_handler, CSS_NODE_DELETED,
			pw, n, NULL, libcss_node_data);

	return CSS_OK;
}

static css_error get_libcss_node_data(void *pw, void *n,
		void **libcss_node_data)
{
	UNUSED(pw);
	UNUSED(n);
	*libcss_node_data = NULL;

	return CSS_OK;
}


/* CSS Parser API */

/**
 * @brief Create a new CSS parser instance
 * @return css_parser_t* or NULL on failure
 */
css_parser_t* css_parser_create(void)
{
    css_parser_t *parser = malloc(sizeof(css_parser_t));
    if (!parser) return NULL;
    
    memset(parser, 0, sizeof(css_parser_t));
    
    /* Initialize unit context */
    parser->unit_ctx.viewport_width = 800 * (1 << CSS_RADIX_POINT);
    parser->unit_ctx.viewport_height = 600 * (1 << CSS_RADIX_POINT);
    parser->unit_ctx.font_size_default = 16 * (1 << CSS_RADIX_POINT);
    parser->unit_ctx.font_size_minimum = 6 * (1 << CSS_RADIX_POINT);
    parser->unit_ctx.device_dpi = 96 * (1 << CSS_RADIX_POINT);
    parser->unit_ctx.root_style = NULL;
    parser->unit_ctx.pw = NULL;
    
    /* Create select context */
    css_error code = css_select_ctx_create(&parser->select_ctx);
    if (code != CSS_OK) {
        free(parser);
        return NULL;
    }
    
    return parser;
}

/**
 * @brief Parse CSS data and add to parser
 * @param parser CSS parser instance
 * @param css_data CSS source code
 * @param url Base URL for relative URLs (can be NULL)
 * @return 0 on success, error code on failure
 */
int css_parser_parse(css_parser_t *parser, const char *css_data, const char *url)
{
    if (!parser || !css_data) return -1;
    
    css_stylesheet_params params = {
        .params_version = CSS_STYLESHEET_PARAMS_VERSION_1,
        .level = CSS_LEVEL_21,
        .charset = "UTF-8",
        .url = url ? url : "inline",
        .title = "stylesheet",
        .allow_quirks = false,
        .inline_style = false,
        .resolve = resolve_url,
        .resolve_pw = NULL,
        .import = NULL,
        .import_pw = NULL,
        .color = NULL,
        .color_pw = NULL,
        .font = NULL,
        .font_pw = NULL
    };
    
    css_error code = css_stylesheet_create(&params, &parser->stylesheet);
    if (code != CSS_OK) return code;
    
    /* Append CSS data */
    size_t data_len = strlen(css_data);
    code = css_stylesheet_append_data(parser->stylesheet, 
                                    (const uint8_t *)css_data, data_len);
    if (code != CSS_OK && code != CSS_NEEDDATA) {
        css_stylesheet_destroy(parser->stylesheet);
        parser->stylesheet = NULL;
        return code;
    }
    
    code = css_stylesheet_data_done(parser->stylesheet);
    if (code != CSS_OK) {
        css_stylesheet_destroy(parser->stylesheet);
        parser->stylesheet = NULL;
        return code;
    }
    
    /* Add stylesheet to select context */
    code = css_select_ctx_append_sheet(parser->select_ctx, parser->stylesheet, 
                                     CSS_ORIGIN_AUTHOR, NULL);
    if (code != CSS_OK) {
        css_stylesheet_destroy(parser->stylesheet);
        parser->stylesheet = NULL;
        return code;
    }
    
    return 0;
}

/**
 * @brief Get computed style for an element
 * @param parser CSS parser instance
 * @param element_name HTML element name (e.g., "div", "p", "h1")
 * @param media Media type (CSS_MEDIA_SCREEN, CSS_MEDIA_PRINT, etc.)
 * @return css_select_results* or NULL on failure
 */
css_select_results* css_parser_get_style(css_parser_t *parser, 
                                       const char *element_name,
                                       css_media_type media_type)
{
    if (!parser || !element_name) return NULL;
    
    lwc_string *elem_str;
    lwc_intern_string(element_name, strlen(element_name), &elem_str);
    if (!elem_str) return NULL;
    
    css_media media = { .type = media_type };
    css_select_results *results;
    
    css_error code = css_select_style(parser->select_ctx, elem_str,
                                    &parser->unit_ctx, &media, NULL,
                                    &select_handler, 0, &results);
    
    lwc_string_unref(elem_str);
    
    if (code != CSS_OK) return NULL;
    return results;
}

/**
 * @brief Get color property from computed style
 * @param results Computed style results
 * @param color Output color value
 * @return CSS color type
 */
uint8_t css_parser_get_color(css_select_results *results, css_color *color)
{
    if (!results || !color) return CSS_COLOR_INHERIT;
    
    return css_computed_color(results->styles[CSS_PSEUDO_ELEMENT_NONE], color);
}

/**
 * @brief Destroy CSS parser instance
 * @param parser CSS parser to destroy
 */
void css_parser_destroy(css_parser_t *parser)
{
    if (!parser) return;
    
    if (parser->select_ctx) {
        css_select_ctx_destroy(parser->select_ctx);
    }
    
    if (parser->stylesheet) {
        css_stylesheet_destroy(parser->stylesheet);
    }
    
    free(parser);
}

/* CSS parser instance (global for simplicity, better to pass as context) */
static css_parser_t *global_css_parser = NULL;
/**
 * @brief Parse inline CSS style string
 */
css_select_results* css_parse_inline_style(const char *style_str, const char *element_name)
{
    if (!global_css_parser || !style_str || !element_name) return NULL;
    
    /* Create a temporary CSS rule */
    char css_buffer[1024];
    snprintf(css_buffer, sizeof(css_buffer), "%s { %s }", element_name, style_str);
    
    /* Parse the inline style */
    if (css_parser_parse(global_css_parser, css_buffer, "inline") != 0) {
        return NULL;
    }
    
    /* Get the computed style */
    return css_parser_get_style(global_css_parser, element_name, CSS_MEDIA_SCREEN);
}

/**
 * @brief Get CSS property value as string
 */
const char* css_get_property_string(css_select_results *results, uint32_t property)
{
    if (!results) return NULL;
    
    css_computed_style *style = results->styles[CSS_PSEUDO_ELEMENT_NONE];
    if (!style) return NULL;
    
    /* This is a simplified version - you'd need to implement proper property extraction */
    switch (property) {
        case CSS_PROP_FONT_FAMILY: {
            uint8_t font_family = css_computed_font_family(style, NULL);
            /* Convert to string representation */
            return "Arial"; /* Placeholder */
        }
        case CSS_PROP_FONT_SIZE: {
            css_fixed size;
            css_unit unit;
            uint8_t font_size = css_computed_font_size(style, &size, &unit);
            /* Convert to string representation */
            return "16px"; /* Placeholder */
        }
        case CSS_PROP_COLOR: {
            css_color color;
            uint8_t color_type = css_computed_color(style, &color);
            if (color_type == CSS_COLOR_COLOR) {
                static char color_str[8];
                snprintf(color_str, sizeof(color_str), "#%06x", color);
                return color_str;
            }
            return NULL;
        }
		case CSS_PROP_BORDER_TOP_WIDTH:
		case CSS_PROP_BORDER_RIGHT_WIDTH:
		case CSS_PROP_BORDER_BOTTOM_WIDTH:
		case CSS_PROP_BORDER_LEFT_WIDTH:{
            /* Get border width */
            css_fixed width;
            css_unit unit;
            uint8_t border_width = css_computed_border_top_width(style, &width, &unit);
            if (border_width == CSS_BORDER_WIDTH_WIDTH) {
                static char width_str[16];
                int px_width = FIXTOINT(width);
                snprintf(width_str, sizeof(width_str), "%dpx", px_width);
                return width_str;
            }
            return "1px"; /* Default */
        }
		case CSS_PROP_BORDER_TOP_STYLE:
		case CSS_PROP_BORDER_RIGHT_STYLE:
		case CSS_PROP_BORDER_BOTTOM_STYLE:
		case CSS_PROP_BORDER_LEFT_STYLE: {
            /* Get border style */
            uint8_t border_style = css_computed_border_top_style(style);
            switch (border_style) {
                case CSS_BORDER_STYLE_SOLID: return "solid";
                case CSS_BORDER_STYLE_DASHED: return "dashed";
                case CSS_BORDER_STYLE_DOTTED: return "dotted";
                case CSS_BORDER_STYLE_DOUBLE: return "double";
                default: return "solid";
            }
        }
		case CSS_PROP_BORDER_TOP_COLOR:
		case CSS_PROP_BORDER_RIGHT_COLOR:
		case CSS_PROP_BORDER_BOTTOM_COLOR:
		case CSS_PROP_BORDER_LEFT_COLOR:{
            /* Get border color */
            css_color color;
            uint8_t color_type = css_computed_border_top_color(style, &color);
            if (color_type == CSS_COLOR_COLOR) {
                static char color_str[8];
                snprintf(color_str, sizeof(color_str), "#%06x", color);
                return color_str;
            }
            return "#000000"; /* Default black */
        }
        // case CSS_PROP_BORDER: {
        //     /* Get shorthand border property */
        //     static char border_str[64];
        //     const char *width = css_get_property_string(results, CSS_PROP_BORDER_WIDTH);
        //     const char *style = css_get_property_string(results, CSS_PROP_BORDER_STYLE);
        //     const char *color = css_get_property_string(results, CSS_PROP_BORDER_COLOR);
            
        //     if (width && style && color) {
        //         snprintf(border_str, sizeof(border_str), "%s %s %s", width, style, color);
        //         return border_str;
        //     }
        //     return "1px solid #000000"; /* Default */
        // }
        default:
            return NULL;
    }
}

/**
 * @brief Get CSS property value as integer (for font size)
 */
int css_get_property_int(css_select_results *results, uint32_t property, int default_value)
{
    if (!results) return default_value;
    
    css_computed_style *style = results->styles[CSS_PSEUDO_ELEMENT_NONE];
    if (!style) return default_value;
    
    switch (property) {
        case CSS_PROP_FONT_SIZE: {
            css_fixed size;
            css_unit unit;
            uint8_t font_size = css_computed_font_size(style, &size, &unit);
            
            /* Convert to pixels */
            if (font_size == CSS_FONT_SIZE_DIMENSION) {
                int px_size = FIXTOINT(size);
                /* Handle different units */
                switch (unit) {
                    case CSS_UNIT_PX: return px_size;
                    case CSS_UNIT_PT: return (int)(px_size * 1.33); /* pt to px */
                    case CSS_UNIT_EM: return (int)(px_size * 16); /* em to px (assuming 16px base) */
                    case CSS_UNIT_PCT: return (int)(px_size * 16 / 100.0); /* % to px */
                    default: return default_value;
                }
            }
            return default_value;
        }
        default:
            return default_value;
    }
}
