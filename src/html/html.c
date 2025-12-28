/*
 * This file is part of Hubbub.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 *
 * Copyright 2008 Andrew Sidwell <takkaria@netsurf-browser.org> 
 * Copyright 2008 John-Mark Bell <jmb@netsurf-browser.org>
 */

#include "common/common.h"
#include "common/shared.h"
#include "libcss/errors.h"
#include "libcss/stylesheet.h"
#include "libxml/tree.h"
#include "r_local.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unicode/ucnv.h>
#include <unistd.h>
#include <wchar.h>
#define _GNU_SOURCE /* for strndup */
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include <libxml/HTMLparser.h>
#include <libxml/HTMLtree.h>
#include <libxml/debugXML.h>

#include <hubbub/parser.h>
#include <hubbub/tree.h>
#include <libcss/libcss.h>
#include <libcss/select.h>
#define LAY_IMPLEMENTATION
#include "layout.h"

#include "html.h"
#include "animation/anim_state.h"
#include "css/css_animation.h"
#include "css.h"

#define UNUSED(x) ((x)=(x))

#define GETLAYID(node) ((userdata*)node->_private)->layid

// Macro to apply animated opacity to a color
#define APPLY_ANIMATED_OPACITY(color, opacity) \
    ((COLOR32){(color).r, (color).g, (color).b, (BYTE)((color).a * (opacity))})

/**
 * Error codes
 */
typedef enum error_code {
	OK,
	NOMEM,
	BADENCODING,
	ENCODINGCHANGE
} error_code;

/**
 * Source of encoding information
 */
typedef enum encoding_source {
	ENCODING_SOURCE_HEADER,
	ENCODING_SOURCE_DETECTED,
	ENCODING_SOURCE_META
} encoding_source;

/**
 * Our context
 */
typedef struct context {
	hubbub_parser *parser;			/**< Underlying parser */

	htmlDocPtr document;			/**< Document we're building */

	const char *encoding;			/**< The charset of the input */
	encoding_source enc_source;		/**< The encoding source */

#define NUM_NAMESPACES (6)
	xmlNsPtr namespaces[NUM_NAMESPACES];	/**< XML namespaces */
#undef NUM_NAMESPACES

	hubbub_tree_handler tree_handler;	/**< Hubbub tree callbacks */
	
	// Layout integration
	lay_context* layout_ctx;			/**< Layout context */
	struct PZHashTable *node_to_layout_id; /**< XML node -> layout ID mapping */

	// Animation integration
	animation_manager_t* anim_mgr;	/**< Animation manager */

	// Keyframes storage
	int num_keyframes;			/**< Number of stored keyframe animations */
	#define MAX_KEYFRAMES 32
	struct {
		char name[64];			/**< Animation name */
		keyframe_t keyframes[32];	/**< Keyframe data */
		int keyframe_count;		/**< Number of keyframes */
	} keyframes_store[MAX_KEYFRAMES];

	// CSS selection context
	css_select_ctx *css_select_ctx;	/**< CSS selection context */
	css_stylesheet *css_stylesheet;	/**< Parsed stylesheet from <style> tags */

	int inhead;
} context;

#include "css.h"
typedef struct userdata{
	uint refcount;
	lay_id layid;
	int __ln;
	LPCSTR __f;
	LPCSS parsedStyle;
	LPCSS computedStyle;
	uint32_t animation_id;  /**< Animation instance ID if this element is animated */
	COLOR32 bg_color;        /**< Background color for rendering */
	bool has_bg_color;       /**< Whether background color is set */
} userdata;

#define ALLOCUDREF(c,ud,rc) \
	assert(c); \
	assert(c->layout_ctx); \
	userdata *ud = malloc(sizeof(userdata)); \
	ud->__f = __FILE__; \
	ud->__ln= __LINE__; \
	ud->layid = lay_item(c->layout_ctx); \
	ud->parsedStyle = NULL; \
	ud->computedStyle = NULL; \
	ud->animation_id = 0; \
	ud->has_bg_color = false; \
	ud->refcount = rc;

#define ALLOCUD(c,ud) \
	assert(c); \
	assert(c->layout_ctx); \
	userdata *ud = malloc(sizeof(userdata)); \
	ud->__f = __FILE__; \
	ud->__ln= __LINE__; \
	ud->layid = lay_item(c->layout_ctx); \
	ud->parsedStyle = NULL; \
	ud->computedStyle = NULL; \
	ud->animation_id = 0; \
	ud->has_bg_color = false; \
	ud->refcount = 0;

/**
 * Mapping of namespace prefixes to URIs, indexed by hubbub_ns.
 */
static struct {
	const char *prefix;
	const char *url;
} namespaces[] = {
	{ NULL, NULL },
	{ NULL, "http://www.w3.org/1999/xhtml" },
	{ "math", "http://www.w3.org/1998/Math/MathML" },
	{ "svg", "http://www.w3.org/2000/svg" },
	{ "xlink", "http://www.w3.org/1999/xlink" },
	/** \todo Oh dear. LibXML2 refuses to create any namespace with a 
	 * prefix of "xml". That sucks, royally. */
	{ "xml", "http://www.w3.org/XML/1998/namespace" },
	{ "xmlns", "http://www.w3.org/2000/xmlns/" }
};

static inline char *c_string_from_hubbub_string(context *ctx, 
		const hubbub_string *str);
static void create_namespaces(context *ctx, xmlNode *root);
static hubbub_error create_comment(void *ctx, const hubbub_string *data, 
		void **result);
static hubbub_error create_doctype(void *ctx, const hubbub_doctype *doctype,
		void **result);
static hubbub_error create_element(void *ctx, const hubbub_tag *tag, 
		void **result);
static hubbub_error create_text(void *ctx, const hubbub_string *data, 
		void **result);
static hubbub_error ref_node(void *ctx, void *node);
static hubbub_error unref_node(void *ctx, void *node);
static hubbub_error append_child(void *ctx, void *parent, void *child, 
		void **result);
static hubbub_error insert_before(void *ctx, void *parent, void *child, 
		void *ref_child, void **result);
static hubbub_error remove_child(void *ctx, void *parent, void *child, 
		void **result);
static hubbub_error clone_node(void *ctx, void *node, bool deep, void **result);
static hubbub_error reparent_children(void *ctx, void *node, void *new_parent);
static hubbub_error get_parent(void *ctx, void *node, bool element_only, 
		void **result);
static hubbub_error has_children(void *ctx, void *node, bool *result);
static hubbub_error form_associate(void *ctx, void *form, void *node);
static hubbub_error add_attributes(void *ctx, void *node,
		const hubbub_attribute *attributes, uint32_t n_attributes);
static hubbub_error set_quirks_mode(void *ctx, hubbub_quirks_mode mode);
static hubbub_error change_encoding(void *ctx, const char *charset);

/* Prototype tree handler struct */
static hubbub_tree_handler tree_handler = {
	create_comment,
	create_doctype,
	create_element,
	create_text,
	ref_node,
	unref_node,
	append_child,
	insert_before,
	remove_child,
	clone_node,
	reparent_children,
	get_parent,
	has_children,
	form_associate,
	add_attributes,
	set_quirks_mode,
	change_encoding,
	NULL
};

/******************************************************************************
 * Main hubbub driver code                                                    *
 ******************************************************************************/
static error_code create_context(const char *charset, context **ctx);
static void destroy_context(context *c);
static error_code parse_chunk(context *c, const uint8_t *data, size_t len);
static error_code parse_completed(context *c);
int html_render_init(context *ctx);

/* Layout helper functions */
static int extract_number(const char *css, const char *property);
static color32_t parse_css_color(const char *color_str);
void apply_css_to_layout(context *c, xmlNode *node, const char *css);
void print_layout_info(lay_context *layout_ctx, xmlDoc *document, context *c);
static void print_node_layout(lay_context *layout_ctx, xmlNode *node, int depth, context *c);
static void apply_computed_style_to_lay(context *ctx, xmlNode *node, const css_select_results *results);



/**
 * Create a parsing context
 *
 * \param charset  The charset the input data is in, or NULL to autodetect
 * \param ctx      Location to receive context
 * \return OK on success, 
 *         NOMEM on memory exhaustion, 
 *         BADENCODING if charset isn't supported
 */
error_code create_context(const char *charset, context **ctx)
{
	context *c;
	hubbub_parser_optparams params;
	uint32_t i;
	hubbub_error error;

	c = malloc(sizeof(context));
	if (c == NULL)
		return NOMEM;

	c->parser = NULL;
	c->encoding = charset;
	c->enc_source = ENCODING_SOURCE_HEADER;
	c->document = NULL;

	/* Create the parser */
	error = hubbub_parser_create(c->encoding, true, &c->parser);
	if (error != HUBBUB_OK) {
		free(c);
		if (error == HUBBUB_BADENCODING)
			return BADENCODING;
		else
			return NOMEM;	/* Assume OOM */
	}

	/* Create the root node of the document */
	c->document = htmlNewDocNoDtD(NULL, NULL);
	if (c->document == NULL) {
		hubbub_parser_destroy(c->parser);
		free(c);
		return NOMEM;
	}

	// layout context 
	c->layout_ctx = malloc(sizeof(lay_context));
	/* Initialize layout context */
	lay_init_context(c->layout_ctx);
	lay_reset_context(c->layout_ctx); /* Clear any existing data */
	lay_reserve_items_capacity(c->layout_ctx, 1000); /* Pre-allocate */

	// animation manager
	c->anim_mgr = malloc(sizeof(animation_manager_t));
	if (c->anim_mgr == NULL) {
		lay_destroy_context(c->layout_ctx);
		free(c->layout_ctx);
		hubbub_parser_destroy(c->parser);
		free(c);
		return NOMEM;
	}
	anim_manager_init(c->anim_mgr);

	// CSS selection context
	css_error css_err = css_select_ctx_create(&c->css_select_ctx);
	if (css_err != CSS_OK) {
		anim_manager_cleanup(c->anim_mgr);
		free(c->anim_mgr);
		lay_destroy_context(c->layout_ctx);
		free(c->layout_ctx);
		hubbub_parser_destroy(c->parser);
		free(c);
		return NOMEM;
	}
	c->css_stylesheet = NULL;

	/* Allocate userdata for document */
	ALLOCUD(c,doc_ud);
	if (doc_ud == NULL) {
		hubbub_parser_destroy(c->parser);
		free(c);
		return NOMEM;
	}

	

	size2_t const viewportsize = R_GetViewPortSize();

	lay_set_size(c->layout_ctx, doc_ud->layid, (lay_vec2){viewportsize.width,viewportsize.height});

	c->document->_private = (void *) doc_ud;

	for (i = 0;
		i < sizeof(c->namespaces) / sizeof(c->namespaces[0]); i++) {
		c->namespaces[i] = NULL;
	}

	/* The following are both needed to make hubbub do anything. If it has
	 * no tree handler or document node registered, it won't attempt to
	 * build a tree. */

	/* Register tree handler with hubbub */
	c->tree_handler = tree_handler;
	c->tree_handler.ctx = (void *) c;

	params.tree_handler = &c->tree_handler;
	hubbub_parser_setopt(c->parser, HUBBUB_PARSER_TREE_HANDLER, &params);

	/* Also tell it about the document node (referencing it first) */
	ref_node(c, c->document);
	params.document_node = c->document;
	hubbub_parser_setopt(c->parser, HUBBUB_PARSER_DOCUMENT_NODE, &params);

	*ctx = c;

	return OK;
}

/**
 * Destroy a parsing context
 *
 * \param c  Context to destroy
 */
void destroy_context(context *c)
{
	if (c == NULL)
		return;

	if (c->parser != NULL)
		hubbub_parser_destroy(c->parser);

	if (c->document != NULL) {
		/* Free userdata for document node */
		userdata *doc_ud = (userdata *) c->document->_private;
		if (doc_ud != NULL) {
			free(doc_ud);
		}
		xmlFreeDoc(c->document);
	}

	/* Clean up layout context */
	lay_destroy_context(c->layout_ctx);

	/* Clean up animation manager */
	if (c->anim_mgr != NULL) {
		anim_manager_cleanup(c->anim_mgr);
		free(c->anim_mgr);
		c->anim_mgr = NULL;
	}

	/* Clean up CSS resources */
	if (c->css_stylesheet != NULL) {
		css_stylesheet_destroy(c->css_stylesheet);
		c->css_stylesheet = NULL;
	}
	if (c->css_select_ctx != NULL) {
		css_select_ctx_destroy(c->css_select_ctx);
		c->css_select_ctx = NULL;
	}

	c->parser = NULL;
	c->encoding = NULL;
	c->document = NULL;

	free(c);

	return;
}

/**
 * Parse a chunk of the input document
 *
 * \param c     Parsing context
 * \param data  Data buffer
 * \param len   Length, in bytes, of data in buffer
 * \return OK on success,
 *         ENCODINGCHANGE if the encoding needs changing
 */
error_code parse_chunk(context *c, const uint8_t *data, size_t len)
{
	hubbub_error err;

	err = hubbub_parser_parse_chunk(c->parser, (uint8_t *) data, len);
	if (err == HUBBUB_ENCODINGCHANGE)
		return ENCODINGCHANGE;

	return OK;
}

/**
 * Inform that we've run out of input to parse
 *
 * \param c  Parsing context
 * \return OK.
 */
error_code parse_completed(context *c)
{
	hubbub_error error;

	error = hubbub_parser_completed(c->parser);
	/** \todo error handling */

	return OK;
}

/******************************************************************************
 * Helper functions for tree building                                         *
 ******************************************************************************/

/**
 * Convert a hubbub string to a C string
 *
 * \param ctx  Our context
 * \param str  The string to convert
 * \return Pointer to C string, must be freed
 *
 * This is a simple utility routine, as libXML expects data to be C strings.
 * If we were implementing our own tree, we might store hubbub-style strings
 * instead (with the associated memory saving)
 */
char *c_string_from_hubbub_string(context *ctx, const hubbub_string *str)
{
	return strndup((const char *) str->ptr, (int) str->len);
}

/**
 * Initialise a context's XML namespaces
 *
 * \param ctx   Our context
 * \param root  The root node of the XML tree
 *
 * Again, this is specific to the needs of libXML.
 */
void create_namespaces(context *ctx, xmlNode *root)
{
	uint32_t i;

	/* Index 0 is the NULL namespace, so skip over it */
	for (i = 1; i < sizeof(namespaces) / sizeof(namespaces[0]); i++) {
		ctx->namespaces[i - 1] = xmlNewNs(root, 
				BAD_CAST namespaces[i].url, 
				BAD_CAST namespaces[i].prefix);

		/* Expect "xml" to fail here */
		if (ctx->namespaces[i - 1] == NULL) {
			fprintf(stderr, 
				"WARNING: Failed creating namespace %s\n", 
					namespaces[i].prefix);
		}
	}
}

/******************************************************************************
 * Tree callbacks for hubbub                                                  *
 ******************************************************************************/

/**
 * Create a comment node
 *
 * \param ctx     Our context
 * \param data    The comment body
 * \param result  Location to receive manufactured node
 * \return HUBBUB_OK on success, appropriate error otherwise
 *
 * Postcondition: if successful, result's reference count must be 1.
 */
hubbub_error create_comment(void *ctx, const hubbub_string *data, void **result)
{
	context *c = (context *) ctx;
	char *content;
	xmlNodePtr n;

	content = c_string_from_hubbub_string(c, data);
	if (content == NULL)
		return HUBBUB_NOMEM;

	n = xmlNewDocComment(c->document, BAD_CAST content);
	if (n == NULL) {
		free(content);
		return HUBBUB_NOMEM;
	}
	ALLOCUDREF(c, ud,1);
	n->_private = (void *) ud;

	free(content);

	*result = (void *) n;

	return HUBBUB_OK;
}

/**
 * Create a doctype node
 *
 * \param ctx      Our context
 * \param doctype  Data for doctype node (name, public ID and system ID)
 * \param result   Location to receive manufactured node
 * \return HUBBUB_OK on success, appropriate error otherwise
 *
 * Postcondition: if successful, result's reference count must be 1.
 */
hubbub_error create_doctype(void *ctx, const hubbub_doctype *doctype, void **result)
{
	context *c = (context *) ctx;
	char *name, *public = NULL, *system = NULL;
	xmlDtdPtr n;

	name = c_string_from_hubbub_string(c, &doctype->name);
	if (name == NULL)
		return HUBBUB_NOMEM;

	/* May not have public ID */
	if (!doctype->public_missing) {
		public = c_string_from_hubbub_string(c, &doctype->public_id);
		if (public == NULL) {
			free(name);
			return HUBBUB_NOMEM;
		}
	}

	/* May not have system ID */
	if (!doctype->system_missing) {
		system = c_string_from_hubbub_string(c, &doctype->system_id);
		if (system == NULL) {
			free(public);
			free(name);
			return HUBBUB_NOMEM;
		}
	}

	n = xmlNewDtd(c->document, BAD_CAST name,
			BAD_CAST (public ? public : ""),
			BAD_CAST (system ? system : ""));
	if (n == NULL) {
		free(system);
		free(public);
		free(name);
		return HUBBUB_NOMEM;
	}
	/* Again, reference count must be 1, and allocate userdata */
	ALLOCUDREF(c, ud,1);
	if (ud == NULL) {
		free(system);
		free(public);
		free(name);
		return HUBBUB_NOMEM;
	}
	n->_private = (void *) ud;

	*result = (void *) n;

	free(system);
	free(public);
	free(name);

	return HUBBUB_OK;
}

/**
 * Create an element node
 *
 * \param ctx     Our context
 * \param tag     Data for node
 * \param result  Location to receive manufactured node
 * \return HUBBUB_OK on success, appropriate error otherwise
 *
 * Postcondition: if successful, result's reference count must be 1.
 */
hubbub_error create_element(void *ctx, const hubbub_tag *tag, void **result)
{
	context *c = (context *) ctx;
	char *name;
	xmlNodePtr n;

	name = c_string_from_hubbub_string(c, &tag->name);
	if (name == NULL)
		return HUBBUB_NOMEM;

	if (c->namespaces[0] != NULL) {
		n = xmlNewDocNode(c->document, c->namespaces[tag->ns - 1],
				BAD_CAST name, NULL);
	} else {
		n = xmlNewDocNode(c->document, NULL, BAD_CAST name, NULL);

		/* We're creating the root node of the document. Therefore,
		 * create the namespaces and set this node's namespace */
		if (n != NULL && c->namespaces[0] == NULL) {
			create_namespaces(c, (void *) n);

			xmlSetNs(n, c->namespaces[tag->ns - 1]);
		}
	}
	if (n == NULL) {
		free(name);
		return HUBBUB_NOMEM;
	}
	/* Reference count must be 1, and allocate userdata */
	ALLOCUDREF(c, ud, 1)
	if (ud == NULL) {
		free(name);
		return HUBBUB_NOMEM;
	}
	n->_private = (void *) ud;

	/* Attempt to add attributes to node */
	if (tag->n_attributes > 0 && add_attributes(ctx, (void *) n,
			tag->attributes, tag->n_attributes) != 0) {
		xmlFreeNode(n);
		free(name);
		return HUBBUB_NOMEM;
	}

	*result = (void *) n;

	// 获取布局 ID
	lay_id layout_id = ud->layid;
	
	// 为HTML元素设置根级布局
	if (strcmp(name, "html") == 0) {
		// HTML元素作为根级容器：填充整个窗口，使用列布局
		lay_set_behave(c->layout_ctx, layout_id, LAY_HFILL | LAY_VFILL);
		lay_set_contain(c->layout_ctx, layout_id, LAY_COLUMN);
		// 设置默认边距
		lay_set_margins_ltrb(c->layout_ctx, layout_id, 0, 0, 0, 0);
	}
	// 为body元素设置布局
	else if (strcmp(name, "body") == 0) {
		// Body元素：填充可用空间，使用文档流布局
		lay_set_behave(c->layout_ctx, layout_id, LAY_HFILL | LAY_VFILL);
		// 设置默认边距（符合浏览器标准）
		lay_set_margins_ltrb(c->layout_ctx, layout_id, 8, 8, 8, 8);
	}
	// 为div元素设置布局
	else if (strcmp(name, "div") == 0) {
		// 默认块级元素：填充可用宽度，垂直排列子元素
		lay_set_behave(c->layout_ctx, layout_id, LAY_HFILL | LAY_VFILL);
		lay_set_contain(c->layout_ctx, layout_id, LAY_COLUMN);
		// 设置默认边距
		lay_set_margins_ltrb(c->layout_ctx, layout_id, 0, 0, 0, 0);
	}
	// 为span元素设置布局
	else if (strcmp(name, "span") == 0) {
		// 内联元素：不设置填充，大小由内容决定
		// 保持默认布局行为
	}
	// 为p元素设置布局
	else if (strcmp(name, "p") == 0) {
		// 段落元素：块级，有下边距，垂直排列内容
		lay_set_behave(c->layout_ctx, layout_id, LAY_HFILL | LAY_VFILL);
		lay_set_contain(c->layout_ctx, layout_id, LAY_COLUMN);
		// 设置段落间距
		lay_set_margins_ltrb(c->layout_ctx, layout_id, 0, 0, 0, 8);
	}
	// 为h1-h6元素设置布局
	else if (strncmp(name, "h", 1) == 0 && strlen(name) == 2 &&
			 name[1] >= '1' && name[1] <= '6') {
		// 标题元素：块级，有边距，垂直排列内容
		lay_set_behave(c->layout_ctx, layout_id, LAY_HFILL | LAY_VFILL);
		lay_set_contain(c->layout_ctx, layout_id, LAY_COLUMN);
		// 设置标题边距
		lay_set_margins_ltrb(c->layout_ctx, layout_id, 0, 0, 0, 6);
	}
	// 为ul和ol元素设置布局
	else if (strcmp(name, "ul") == 0 || strcmp(name, "ol") == 0) {
		// 列表元素：块级，有边距，垂直排列列表项
		lay_set_behave(c->layout_ctx, layout_id, LAY_HFILL | LAY_VFILL);
		lay_set_contain(c->layout_ctx, layout_id, LAY_COLUMN);
		// 设置列表边距
		lay_set_margins_ltrb(c->layout_ctx, layout_id, 0, 0, 0, 8);
	}
	// 为li元素设置布局
	else if (strcmp(name, "li") == 0) {
		// 列表项元素：块级，有左边距用于列表标记
		lay_set_behave(c->layout_ctx, layout_id, LAY_HFILL | LAY_VFILL);
		// 设置列表项边距
		lay_set_margins_ltrb(c->layout_ctx, layout_id, 20, 0, 0, 2);
	}
	// 为table元素设置布局
	else if (strcmp(name, "table") == 0) {
		// 表格元素：填充可用宽度，自动布局
		lay_set_behave(c->layout_ctx, layout_id, LAY_HFILL | LAY_VFILL);
		// 表格默认边距
		lay_set_margins_ltrb(c->layout_ctx, layout_id, 0, 0, 0, 8);
	}
	// 为tr元素设置布局
	else if (strcmp(name, "tr") == 0) {
		// 表格行元素：行布局，填充可用宽度
		lay_set_behave(c->layout_ctx, layout_id, LAY_HFILL | LAY_VFILL);
		lay_set_contain(c->layout_ctx, layout_id, LAY_ROW);
	}
	// 为td和th元素设置布局
	else if (strcmp(name, "td") == 0 || strcmp(name, "th") == 0) {
		// 表格单元格元素：填充可用空间
		lay_set_behave(c->layout_ctx, layout_id, LAY_HFILL | LAY_VFILL);
		// 设置单元格内边距
		lay_set_margins_ltrb(c->layout_ctx, layout_id, 4, 4, 4, 4);
	}
	// 为form元素设置布局
	else if (strcmp(name, "form") == 0) {
		// 表单元素：块级，有边距，垂直排列表单项
		lay_set_behave(c->layout_ctx, layout_id, LAY_HFILL | LAY_VFILL);
		lay_set_contain(c->layout_ctx, layout_id, LAY_COLUMN);
		// 设置表单边距
		lay_set_margins_ltrb(c->layout_ctx, layout_id, 0, 0, 0, 16);
	}
	// 为input, textarea, select元素设置布局
	else if (strcmp(name, "input") == 0 || strcmp(name, "textarea") == 0 ||
			 strcmp(name, "select") == 0) {
		// 表单控件元素：固定高度，填充宽度
		lay_set_behave(c->layout_ctx, layout_id, LAY_HFILL | LAY_VFILL);
		// 设置默认大小
		lay_set_size_xy(c->layout_ctx, layout_id, 0, 24);
		// 设置表单控件边距
		lay_set_margins_ltrb(c->layout_ctx, layout_id, 0, 4, 0, 4);
	}
	// 为img元素设置布局
	else if (strcmp(name, "img") == 0) {
		// 图片元素：内联块级，由内容决定大小
		// 保持默认布局行为，不设置填充
		// 设置图片边距
		lay_set_margins_ltrb(c->layout_ctx, layout_id, 0, 0, 0, 0);
	}
	// 为a元素设置布局
	else if (strcmp(name, "a") == 0) {
		// 链接元素：内联元素，由内容决定大小
		// 保持默认布局行为
		// 设置链接下划线间距
		lay_set_margins_ltrb(c->layout_ctx, layout_id, 0, 0, 0, 2);
	}
	// 为br元素设置布局
	else if (strcmp(name, "br") == 0) {
		// 换行元素：强制换行
		lay_set_behave(c->layout_ctx, layout_id, LAY_HFILL | LAY_VFILL);
		// 设置最小高度
		lay_set_size_xy(c->layout_ctx, layout_id, 0, 16);
	}
	
	free(name);

	return HUBBUB_OK;
}

/**
 * Create a text node
 *
 * \param ctx     Our context
 * \param data    Node data
 * \param result  Location to receive manufactured node
 * \return HUBBUB_OK on success, appropriate error otherwise
 *
 * Postcondition: if successfult, result's reference count must be 1.
 */
hubbub_error create_text(void *ctx, const hubbub_string *data, void **result)
{
	context *c = (context *) ctx;
	xmlNodePtr n;

	n = xmlNewDocTextLen(c->document, BAD_CAST data->ptr, (int) data->len);
	if (n == NULL) {
		return HUBBUB_NOMEM;
	}
	/* Reference count must be 1, and allocate userdata */
	ALLOCUDREF(c, ud, 1);
	if (ud == NULL) {
		return HUBBUB_NOMEM;
	}
	n->_private = (void *) ud;

	*result = (void *) n;

	return HUBBUB_OK;
}

/**
 * Increase a node's reference count
 *
 * \param ctx   Our context
 * \param node  The node to reference
 * \return HUBBUB_OK on success, appropriate error otherwise
 */
hubbub_error ref_node(void *ctx, void *node)
{
	context *c = (context *) ctx;

	if (node == c->document) {
		xmlDoc *n = (xmlDoc *) node;
		userdata *ud = (userdata *) n->_private;
		assert(ud!=NULL);
		ud->refcount++;
	} else {
		xmlNode *n = (xmlNode *) node;
		userdata *ud = (userdata *) n->_private;
		if (ud == NULL) {
			/* This should not happen for properly created nodes */
			return HUBBUB_NOMEM;
		}
		ud->refcount++;
	}

	return HUBBUB_OK;
}

/**
 * Decrease a node's reference count
 *
 * \param ctx   Our context
 * \param node  The node to unreference
 * \return HUBBUB_OK on success, appropriate error otherwise
 *
 * Postcondition: If the node's reference count becomes zero, and it has no 
 * parent, and it is not the document node, then it is destroyed.
 */
hubbub_error unref_node(void *ctx, void *node)
{
	context *c = (context *) ctx;

	if (node == c->document) {
		xmlDoc *n = (xmlDoc *) node;
		userdata *ud = (userdata *) n->_private;
		
		/* Trap any attempt to unref a non-referenced node */
		assert(ud != NULL && ud->refcount != 0 && "Node has refcount of zero");

		/* Never destroy document node */
		ud->refcount--;
	} else {
		xmlNode *n = (xmlNode *) node;
		userdata *ud = (userdata *) n->_private;

		/* Trap any attempt to unref a non-referenced node */
		assert(ud != NULL && ud->refcount != 0 && "Node has refcount of zero");

		ud->refcount--;

		/* Destroy node, if it has no parent and refcount is zero */
		if (ud->refcount == 0 && n->parent == NULL) {
			free(ud);  /* Free userdata structure */
			xmlFreeNode(n);
		}
	}

	return HUBBUB_OK;
}

/**
 * Append a node to the end of another's child list
 *
 * \param ctx     Our context
 * \param parent  The node to append to
 * \param child   The node to append
 * \param result  Location to receive appended node
 * \return HUBBUB_OK on success, appropriate error otherwise
 *
 * Postcondition: if successful, result's reference count is increased by 1
 *
 * Important: *result may not == child (e.g. if text nodes got coalesced)
 */
hubbub_error append_child(void *ctx, void *parent, void *child, void **result)
{
	xmlNode *chld = (xmlNode *) child;
	xmlNode *p = (xmlNode *) parent;

	/* Note: this does not exactly follow the current specification.
	 * See http://www.whatwg.org/specs/web-apps/current-work/ \
	 *     multipage/tree-construction.html#insert-a-character
	 * for the exact behaviour required.
	 */

	if (chld->type == XML_TEXT_NODE && p->last != NULL &&
			p->last->type == XML_TEXT_NODE) {
		/* Need to clone the child, as libxml will free it if it
		 * merges the content with a pre-existing text node. */

		userdata cud = *((userdata*)chld->_private);
		chld = xmlCopyNode(chld, 0);
		if (chld == NULL)
			return HUBBUB_NOMEM;

		if (chld->_private == NULL) {
			// 如果 xmlCopyNode 没有拷贝 _private，则需要手动拷贝
			userdata *orig_private = (userdata*)((xmlNode *) child)->_private;
			if (orig_private != NULL) {
				chld->_private = malloc(sizeof(userdata));
				memcpy(chld->_private,orig_private,sizeof(userdata));
			}
	   	}
		*result = xmlAddChild(p, chld);

		assert(*result != (void *) chld);
	} else {
		*result = xmlAddChild(p, chld);
		assert(chld->_private);
	}

	if (*result == NULL)
		return HUBBUB_NOMEM;

	ref_node(ctx, *result);

	// 获取父节点的布局 ID
	   lay_id parent_id = GETLAYID(p);
	   
	   // 获取子节点的布局 ID（使用 *result 而不是 chld，因为 chld 可能已被释放）
	   lay_id child_id;
	   if (*result != NULL) {
	       xmlNode *result_node = (xmlNode *)*result;
	       child_id = GETLAYID(result_node);
	   } else {
	       child_id = LAY_INVALID_ID;
	   }

	   // 将子布局项插入到父布局项中
	   if (parent_id != LAY_INVALID_ID && child_id != LAY_INVALID_ID) {
			if(!lay_isinserted(((context *)ctx)->layout_ctx,child_id)){
	       		lay_insert(((context *)ctx)->layout_ctx, parent_id, child_id);
			}
	   }

	return HUBBUB_OK;
}

/**
 * Insert a node into another's child list
 *
 * \param ctx        Our context
 * \param parent     The node to insert into
 * \param child      The node to insert
 * \param ref_child  The node to insert before
 * \param result     Location to receive inserted node
 * \return HUBBUB_OK on success, appropriate error otherwise
 *
 * Postcondition: if successful, result's reference count is increased by 1
 *
 * Important: *result may not == child (e.g. if text nodes got coalesced)
 */
hubbub_error insert_before(void *ctx, void *parent, void *child, void *ref_child,
		void **result)
{
	xmlNode *chld = (xmlNode *) child;
	xmlNode *ref = (xmlNode *) ref_child;

	if (chld->type == XML_TEXT_NODE && ref->prev != NULL && 
			ref->prev->type == XML_TEXT_NODE) {
		/* Clone text node, as it'll be freed by libxml */
		chld = xmlCopyNode(chld, 0);
		if (chld == NULL)
			return HUBBUB_NOMEM;

		*result = xmlAddNextSibling(ref->prev, chld);
		if (chld->_private == NULL) {
			// 如果 xmlCopyNode 没有拷贝 _private，则需要手动拷贝
			userdata *orig_private = (userdata*)((xmlNode *) child)->_private;
			if (orig_private != NULL) {
				chld->_private = malloc(sizeof(userdata));
				memcpy(chld->_private,orig_private,sizeof(userdata));
			}
		}

		assert(*result != (void *) chld);
	} else {
		*result = xmlAddPrevSibling(ref, chld);
	}

	if (*result == NULL)
		return HUBBUB_NOMEM;

	ref_node(ctx, *result);

	return HUBBUB_OK;
}

/**
 * Remove a node from another's child list
 *
 * \param ctx     Our context
 * \param parent  The node to remove from
 * \param child   The node to remove
 * \param result  Location to receive removed node
 * \return HUBBUB_OK on success, appropriate error otherwise
 *
 * Postcondition: if successful, result's reference count is increased by 1
 */
hubbub_error remove_child(void *ctx, void *parent, void *child, void **result)
{
	xmlNode *chld = (xmlNode *) child;

	xmlUnlinkNode(chld);

	*result = child;

	ref_node(ctx, *result);

	return HUBBUB_OK;
}

/**
 * Clone a node
 * 
 * \param ctx     Our context
 * \param node    The node to clone
 * \param deep    True to clone entire subtree, false to clone only the node
 * \param result  Location to receive clone
 * \return HUBBUB_OK on success, appropriate error otherwise
 *
 * Postcondition: if successful, result's reference count must be 1.
 */
hubbub_error clone_node(void *ctx, void *node, bool deep, void **result)
{
	xmlNode *n = (xmlNode *) node;
	context* c = (context *) ctx;
	*result = xmlCopyNode(n, deep ? 1 : 2);

	if (*result == NULL)
		return HUBBUB_NOMEM;

	memcpy(((xmlNode *)(*result))->_private,n->_private,sizeof(userdata));

	return HUBBUB_OK;
}

/**
 * Move all the children of one node to another
 *
 * \param ctx         Our context
 * \param node        The initial parent node
 * \param new_parent  The new parent node
 * \return HUBBUB_OK on success, appropriate error otherwise
 */
hubbub_error reparent_children(void *ctx, void *node, void *new_parent)
{
	xmlNode *n = (xmlNode *) node;
	xmlNode *p = (xmlNode *) new_parent;
	xmlNode *child;

	for (child = n->children; child != NULL; ) {
		xmlNode *next = child->next;

		xmlUnlinkNode(child);

		if (xmlAddChild(p, child) == NULL)
			return HUBBUB_NOMEM;

		child = next;
	}

	return HUBBUB_OK;
}

/**
 * Retrieve the parent of a node
 *
 * \param ctx           Our context
 * \param node          Node to retrieve the parent of
 * \param element_only  True if the parent must be an element, false otherwise
 * \param result        Location to receive parent node
 * \return HUBBUB_OK on success, appropriate error otherwise
 *
 * Postcondition: if there is a parent, then result's reference count must be
 * increased.
 */
hubbub_error get_parent(void *ctx, void *node, bool element_only, void **result)
{
	xmlNode *n = (xmlNode *) node;

	*result = (void *) n->parent;

	if (*result != NULL && element_only && 
			((xmlNode *) *result)->type != XML_ELEMENT_NODE) {
		*result = NULL;
	}

	if (*result != NULL)
		ref_node(ctx, *result);

	return HUBBUB_OK;
}

/**
 * Determine if a node has children
 *
 * \param ctx     Our context
 * \param node    The node to inspect
 * \param result  Location to receive result
 * \return HUBBUB_OK on success, appropriate error otherwise
 */
hubbub_error has_children(void *ctx, void *node, bool *result)
{
	xmlNode *n = (xmlNode *) node;

	*result = n->children != NULL;

	return HUBBUB_OK;
}

/**
 * Associate a node with a form
 *
 * \param ctx   Our context
 * \param form  The form to associate with
 * \param node  The node to associate
 * \return HUBBUB_OK on success, appropriate error otherwise
 */
hubbub_error form_associate(void *ctx, void *form, void *node)
{
	/* In this implementation, we do nothing here.
	 * 
	 * If we wish to process forms afterwards, then we would want to use 
	 * this entry point to associate inputs with form elements. This is 
	 * useful because forms may be misnested in the source data and thus
	 * it is not necessarily sufficient to search the resultant DOM to 
	 * perform the association.
	 *
	 * Note that this callback will be called even if the node has
	 * an @form. In that case, the association should be between the node
	 * and the form identified by the ID in @form. This may not be the same
	 * as the form passed in.
	 */
	return HUBBUB_OK;
}

/**
 * Add attributes to a node
 *
 * \param ctx           Our context
 * \param node          The node to add to
 * \param attributes    Array of attributes to add
 * \param n_attributes  Number of entries in array
 * \return HUBBUB_OK on success, appropriate error otherwise
 */
hubbub_error add_attributes(void *ctx, void *node, 
		const hubbub_attribute *attributes, uint32_t n_attributes)
{
	context *c = (context *) ctx;
	xmlNode *n = (xmlNode *) node;
	uint32_t attr;

	for (attr = 0; attr < n_attributes; attr++) {
		xmlAttr *prop;
		char *name, *value;

		name = c_string_from_hubbub_string(c, &attributes[attr].name);
		if (name == NULL)
			return HUBBUB_NOMEM;

		value = c_string_from_hubbub_string(c, &attributes[attr].value);
		if (value == NULL) {
			free(name);
			return HUBBUB_NOMEM;
		}

		printf("DEBUG: attr %d: name='%s', value='%s'\n", attr, name, value ? value : "NULL");
		if (attributes[attr].ns != HUBBUB_NS_NULL && 
				c->namespaces[0] != NULL) {
			prop = xmlNewNsProp(n, 
					c->namespaces[attributes[attr].ns - 1],
					BAD_CAST name, BAD_CAST value);
		} else {
			prop = xmlNewProp(n, BAD_CAST name, BAD_CAST value);
		}
		if (prop == NULL) {
			free(value);
			free(name);
			return HUBBUB_NOMEM;
		}

		if (name != NULL && strcmp(name, "style") == 0) {
            if (value != NULL) {
                printf("DEBUG: style attribute found: '%s'\n", value);
                /* Note: Inline style is now handled by html_getnodestyle() via css_select_style
                   No need to apply it separately here */
                // apply_css_to_layout(c, n, value); // REMOVED - handled by libcss
            }
        }

		if (name != NULL && strcmp(name, "animation") == 0) {
            if (value != NULL) {
                // Parse and start animation
                apply_animation_attribute(c, n, value);
            }
        }

		free(value);
		free(name);
	}

	return HUBBUB_OK;
}

/**
 * Notification of the quirks mode of a document
 *
 * \param ctx   Our context
 * \param mode  The quirks mode
 * \return HUBBUB_OK on success, appropriate error otherwise
 */
hubbub_error set_quirks_mode(void *ctx, hubbub_quirks_mode mode)
{
	/* In this implementation, we do nothing.
	 * 
	 * The quirks mode is really only of any use when applying CSS 
	 * to the resulting DOM tree.
	 */
	return HUBBUB_OK;
}

/**
 * Notification that a potential encoding change is required
 *
 * \param ctx      Our context
 * \param charset  The new charset for the source data
 * \return HUBBUB_OK to continue using the current input handler, 
 *         HUBBUB_ENCODINGCHANGE to stop processing immediately and 
 *                               return control to the client,
 *         appropriate error otherwise.
 */
hubbub_error change_encoding(void *ctx, const char *charset)
{
	context *c = (context *) ctx;
	uint32_t source;
	const char *name;

	/* If we have an encoding here, it means we are *certain* */
	if (c->encoding != NULL) {
		return HUBBUB_OK;
	}

	/* Find the confidence otherwise (can only be from a BOM) */
	name = hubbub_parser_read_charset(c->parser, &source);

	if (source == HUBBUB_CHARSET_CONFIDENT) {
		c->enc_source = ENCODING_SOURCE_DETECTED;
		c->encoding = (char *) charset;
		return HUBBUB_OK;
	}

	/* So here we have something of confidence tentative... */
	/* http://www.whatwg.org/specs/web-apps/current-work/#change */

	/* 2. "If the new encoding is identical or equivalent to the encoding
	 * that is already being used to interpret the input stream, then set
	 * the confidence to confident and abort these steps." */

	/* Whatever happens, the encoding should be set here; either for
	 * reprocessing with a different charset, or for confirming that the
	 * charset is in fact correct */
	c->encoding = charset;
	c->enc_source = ENCODING_SOURCE_META;

	/* Equal encodings will have the same string pointers */
	return (charset == name) ? HUBBUB_OK : HUBBUB_ENCODINGCHANGE;
}

/**
 * Get current animated value for an element (if animated)
 * @param c HTML context
 * @param node XML node
 * @param prop_index Property index to retrieve
 * @param value Output value
 * @return true if value exists and is animated, false otherwise
 */
static bool get_element_animation_value(context *c, xmlNode *node, int prop_index, anim_value_t *value) {
    if (!c || !node || !c->anim_mgr) return false;
    
    userdata *ud = (userdata *) node->_private;
    if (!ud || ud->animation_id == 0) return false;
    
    // Get current animation value
    return anim_manager_get_value(c->anim_mgr, ud->animation_id, prop_index, value);
}

/**
 * Apply animation attribute to an HTML element
 * Parses the animation shorthand and starts the animation if keyframes are found
 */
void apply_animation_attribute(context *c, xmlNode *node, const char *animation_value) {
	if (!c || !node || !animation_value) return;
	if (!c->anim_mgr) return;


	// Parse animation shorthand property
	animation_props_t props;
	if (!css_parse_animation_shorthand(animation_value, &props)) {
		printf("Failed to parse animation: '%s'\n", animation_value);
		return;
	}

	printf("Parsed animation: name='%s', duration=%.2fs, timing=%s, iterations=%d\n",
		   props.name, props.duration, props.timing_func, props.iteration_count);

	// Find keyframes by name
	int keyframes_idx = -1;
	for (int i = 0; i < c->num_keyframes; i++) {
		if (strcasecmp(c->keyframes_store[i].name, props.name) == 0) {
			keyframes_idx = i;
			break;
		}
	}

	if (keyframes_idx == -1) {
		printf("Keyframes '%s' not found in stored keyframes\n", props.name);
		return;
	}

	printf("Found keyframes: '%s' with %d keyframes\n",
		   c->keyframes_store[keyframes_idx].name,
		   c->keyframes_store[keyframes_idx].keyframe_count);

	// Start animation
	uint32_t anim_id = anim_manager_start(
		c->anim_mgr,
		&props,
		c->keyframes_store[keyframes_idx].keyframes,
		c->keyframes_store[keyframes_idx].keyframe_count
	);

	if (anim_id == 0) {
		printf("Failed to start animation\n");
		return;
	}

	printf("Started animation with ID: %u\n", anim_id);

	// Store animation ID in node's userdata
	userdata *ud = (userdata *) node->_private;
	if (ud) {
		ud->animation_id = anim_id;
	}
}

void apply_css_to_layout(context *c, xmlNode *node, const char *css)
{
	lay_id layout_id = GETLAYID(node);
	
	if (layout_id == LAY_INVALID_ID) return;
	
	printf("DEBUG: apply_css_to_layout called with css='%s'\n", css);
	
	/* Enhanced CSS parsing with support for more properties */
	
	/* Display and Flexbox */
	if (strstr(css, "display: flex")) {
		if (strstr(css, "flex-direction: row")) {
			lay_set_contain(c->layout_ctx, layout_id, LAY_ROW);
		} else {
			lay_set_contain(c->layout_ctx, layout_id, LAY_COLUMN);
		}
	}
	
	/* Width and Height */
	if (strstr(css, "width:")) {
		int width = extract_number(css, "width:");
		lay_set_size_xy(c->layout_ctx, layout_id, width,
		               lay_get_size(c->layout_ctx, layout_id)[1]);
	}
	
	if (strstr(css, "height:")) {
		int height = extract_number(css, "height:");
		lay_set_size_xy(c->layout_ctx, layout_id,
		               lay_get_size(c->layout_ctx, layout_id)[0], height);
	}
	
	/* Enhanced Margin parsing - supports both simple and compound values */
	if (strstr(css, "margin:")) {
		const char *margin_start = strstr(css, "margin:");
		margin_start += 7; /* Skip "margin:" */
		
		/* Skip whitespace */
		while (*margin_start && (*margin_start == ' ' || *margin_start == ':')) {
			margin_start++;
		}
		
		/* Parse margin values */
		char margin_str[32];
		int i = 0;
		while (*margin_start && (*margin_start != ';' && *margin_start != '}') && i < sizeof(margin_str) - 1) {
			margin_str[i++] = *margin_start++;
		}
		margin_str[i] = '\0';
		
		/* Parse different margin formats */
		int top, right, bottom, left;
		
		if (strstr(margin_str, "px")) {
			/* Single value: margin: 10px */
			if (sscanf(margin_str, "%dpx", &top) == 1) {
				lay_set_margins_ltrb(c->layout_ctx, layout_id, top, top, top, top);
			}
			/* Two values: margin: 10px 5px */
			else if (sscanf(margin_str, "%dpx %dpx", &top, &right) == 2) {
				lay_set_margins_ltrb(c->layout_ctx, layout_id, top, right, top, right);
			}
			/* Three values: margin: 10px 5px 2px */
			else if (sscanf(margin_str, "%dpx %dpx %dpx", &top, &right, &bottom) == 3) {
				lay_set_margins_ltrb(c->layout_ctx, layout_id, top, right, bottom, right);
			}
			/* Four values: margin: 10px 5px 2px 8px */
			else if (sscanf(margin_str, "%dpx %dpx %dpx %dpx", &top, &right, &bottom, &left) == 4) {
				lay_set_margins_ltrb(c->layout_ctx, layout_id, top, right, bottom, left);
			}
		} else {
			/* Try to parse as plain number */
			int margin_val;
			if (sscanf(margin_str, "%d", &margin_val) == 1) {
				lay_set_margins_ltrb(c->layout_ctx, layout_id, margin_val, margin_val, margin_val, margin_val);
			}
		}
	}
	
	/* Individual margin properties */
	if (strstr(css, "margin-top:")) {
		int top = extract_number(css, "margin-top:");
		lay_scalar l, t, r, b;
		lay_get_margins_ltrb(c->layout_ctx, layout_id, &l, &t, &r, &b);
		lay_set_margins_ltrb(c->layout_ctx, layout_id, l, top, r, b);
	}
	
	if (strstr(css, "margin-right:")) {
		int right = extract_number(css, "margin-right:");
		lay_scalar l, t, r, b;
		lay_get_margins_ltrb(c->layout_ctx, layout_id, &l, &t, &r, &b);
		lay_set_margins_ltrb(c->layout_ctx, layout_id, l, t, right, b);
	}
	
	if (strstr(css, "margin-bottom:")) {
		int bottom = extract_number(css, "margin-bottom:");
		lay_scalar l, t, r, b;
		lay_get_margins_ltrb(c->layout_ctx, layout_id, &l, &t, &r, &b);
		lay_set_margins_ltrb(c->layout_ctx, layout_id, l, t, r, bottom);
	}
	
	if (strstr(css, "margin-left:")) {
		int left = extract_number(css, "margin-left:");
		lay_scalar l, t, r, b;
		lay_get_margins_ltrb(c->layout_ctx, layout_id, &l, &t, &r, &b);
		lay_set_margins_ltrb(c->layout_ctx, layout_id, left, t, r, b);
	}
	
	/* Padding support - new feature */
	if (strstr(css, "padding:")) {
		const char *padding_start = strstr(css, "padding:");
		padding_start += 8; /* Skip "padding:" */
		
		/* Skip whitespace */
		while (*padding_start && (*padding_start == ' ' || *padding_start == ':')) {
			padding_start++;
		}
		
		/* Parse padding values */
		char padding_str[32];
		int i = 0;
		while (*padding_start && (*padding_start != ';' && *padding_start != '}') && i < sizeof(padding_str) - 1) {
			padding_str[i++] = *padding_start++;
		}
		padding_str[i] = '\0';
		
		/* Since layout system doesn't have direct padding support,
		   we'll simulate it by adjusting margins and element size */
		int top, right, bottom, left;
		
		if (strstr(padding_str, "px")) {
			/* Single value: padding: 10px */
			if (sscanf(padding_str, "%dpx", &top) == 1) {
				/* Simulate padding by increasing element size and adjusting margins */
				lay_scalar current_width, current_height;
				lay_get_size_xy(c->layout_ctx, layout_id, &current_width, &current_height);
				lay_set_size_xy(c->layout_ctx, layout_id, current_width + top * 2, current_height + top * 2);
			}
			/* Two values: padding: 10px 5px */
			else if (sscanf(padding_str, "%dpx %dpx", &top, &right) == 2) {
				lay_scalar current_width, current_height;
				lay_get_size_xy(c->layout_ctx, layout_id, &current_width, &current_height);
				lay_set_size_xy(c->layout_ctx, layout_id, current_width + top + right, current_height + top + right);
			}
		}
	}
	
	/* Individual padding properties */
	if (strstr(css, "padding-top:")) {
		int top = extract_number(css, "padding-top:");
		/* Simulate padding by increasing element size */
		lay_scalar current_width, current_height;
		lay_get_size_xy(c->layout_ctx, layout_id, &current_width, &current_height);
		lay_set_size_xy(c->layout_ctx, layout_id, current_width, current_height + top);
	}
	
	if (strstr(css, "padding-right:")) {
		int right = extract_number(css, "padding-right:");
		lay_scalar current_width, current_height;
		lay_get_size_xy(c->layout_ctx, layout_id, &current_width, &current_height);
		lay_set_size_xy(c->layout_ctx, layout_id, current_width + right, current_height);
	}
	
	if (strstr(css, "padding-bottom:")) {
		int bottom = extract_number(css, "padding-bottom:");
		lay_scalar current_width, current_height;
		lay_get_size_xy(c->layout_ctx, layout_id, &current_width, &current_height);
		lay_set_size_xy(c->layout_ctx, layout_id, current_width, current_height + bottom);
	}
	
	if (strstr(css, "padding-left:")) {
		int left = extract_number(css, "padding-left:");
		lay_scalar current_width, current_height;
		lay_get_size_xy(c->layout_ctx, layout_id, &current_width, &current_height);
		lay_set_size_xy(c->layout_ctx, layout_id, current_width + left, current_height);
	}
	
/* Animation property */
	if (strstr(css, "animation:")) {
		const char *anim_start = strstr(css, "animation:") + 10; /* Skip "animation:" including colon */
		
		printf("DEBUG: Found 'animation:', anim_start='%s'\n", anim_start);
		
		/* Skip whitespace */
		while (*anim_start && (*anim_start == ' ')) {
			anim_start++;
		}
		
		printf("DEBUG: After skipping whitespace, anim_start='%s'\n", anim_start);
		
		/* Extract animation value */
		char anim_value[256];
		int i = 0;
		while (*anim_start && (*anim_start != ';' && *anim_start != '}') && i < sizeof(anim_value) - 1) {
			anim_value[i++] = *anim_start++;
		}
		anim_value[i] = '\0';
		
		printf("DEBUG: Found animation in style: '%s'\n", anim_value);
		
		/* Apply animation */
		apply_animation_attribute(c, node, anim_value);
	}
	
	/* Background color support */
	if (strstr(css, "background-color:")) {
		const char *color_start = strstr(css, "background-color:");
		color_start += 16; /* Skip "background-color:" */
		
		/* Skip whitespace */
		while (*color_start && (*color_start == ' ' || *color_start == ':')) {
			color_start++;
		}
		
		/* Extract color value */
		char color_str[32];
		int i = 0;
		while (*color_start && (*color_start != ';' && *color_start != '}') && i < sizeof(color_str) - 1) {
			color_str[i++] = *color_start++;
		}
		color_str[i] = '\0';
		
		/* Parse and apply color */
		COLOR32 bg_color = parse_css_color(color_str);
		printf("DEBUG: Parsed background-color '%s' -> r=%d,g=%d,b=%d,a=%d\n", color_str, bg_color.r, bg_color.g, bg_color.b, bg_color.a);
		
		/* Store color for rendering */
		if (c) {
			userdata *ud = (userdata *)node->_private;
			if (ud) {
				ud->bg_color = bg_color;
				ud->has_bg_color = true;
				printf("DEBUG: Stored bg_color for element\n");
			}
		}
	}
	
/* Animation property */
	if (strstr(css, "animation:")) {
		const char *anim_start = strstr(css, "animation:") + 10; /* Skip "animation:" including colon */
		
		printf("DEBUG: Found 'animation:', anim_start='%s'\n", anim_start);
		
		/* Skip whitespace */
		while (*anim_start && (*anim_start == ' ')) {
			anim_start++;
		}
		
		printf("DEBUG: After skipping whitespace, anim_start='%s'\n", anim_start);
		
		/* Extract animation value */
		char anim_value[256];
		int i = 0;
		while (*anim_start && (*anim_start != ';' && *anim_start != '}') && i < sizeof(anim_value) - 1) {
			anim_value[i++] = *anim_start++;
		}
		anim_value[i] = '\0';
		
		printf("DEBUG: Found animation in style: '%s'\n", anim_value);
		
		/* Apply animation */
		apply_animation_attribute(c, node, anim_value);
	}
}

static int extract_number(const char *css, const char *property)
{
	const char *start = strstr(css, property);
	if (!start) return 0;
	
	start += strlen(property);
	while (*start && (*start == ' ' || *start == ':')) start++;
	
	int num = 0;
	while (*start >= '0' && *start <= '9') {
		num = num * 10 + (*start - '0');
		start++;
	}
	
	return num;
}
/**
 * @brief Enhanced CSS number extraction with unit support
 */
static int extract_number_with_unit(const char *css, const char *property, const char **end_ptr)
{
	const char *start = strstr(css, property);
	if (!start) return 0;
	
	start += strlen(property);
	while (*start && (*start == ' ' || *start == ':')) start++;
	
	int num = 0;
	while (*start >= '0' && *start <= '9') {
		num = num * 10 + (*start - '0');
		start++;
	}
	
	if (end_ptr) *end_ptr = start;
	return num;
}

/**
 * @brief Parse CSS color value
 */
static color32_t parse_css_color(const char *color_str)
{
	if (!color_str) return (color32_t){255, 255, 255, 255}; /* White */
	
	/* Handle hex colors: #RRGGBB or #RGB */
	if (color_str[0] == '#') {
		if (strlen(color_str) == 7) {
			/* #RRGGBB format */
			unsigned int r, g, b;
			if (sscanf(color_str + 1, "%02x%02x%02x", &r, &g, &b) == 3) {
				return (color32_t){(BYTE)r, (BYTE)g, (BYTE)b, 255};
			}
		} else if (strlen(color_str) == 4) {
			/* #RGB format */
			unsigned int r, g, b;
			if (sscanf(color_str + 1, "%1x%1x%1x", &r, &g, &b) == 3) {
				return (color32_t){(BYTE)(r * 17), (BYTE)(g * 17), (BYTE)(b * 17), 255};
			}
		}
	}
	
	/* Handle named colors (basic ones) */
	if (strcmp(color_str, "red") == 0) return (color32_t){255, 0, 0, 255};
	if (strcmp(color_str, "green") == 0) return (color32_t){0, 255, 0, 255};
	if (strcmp(color_str, "blue") == 0) return (color32_t){0, 0, 255, 255};
	if (strcmp(color_str, "white") == 0) return (color32_t){255, 255, 255, 255};
	if (strcmp(color_str, "black") == 0) return (color32_t){0, 0, 0, 255};
	if (strcmp(color_str, "yellow") == 0) return (color32_t){255, 255, 0, 255};
	if (strcmp(color_str, "orange") == 0) return (color32_t){255, 165, 0, 255};
	if (strcmp(color_str, "purple") == 0) return (color32_t){128, 0, 128, 255};
	if (strcmp(color_str, "pink") == 0) return (color32_t){255, 192, 203, 255};
	if (strcmp(color_str, "brown") == 0) return (color32_t){165, 42, 42, 255};
	if (strcmp(color_str, "gray") == 0) return (color32_t){128, 128, 128, 255};
	if (strcmp(color_str, "grey") == 0) return (color32_t){128, 128, 128, 255};
	
	return (color32_t){255, 255, 255, 255}; /* Default to white */
}

/**
 * @brief Enhanced CSS style application with better property support
 */
void apply_enhanced_css_to_layout(context *c, xmlNode *node, const char *css)
{
	printf("DEBUG: apply_enhanced_css_to_layout called for node '%s'\n", node->name ? (char*)node->name : "NULL");
	printf("DEBUG: CSS content: '%s'\n", css);
	lay_id layout_id = GETLAYID(node);
	printf("DEBUG: layout_id=%d\n", layout_id);
	
	if (layout_id == LAY_INVALID_ID) return;
	
	/* Enhanced CSS parsing with support for more properties */
	
	/* Display and Layout */
	if (strstr(css, "display: flex")) {
		if (strstr(css, "flex-direction: row")) {
			lay_set_contain(c->layout_ctx, layout_id, LAY_ROW);
		} else {
			lay_set_contain(c->layout_ctx, layout_id, LAY_COLUMN);
		}
	}
	
	if (strstr(css, "display: block")) {
		lay_set_contain(c->layout_ctx, layout_id, LAY_COLUMN);
	}
	
	if (strstr(css, "display: inline")) {
		/* Keep default behavior for inline elements */
	}
	
	/* Width and Height with units */
	if (strstr(css, "width:")) {
		const char *end_ptr;
		int width = extract_number_with_unit(css, "width:", &end_ptr);
		printf("DEBUG: Parsing width, raw=%d\n", width);
		if (end_ptr && strstr(end_ptr, "px")) {
			/* Use pixel value directly */
			printf("DEBUG: width is in pixels: %d\n", width);
		} else if (end_ptr && strstr(end_ptr, "%")) {
			/* Percentage - calculate based on parent width */
			size2_t vpsize = R_GetViewPortSize();
			width = (int)(vpsize.width * width / 100.0);
			printf("DEBUG: width is percentage, calculated: %d\n", width);
		}
		lay_vec2 current_size = lay_get_size(c->layout_ctx, layout_id);
		printf("DEBUG: Current size before setting width: (%d,%d)\n", current_size[0], current_size[1]);
		lay_set_size_xy(c->layout_ctx, layout_id, width, current_size[1]);
		current_size = lay_get_size(c->layout_ctx, layout_id);
		printf("DEBUG: Size after setting width: (%d,%d)\n", current_size[0], current_size[1]);
	}
	
	if (strstr(css, "height:")) {
		const char *end_ptr;
		int height = extract_number_with_unit(css, "height:", &end_ptr);
		printf("DEBUG: Parsing height, raw=%d\n", height);
		if (end_ptr && strstr(end_ptr, "px")) {
			/* Use pixel value directly */
			printf("DEBUG: height is in pixels: %d\n", height);
		} else if (end_ptr && strstr(end_ptr, "%")) {
			/* Percentage - calculate based on parent height */
			size2_t vpsize = R_GetViewPortSize();
			height = (int)(vpsize.height * height / 100.0);
			printf("DEBUG: height is percentage, calculated: %d\n", height);
		}
		lay_vec2 current_size = lay_get_size(c->layout_ctx, layout_id);
		printf("DEBUG: Current size before setting height: (%d,%d)\n", current_size[0], current_size[1]);
		lay_set_size_xy(c->layout_ctx, layout_id, current_size[0], height);
		current_size = lay_get_size(c->layout_ctx, layout_id);
		printf("DEBUG: Size after setting height: (%d,%d)\n", current_size[0], current_size[1]);
	}
	
	/* Background color support */
	if (strstr(css, "background-color:")) {
		const char *color_start = strstr(css, "background-color:");
		color_start += 16; /* Skip "background-color:" */
		
		/* Skip whitespace */
		while (*color_start && (*color_start == ' ' || *color_start == ':')) {
			color_start++;
		}
		
		/* Extract color value */
		char color_str[32];
		int i = 0;
		while (*color_start && (*color_start != ';' && *color_start != '}') && i < sizeof(color_str) - 1) {
			color_str[i++] = *color_start++;
		}
		color_str[i] = '\0';
		
		/* Parse and apply color */
		COLOR32 bg_color = parse_css_color(color_str);
		printf("DEBUG: Parsed background-color '%s' -> r=%d,g=%d,b=%d,a=%d\n", color_str, bg_color.r, bg_color.g, bg_color.b, bg_color.a);
		
		/* Store color for rendering */
		if (c) {
			userdata *ud = (userdata *)node->_private;
			if (ud) {
				ud->bg_color = bg_color;
				ud->has_bg_color = true;
				printf("DEBUG: Stored bg_color for element\n");
			}
		}
	}
	if (strstr(css, "font-size:")) {
		int font_size = extract_number(css, "font-size:");
		/* Store font size for rendering (this would need to be integrated with the rendering system) */
		/* For now, we'll just store it in the userdata */
		if (c) {
			userdata *ud = (userdata *)node->_private;
			if (ud) {
				/* We could extend userdata to store font size information */
				/* For now, this is just a placeholder */
			}
		}
	}
	
	/* Animation property */
	if (strstr(css, "animation:")) {
		const char *anim_start = strstr(css, "animation:") + 9; /* Skip "animation:" */
		
		printf("DEBUG: Found 'animation:', anim_start='%s'\n", anim_start);
		
		/* Skip whitespace */
		while (*anim_start && (*anim_start == ' ')) {
			anim_start++;
		}
		
		printf("DEBUG: After skipping whitespace, anim_start='%s'\n", anim_start);
		
		/* Extract animation value */
		char anim_value[256];
		int i = 0;
		while (*anim_start && (*anim_start != ';' && *anim_start != '}') && i < sizeof(anim_value) - 1) {
			anim_value[i++] = *anim_start++;
		}
		anim_value[i] = '\0';
		
		printf("DEBUG: Found animation in style: '%s'\n", anim_value);
		
		/* Apply animation */
		apply_animation_attribute(c, node, anim_value);
	}
	
	if (strstr(css, "font-family:")) {
		const char *font_start = strstr(css, "font-family:");
		font_start += 12; /* Skip "font-family:" */
		
		/* Skip whitespace */
		while (*font_start && (*font_start == ' ' || *font_start == ':')) {
			font_start++;
		}
		
		/* Extract font family value */
		char font_str[64];
		int i = 0;
		while (*font_start && (*font_start != ';' && *font_start != '}') && i < sizeof(font_str) - 1) {
			font_str[i++] = *font_start++;
		}
		font_str[i] = '\0';
		
		/* Store font family for rendering (this would need to be integrated with the rendering system) */
		/* For now, we'll just store it in the userdata */
		if (c) {
			userdata *ud = (userdata *)node->_private;
			if (ud) {
				/* We could extend userdata to store font family information */
				/* For now, this is just a placeholder */
			}
		}
	}
}
void print_layout_info(lay_context *layout_ctx, xmlDoc *document, context *c)
{
	xmlNode *root = xmlDocGetRootElement(document);
	print_node_layout(layout_ctx, root, 0, c);
}

void print_node_layout(lay_context *layout_ctx, xmlNode *node, int depth, context *c)
{
	lay_id layout_id;
	if (c != NULL) {
		layout_id = GETLAYID(node);
		if(layout_id==LAY_INVALID_ID){
			return;
		}
	}
	for (int i = 0; i < depth; i++) printf("  ");
	
		lay_scalar x, y, width, height;
		lay_get_rect_xywh(layout_ctx, layout_id, &x, &y, &width, &height);
		
		printf("%s: xy=(%d, %d) w=%d, h=%d [id:%d]\n",
		       node->name ? (char*)node->name : "unknown", (int)x, (int)y, (int)width, (int)height, layout_id);
		
	xmlNode *child = node->children;
	while (child != NULL) {
		print_node_layout(layout_ctx, child, depth + 1, c);
		child = child->next;
	}
}

// 全局变量定义
#define MAX_HTML_PAGES 32
static int g_html_pages_count = 0;
static context *g_html_render_context[MAX_HTML_PAGES];
static int g_html_frame_count = 0;
static bool g_html_render_enabled = true;
static float g_html_last_time = 0.0f;  // 用于计算delta_time

// HTML渲染模式枚举
typedef enum {
    HTML_RENDER_MODE_ANIMATED,  // 动画模式（默认）
    HTML_RENDER_MODE_INTERACTIVE // 交互模式
} html_render_mode_t;

static html_render_mode_t g_html_render_mode = HTML_RENDER_MODE_ANIMATED;

// 渲染矩形边框
void render_rect_border(lay_scalar x, lay_scalar y, lay_scalar width, lay_scalar height, COLOR32 color) {
    if (width <= 0 || height <= 0) return;
    
    // 归一化坐标
    RECT rect = {
        NORM(x), 
        NORM(y),
        NORM(width),
        NORM(height)
    };
    
    R_DrawWireRect(&rect, color);
}

// 渲染填充矩形
void render_rect_fill(lay_scalar x, lay_scalar y, lay_scalar width, lay_scalar height, COLOR32 color) {
    if (width <= 0 || height <= 0) return;
    
    // 归一化坐标
    RECT rect = {
        NORM(x), 
        NORM(y),
        NORM(width),
        NORM(height)
    };
    
    RECT uv = {0, 0, 1, 1};
    DRAWIMAGE drawImg = {
        .texture = tr.texture[TEX_WHITE],
        .screen = rect,
        .uv = uv,
        .color = color,
        .rotate = false,
        .shader = SHADER_UI,
        .model_matrix = NULL
    };
    R_DrawImageEx(&drawImg);
}

#include "css.h"
css_error css_resolve_url(void *pw,
		const char *base, lwc_string *rel, lwc_string **abs)
{
	UNUSED(pw);
	UNUSED(base);

	/* About as useless as possible */
	*abs = lwc_string_ref(rel);

	return CSS_OK;
}
css_error css_handle_resolve_color(void *pw,
		lwc_string *name, css_color *color){
			return CSS_OK;
		}
css_error css_handle_resolve_font(void *pw,
		lwc_string *name, css_system_font *system_font){
			return CSS_OK;
		}		

css_error css_handle_import(void *pw,
		css_stylesheet *parent, lwc_string *url){
return CSS_OK;
		}	
/**
 * @brief Get computed node style using CSS parser
 */
LPCSS html_getnodestyle(context *ctx, xmlNode* node)
{
    userdata* ud = node->_private;
    if (!ud) return NULL;
    
    /* Check if we already computed the style */
    if (ud->computedStyle) {
        return ud->computedStyle;
    }
    
    /* Get context - use global HTML context if ctx is NULL */
    context *css_ctx = ctx ? ctx : g_html_render_context[0];
    if (!css_ctx || !css_ctx->css_select_ctx) {
        /* Fall back to inline style only */
		if(!ud->parsedStyle){
			xmlChar* style_str = xmlGetProp(node, BAD_CAST "style");
			if (!style_str) {
				style_str = xmlGetNsProp(node, BAD_CAST "style", NULL);
			}
			
			if (style_str) {
				const char *element_name = (const char*)node->name;
				if (!element_name) element_name = "div";
				
				css_select_results *results = css_parse_style((const char*)style_str, element_name);
				if (results) {
					ud->parsedStyle = results;
					ud->computedStyle = results;
				}
				
				xmlFree(style_str);
			}
		}
        return ud->computedStyle;
    }
    
    /* Prepare inline style if exists */
    css_stylesheet *inline_style = NULL;
    xmlChar* inline_style_str = xmlGetProp(node, BAD_CAST "style");
    if (!inline_style_str) {
        inline_style_str = xmlGetNsProp(node, BAD_CAST "style", NULL);
    }
    
    if (inline_style_str && strlen((const char*)inline_style_str) > 0) {
        const char *element_name = (const char*)node->name;
        if (!element_name) element_name = "div";
        
        /* Create CSS rule from inline style */
        char css_buffer[2048];
        snprintf(css_buffer, sizeof(css_buffer), "%s { %s }", element_name, (const char*)inline_style_str);
        
        /* Create stylesheet parameters */
        css_stylesheet_params params = {
            .params_version = CSS_STYLESHEET_PARAMS_VERSION_1,
            .level = CSS_LEVEL_21,
            .charset = "UTF-8",
            .url = "inline",
            .title = "inline-style",
            .allow_quirks = false,
            .inline_style = true,
            .resolve = css_resolve_url,
            .resolve_pw = NULL,
            .import = css_handle_import,
            .import_pw = NULL,
            .color = css_handle_resolve_color,
            .color_pw = NULL,
            .font = css_handle_resolve_font,
            .font_pw = NULL
        };
        
        /* Parse inline style */
        css_error code = css_stylesheet_create(&params, &inline_style);
        if (code == CSS_OK) {
            code = css_stylesheet_append_data(inline_style, 
                                        (const uint8_t *)css_buffer, strlen(css_buffer));
            if (code == CSS_OK || code == CSS_NEEDDATA) {
                code = css_stylesheet_data_done(inline_style);
            }
        }
        
        xmlFree(inline_style_str);
    }
    
    /* Use css_select_style to get computed style */
    css_media media = { .type = CSS_MEDIA_SCREEN };
    css_select_results *results = NULL;
    
    css_error code = css_select_style(css_ctx->css_select_ctx, node,
                                    css_get_unit_ctx(), &media, inline_style,
                                    &select_handler, css_ctx, &results);
    
    /* Clean up inline style if created */
    if (inline_style) {
        css_stylesheet_destroy(inline_style);
    }
    
    if (code == CSS_OK && results) {
        /* Store computed style */
        ud->parsedStyle = results;
        ud->computedStyle = results;
        printf("DEBUG: Computed style for element '%s'\n", node->name ? (char*)node->name : "unknown");
        
        /* Apply computed style to lay layout system */
        apply_computed_style_to_lay(css_ctx, node, results);
    } else {
        printf("DEBUG: Failed to compute style for element '%s': %s\n", 
               node->name ? (char*)node->name : "unknown", css_error_to_string(code));
    }
    
    return ud->computedStyle;
}

/**
 * @brief Apply CSS computed style to lay layout system
 * This function converts libcss computed style values to lay item properties
 */
static void apply_computed_style_to_lay(context *ctx, xmlNode *node, const css_select_results *results)
{
    if (!ctx || !node || !results) return;
    
    lay_id layout_id = GETLAYID(node);
    if (layout_id == LAY_INVALID_ID) return;
    
    const css_computed_style *style = results->styles[CSS_PSEUDO_ELEMENT_NONE];
    if (!style) return;
    
    printf("DEBUG: apply_computed_style_to_lay for element '%s'\n", node->name ? (char*)node->name : "unknown");
    
    /* === 1. Handle Display Property === */
    uint8_t display_type = css_computed_display(style, false);
    if (display_type != CSS_DISPLAY_NONE) {
        switch(display_type) {
            case CSS_DISPLAY_FLEX:
            case CSS_DISPLAY_BLOCK:
                /* Default to column flex layout for flex containers */
                lay_set_contain(ctx->layout_ctx, layout_id, LAY_COLUMN);
                break;
            case CSS_DISPLAY_INLINE:
            case CSS_DISPLAY_INLINE_BLOCK:
                /* Inline elements - use default stacking */
                lay_set_contain(ctx->layout_ctx, layout_id, LAY_LAYOUT);
                break;
            default:
                /* Default layout */
                break;
        }
    }
    
    /* === 2. Handle Width and Height === */
    css_fixed width_fixed;
    css_unit width_unit;
    uint8_t width_type = css_computed_width(style, &width_fixed, &width_unit);
    
    if (width_type == CSS_WIDTH_SET && width_unit == CSS_UNIT_PX) {
        int width_px = (int)(width_fixed >> 10); /* Convert from fixed (16.16) to int */
        if (width_px > 0) {
            lay_scalar current_height = lay_get_size(ctx->layout_ctx, layout_id)[1];
            lay_set_size_xy(ctx->layout_ctx, layout_id, width_px, current_height);
            printf("DEBUG:   Set width=%dpx\n", width_px);
        }
    }
    
    css_fixed height_fixed;
    css_unit height_unit;
    uint8_t height_type = css_computed_height(style, &height_fixed, &height_unit);
    
    if (height_type == CSS_HEIGHT_SET && height_unit == CSS_UNIT_PX) {
        int height_px = (int)(height_fixed >> 10); /* Convert from fixed (16.16) to int */
        if (height_px > 0) {
            lay_scalar current_width = lay_get_size(ctx->layout_ctx, layout_id)[0];
            lay_set_size_xy(ctx->layout_ctx, layout_id, current_width, height_px);
            printf("DEBUG:   Set height=%dpx\n", height_px);
        }
    }
    
    /* === 3. Handle Margins === */
    css_fixed margin_top_fixed, margin_right_fixed, margin_bottom_fixed, margin_left_fixed;
    css_unit margin_top_unit, margin_right_unit, margin_bottom_unit, margin_left_unit;
    
    uint8_t margin_top_type = css_computed_margin_top(style, &margin_top_fixed, &margin_top_unit);
    uint8_t margin_right_type = css_computed_margin_right(style, &margin_right_fixed, &margin_right_unit);
    uint8_t margin_bottom_type = css_computed_margin_bottom(style, &margin_bottom_fixed, &margin_bottom_unit);
    uint8_t margin_left_type = css_computed_margin_left(style, &margin_left_fixed, &margin_left_unit);
    
    if (margin_top_type == CSS_MARGIN_SET && margin_right_type == CSS_MARGIN_SET &&
        margin_bottom_type == CSS_MARGIN_SET && margin_left_type == CSS_MARGIN_SET) {
        
        /* Convert fixed point to pixels (assuming all are PX for now) */
        int margin_top_px = (int)(margin_top_fixed >> 10);
        int margin_right_px = (int)(margin_right_fixed >> 10);
        int margin_bottom_px = (int)(margin_bottom_fixed >> 10);
        int margin_left_px = (int)(margin_left_fixed >> 10);
        
        lay_set_margins_ltrb(ctx->layout_ctx, layout_id, margin_top_px, margin_right_px, margin_bottom_px, margin_left_px);
        printf("DEBUG:   Set margins: t=%d, r=%d, b=%d, l=%d\n", 
               margin_top_px, margin_right_px, margin_bottom_px, margin_left_px);
    }
    
    /* === 4. Handle Background Color === */
    css_color bg_color;
    uint8_t bg_color_type = css_computed_background_color(style, &bg_color);
    
    if (bg_color_type == CSS_COLOR_COLOR) {
        /* Convert libcss color to our COLOR32 format */
        COLOR32 color;
        color.r = (uint8_t)((bg_color >> 16) & 0xFF);
        color.g = (uint8_t)((bg_color >> 8) & 0xFF);
        color.b = (uint8_t)(bg_color & 0xFF);
        color.a = (uint8_t)((bg_color >> 24) & 0xFF);
        
        /* Store in userdata for rendering */
        userdata *ud = (userdata *)node->_private;
        if (ud) {
            ud->bg_color = color;
            ud->has_bg_color = true;
            printf("DEBUG:   Set bg_color=(%d,%d,%d,%d)\n", color.r, color.g, color.b, color.a);
        }
    }
    
    /* Note: Padding is not fully supported by lay system
       We could simulate it by adjusting element size and content offset */
}

extern LPFONT g_default_text_font;
void html_render_text(xmlNode* textnode, const char *text, lay_scalar x, lay_scalar y, COLOR32 default_color)
{
    if (!text || strlen(text) == 0) return;
    
    COLOR32 render_color = COLOR32_WHITE;
    FONT* render_font = g_default_text_font;

	assert(render_font);
    int font_size = 16; /* Default font size */
    
    /* Try to get style from parent element */
	xmlNode* style_node = textnode;
	LPCSS style = html_getnodestyle(NULL, style_node);
    while (!style && style_node->parent) {
		style_node = style_node->parent;
		style = html_getnodestyle(NULL, style_node);
	}
    if (style) {
        /* Get color from style */
        const char *color_str = css_get_property_string(style, CSS_PROP_COLOR);
        if (color_str && color_str[0] == '#') {
            /* Parse hex color and convert to COLOR32 */
            color32_t parsed_color = parse_css_color(color_str);
            render_color = (COLOR32){parsed_color.r, parsed_color.g, parsed_color.b, parsed_color.a};
        }
        
        /* Get font properties */
        const char *font_family_css = css_get_property_string(style, CSS_PROP_FONT_FAMILY);
        int new_font_size = css_get_property_int(style, CSS_PROP_FONT_SIZE, font_size);
        
        if (font_family_css) {
            
            /* Get font from cache or load it */
            FONT* custom_font = R_FontCacheGet(font_family_css, new_font_size);
            if (custom_font) {
                render_font = custom_font;
                font_size = new_font_size;
            }
        } else if (new_font_size != font_size) {
            /* Only size changed, use default font with new size */
            FONT* sized_font = R_FontCacheGet(DEFAULT_TEXTFONT_NAME, new_font_size);
            if (sized_font) {
                render_font = sized_font;
                font_size = new_font_size;
            }
        }
    }

	// layout_text_html(render_color,arg.rect,)
    RECT rect = MAKE(RECT, NORM(x), NORM(y),1,1);
    R_DrawUtf8Text2(text,rect,render_color,render_font,NULL);
}

// 渲染图片
void render_image(lay_scalar x, lay_scalar y, lay_scalar width, lay_scalar height, LPCTEXTURE texture) {
    if (!texture || width <= 0 || height <= 0) return;
    
    // 归一化坐标
    RECT rect = {
        NORM(x), 
        NORM(y),
        NORM(width),
        NORM(height)
    };
    
    RECT uv = {0, 0, 1, 1};
    DRAWIMAGE drawImg = {
        .texture = texture,
        .screen = rect,
        .uv = uv,
        .color = COLOR32_WHITE,
        .rotate = false,
        .shader = SHADER_UI,
        .model_matrix = NULL
    };
    R_DrawImageEx(&drawImg);
}
// CSS匹配收集辅助结构
typedef struct {
    xmlNode *node;
    char style[1024];
} css_match_t;

// 独立的DOM遍历收集函数
static void traverse_collect(
    xmlNode *node,
    bool is_tag_selector,
    bool is_id_selector,
    bool is_class_selector,
    const char *selector_value,
    const char *style,
    css_match_t *matches,
    int *match_count,
    int max_matches
);

// 简单的CSS类选择器匹配和应用
static void apply_css_class_selectors(context *ctx, xmlNode *root, const char *css) {
    if (!ctx || !root || !css) return;
    
    printf("DEBUG: CSS Class Selector - Starting to parse CSS...\n");
    
    // 关注点分离：使用结构存储匹配结果
    #define MAX_MATCHES 128
    css_match_t matches[MAX_MATCHES];
    int match_count = 0;
    
    const char *pos = css;
    
// 第一阶段：收集所有选择器（body、.class、#id、tagname）
    while (*pos) {
    
    // 第二阶段：收集所有选择器的匹配（.class、#id、tagname）
    printf("DEBUG: Phase 1 - Collecting matches for . and # selectors\n");
    pos = css;  // 重置到CSS开头
    
    while (*pos) {
        // 跳过空白和@keyframes
        while (*pos && (*pos == ' ' || *pos == '\n' || *pos == '\r' || *pos == '\t')) pos++;
        if (*pos == '@') {
            pos++;
            while (*pos && *pos != '{') pos++;
            if (*pos == '{') {
                pos++;
                int brace_count = 1;
                while (*pos && brace_count > 0) {
                    if (*pos == '{') brace_count++;
                    else if (*pos == '}') brace_count--;
                    pos++;
                }
            }
            continue;
        }
        
        if (!*pos) break;
        
        // 提取选择器和样式
        const char *selector_start = pos;
        while (*pos && *pos != '{') pos++;
        if (*pos != '{') break;
        const char *selector_end = pos;
        
        pos++;
        const char *style_start = pos;
        int brace_count = 1;
        while (*pos && brace_count > 0) {
            if (*pos == '{') brace_count++;
            else if (*pos == '}') brace_count--;
            pos++;
        }
        const char *style_end = pos - 1;
        
        // 复制选择器和样式
        char selector[64] = {0};
        int selector_len = selector_end - selector_start;
        if (selector_len > 0) {
            while (selector_len > 0 && (selector_start[selector_len-1] == ' ' || selector_start[selector_len-1] == '\n')) selector_len--;
            if (selector_len > 0) {
                strncpy(selector, selector_start, selector_len < 63 ? selector_len : 63);
                selector[selector_len < 63 ? selector_len : 63] = '\0';
            }
        }
        
        char style[1024] = {0};
        int style_len = style_end - style_start;
        if (style_len > 0 && style_len < sizeof(style) - 1) {
            strncpy(style, style_start, style_len);
            style[style_len] = '\0';
        }
        
// 判断选择器类型：.class、#id、tagname
        bool is_class_selector = (selector[0] == '.');
        bool is_id_selector = (selector[0] == '#');
        bool is_tag_selector = (!is_class_selector && !is_id_selector);
        
const char *selector_value;
            if (is_tag_selector) {
                selector_value = selector;
            } else {
                selector_value = selector + 1;
            }
            
            // 遍历DOM树，收集匹配的节点
            traverse_collect(
                xmlDocGetRootElement(ctx->document),
                is_tag_selector,
                is_id_selector,
                is_class_selector,
                selector_value,
                style,
                matches,
                &match_count,
                MAX_MATCHES
            );
        }
    }
    
    // 第三阶段：将收集到的样式合并到parsedStyle
    printf("DEBUG: Phase 2 - Merging %d collected styles to parsedStyle\n", match_count);
    
    // 创建一个映射来跟踪每个节点已收集的所有选择器样式
    typedef struct {
        xmlNode *node;
        int style_count;
        char styles[2048];
    } node_styles_t;
    
    node_styles_t node_styles[MAX_MATCHES];
    int node_style_count = 0;
    
    // 第一步：收集所有匹配的样式到对应的节点
    for (int i = 0; i < match_count; i++) {
        printf("DEBUG: Collecting style #%d for element '%s'\n", i, matches[i].node->name ? (char*)matches[i].node->name : "NULL");
        
        userdata *ud = (userdata *)matches[i].node->_private;
        if (!ud) continue;
        
        // 查找是否已经为这个节点创建了收集器
        int idx = -1;
        for (int j = 0; j < node_style_count; j++) {
            if (node_styles[j].node == matches[i].node) {
                idx = j;
                break;
            }
        }
        
        if (idx == -1 && node_style_count < MAX_MATCHES) {
            // 创建新的节点样式收集器
            node_styles[node_style_count].node = matches[i].node;
            node_styles[node_style_count].style_count = 0;
            node_styles[node_style_count].styles[0] = '\0';
            idx = node_style_count;
            node_style_count++;
        }
        
        if (idx >= 0 && matches[i].style[0] != '\0') {
            // 追加选择器样式
            if (node_styles[idx].style_count > 0) {
                strncat(node_styles[idx].styles, "; ", sizeof(node_styles[idx].styles) - 1);
            }
            strncat(node_styles[idx].styles, matches[i].style, sizeof(node_styles[idx].styles) - 1);
            node_styles[idx].style_count++;
        }
    }
    
    // 第二步：为每个节点合并内联样式和选择器样式，然后解析
    for (int i = 0; i < node_style_count; i++) {
        node_styles_t *ns = &node_styles[i];
        userdata *ud = (userdata *)ns->node->_private;
        if (!ud) continue;
        
        // 合并所有样式
        char merged_css[4096] = {0};
        
        // 1. 获取内联样式（只获取一次）
        xmlChar* inline_style = xmlGetProp(ns->node, BAD_CAST "style");
        if (inline_style) {
            strncat(merged_css, (const char*)inline_style, sizeof(merged_css) - 1);
            xmlFree(inline_style);
        }
        
        // 2. 添加所有收集到的选择器样式
        if (ns->styles[0] != '\0') {
            if (merged_css[0] != '\0') {
                strncat(merged_css, "; ", sizeof(merged_css) - strlen(merged_css) - 1);
            }
            strncat(merged_css, ns->styles, sizeof(merged_css) - strlen(merged_css) - 1);
        }
        
        // 3. 使用CSS解析器解析合并的样式
        const char *element_name = (const char*)ns->node->name;
        if (!element_name) element_name = "div";
        
        if (merged_css[0] != '\0') {
            printf("DEBUG: Final merged CSS for '%s': '%s'\n", element_name, merged_css);
            
            // 使用CSS解析器解析样式
            css_select_results *results = css_parse_style(merged_css, element_name);
            if (results) {
                // 释放旧的parsedStyle（如果存在）
                if (ud->parsedStyle) {
                    // TODO: 需要实现释放css_select_results的函数
                    // css_select_results_destroy(ud->parsedStyle);
                    ud->parsedStyle = NULL;
                }
                // 存储新的parsedStyle
                ud->parsedStyle = results;
                printf("DEBUG: Stored parsedStyle for '%s'\n", element_name);
            }
        }
    }
}

// 独立的DOM遍历收集函数实现
static void traverse_collect(
    xmlNode *node,
    bool is_tag_selector,
    bool is_id_selector,
    bool is_class_selector,
    const char *selector_value,
    const char *style,
    css_match_t *matches,
    int *match_count,
    int max_matches
) {
    if (!node) return;
    
    bool match = false;
    if (is_tag_selector && node->type == XML_ELEMENT_NODE) {
        // tagname 选择器：匹配元素名称
        const char *element_name = (char*)node->name;
        if (element_name && strcmp(element_name, selector_value) == 0) {
            match = true;
        }
    } else if (node->type == XML_ELEMENT_NODE) {
        for (xmlAttr *attr = node->properties; attr; attr = attr->next) {
            if (attr->name) {
                const char *attr_name = (char*)attr->name;
                bool match = false;
                
                if (is_class_selector && strcmp(attr_name, "class") == 0) {
                    if (attr->children && attr->children->content) {
                        const char *attr_class = (char*)attr->children->content;
                        if (strstr(attr_class, selector_value) != NULL) {
                            match = true;
                        }
                    }
                } else if (is_id_selector && strcmp(attr_name, "id") == 0) {
                    if (attr->children && attr->children->content) {
                        const char *attr_id = (char*)attr->children->content;
                        if (strcmp(attr_id, selector_value) == 0) {
                            match = true;
                        }
                    }
                }
                
                if (match) {
                    // 存储匹配结果（避免重复）
                    int i;
                    for (i = 0; i < *match_count; i++) {
                        if (matches[i].node == node) {
                            break;
                        }
                    }
                    if (i >= *match_count && *match_count < max_matches) {
                        matches[*match_count].node = node;
                        strncpy(matches[*match_count].style, style, sizeof(matches[*match_count].style) - 1);
                        matches[*match_count].style[sizeof(matches[*match_count].style) - 1] = '\0';
                        (*match_count)++;
                    }
                    break;
                }
            }
        }
    }
    
    // 递归遍历子节点
    for (xmlNode *child = node->children; child; child = child->next) {
        traverse_collect(child, is_tag_selector, is_id_selector, is_class_selector, selector_value, style, matches, match_count, max_matches);
    }
}

void process_style_node(context *ctx, xmlNode *node, int depth) {
	if (!ctx || !node) return;
	
	// 检查节点是否已处理过（使用私有数据标记）
	if (node->_private) {
		userdata *ud = (userdata *)node->_private;
		if (ud->refcount & 0x80000000) {  // 使用最高位标记已处理
			return;  // 已处理过，跳过
		}
	}
	
	// 获取style节点的文本内容
	xmlChar *style_content = xmlNodeGetContent(node);
	if (style_content && ctx->css_select_ctx) {
		const char *css_data = (const char *)style_content;
		
		// Create stylesheet parameters
		css_stylesheet_params params = {
			.params_version = CSS_STYLESHEET_PARAMS_VERSION_1,
			.level = CSS_LEVEL_21,
			.charset = "UTF-8",
			.url = "style-tag",
			.title = "stylesheet",
			.allow_quirks = false,
			.inline_style = false,
			.resolve = css_resolve_url,
			.resolve_pw = NULL,
			.import = css_handle_import,
			.import_pw = NULL,
			.color = css_handle_resolve_color,
			.color_pw = NULL,
			.font = css_handle_resolve_font,
			.font_pw = NULL
		};
		
		// Parse stylesheet
		css_stylesheet *stylesheet = NULL;
		css_error code = css_stylesheet_create(&params, &stylesheet);
		if (code == CSS_OK) {
			// Append CSS data
			size_t data_len = strlen(css_data);
			code = css_stylesheet_append_data(stylesheet, 
											(const uint8_t *)css_data, data_len);
			if (code == CSS_OK || code == CSS_NEEDDATA) {
				code = css_stylesheet_data_done(stylesheet);
				if (code == CSS_OK) {
					// Add stylesheet to select context
					code = css_select_ctx_append_sheet(ctx->css_select_ctx, 
											stylesheet, CSS_ORIGIN_AUTHOR, NULL);
					if (code == CSS_OK) {
						printf("DEBUG: Added stylesheet to CSS select context\n");
						// Store stylesheet for cleanup
						if (ctx->css_stylesheet) {
							css_stylesheet_destroy(ctx->css_stylesheet);
						}
						ctx->css_stylesheet = stylesheet;
						// 标记该节点已处理（使用refcount最高位）
						if (node->_private) {
							userdata *ud = (userdata *)node->_private;
							ud->refcount |= 0x80000000;
						}
					} else {
						fprintf(stderr, "Failed to append stylesheet to select context\n");
						css_stylesheet_destroy(stylesheet);
					}
				} else {
					fprintf(stderr, "Failed to complete stylesheet parsing\n");
					css_stylesheet_destroy(stylesheet);
				}
			} else {
				fprintf(stderr, "Failed to append stylesheet data\n");
				css_stylesheet_destroy(stylesheet);
			}
		} else {
			fprintf(stderr, "Failed to create stylesheet\n");
		}
		
		xmlFree(style_content);
	}
}
void process_script_node(context *ctx, xmlNode *node, int depth) {
	if (!ctx || !node) return;
	
	// 获取script节点的文本内容
	xmlChar *script_content = xmlNodeGetContent(node);
	if (script_content) {
		// 这里可以集成JavaScript引擎来执行脚本
		// 目前仅打印脚本内容作为示例
		printf("Script content:\n%s\n", (const char *)script_content);
		
		xmlFree(script_content);
	}
}

// 渲染HTML元素
void render_html_element(context *ctx, xmlNode *node, int depth) {
    if (!ctx || !node) return;
    
    lay_id layout_id = GETLAYID(node);
    if (layout_id == LAY_INVALID_ID) return;
    
    // 获取元素布局信息
    lay_scalar x, y, width, height;
    lay_get_rect_xywh(ctx->layout_ctx, layout_id, &x, &y, &width, &height);
    
    // 根据元素类型进行不同的渲染
    if (node->type == XML_ELEMENT_NODE) {
        const char *element_name = node->name ? (char*)node->name : "unknown";

        // Check if element has animation and apply animated values
        anim_value_t anim_value;
        bool has_animation = get_element_animation_value(ctx, node, 0, &anim_value);
        float animated_opacity = 1.0f; // Default to full opacity
        if (has_animation) {
            // Apply opacity animation (anim_value.number is used for opacity)
            animated_opacity = anim_value.number;
        }

        // 根据元素类型进行渲染
        // First, draw background if it exists
        userdata *ud = (userdata *)node->_private;
        
        // 获取元素的 id 和 class 属性用于标识
        xmlChar *id_attr = xmlGetProp(node, BAD_CAST "id");
        xmlChar *class_attr = xmlGetProp(node, BAD_CAST "class");
        
        // 构建元素标识字符串
        char elem_id[256];
        elem_id[0] = '\0';
        if (id_attr) {
            snprintf(elem_id, sizeof(elem_id), "#%s", (char*)id_attr);
            xmlFree(id_attr);
        }
        if (class_attr) {
            size_t len = strlen(elem_id);
            snprintf(elem_id + len, sizeof(elem_id) - len, ".%s", (char*)class_attr);
            xmlFree(class_attr);
        }
        
        // 打印带层级、标识和ID的调试信息
        char indent[64];
        indent[0] = '\0';
        for (int i = 0; i < depth && i < 15; i++) {
            strcat(indent, "  ");
        }
        printf("%sDEBUG: Rendering '%s'%s [lay_id:%d] xy=(%d,%d) size=(%dx%d), ud=%p, has_bg_color=%d\n", 
               indent, element_name, elem_id, layout_id, (int)x, (int)y, (int)width, (int)height, 
               ud, ud ? ud->has_bg_color : -1);
        if (ud && ud->has_bg_color) {
            printf("DEBUG: Drawing bg_color=(%d,%d,%d,%d) for '%s'\n", 
                   ud->bg_color.r, ud->bg_color.g, ud->bg_color.b, ud->bg_color.a, element_name);
            render_rect_fill(x, y, width, height, APPLY_ANIMATED_OPACITY(ud->bg_color, animated_opacity));
        }

        // 移除了调试边框，让渲染更美观
        if (strcmp(element_name, "p") == 0) {
            // 段落元素 - 无边框
			html_render_text(node, element_name, x + 5, y + 5, (COLOR32){0, 0, 0, 255});
        } else if (strcmp(element_name, "h1") == 0 || strcmp(element_name, "h2") == 0 ||
                   strcmp(element_name, "h3") == 0 || strcmp(element_name, "h4") == 0 ||
                   strcmp(element_name, "h5") == 0 || strcmp(element_name, "h6") == 0) {
            // 标题元素 - 无边框
			html_render_text(node, element_name, x + 5, y + 5, (COLOR32){0, 0, 0, 255});
        } else if (strcmp(element_name, "img") == 0) {
            // 渲染图片占位符
            COLOR32 img_fill_color = (COLOR32){200, 200, 200, 255};
            COLOR32 img_border_color = (COLOR32){100, 100, 100, 255};
            render_rect_fill(x, y, width, height, APPLY_ANIMATED_OPACITY(img_fill_color, animated_opacity)); // 灰色背景
            render_rect_border(x, y, width, height, APPLY_ANIMATED_OPACITY(img_border_color, animated_opacity)); // 深灰色边框
            html_render_text(node,"[IMAGE]",x + 5, y + 5,  APPLY_ANIMATED_OPACITY(img_border_color, animated_opacity)); // 图片标记
        } else if (strcmp(element_name, "input") == 0 ||
                   strcmp(element_name, "textarea") == 0 ||
                   strcmp(element_name, "select") == 0) {
            // 渲染表单控件
            COLOR32 form_fill_color = (COLOR32){240, 240, 240, 255};
            COLOR32 form_border_color = (COLOR32){128, 128, 128, 255};
            COLOR32 form_text_color = (COLOR32){64, 64, 64, 255};
            render_rect_fill(x, y, width, height, APPLY_ANIMATED_OPACITY(form_fill_color, animated_opacity)); // 浅灰色背景
            render_rect_border(x, y, width, height, APPLY_ANIMATED_OPACITY(form_border_color, animated_opacity)); // 灰色边框
            html_render_text(node,"[FORM]", x + 5, y + 5, APPLY_ANIMATED_OPACITY(form_text_color, animated_opacity)); // 表单标记
        } else if (strcmp(element_name, "table") == 0) {
            // 表格元素 - 无边框
        } else if (strcmp(element_name, "ul") == 0 || strcmp(element_name, "ol") == 0) {
            // 列表元素 - 无边框
        } else if (strcmp(element_name, "style") == 0) {
			process_style_node(ctx, node, depth);
            return;
        } else if (strcmp(element_name, "script") == 0) {
			process_script_node(ctx, node, depth);
            return;
        } else if (strcmp(element_name, "div") == 0) {
            // div元素 - 添加白色边框以便调试
            if (!ud || !ud->has_bg_color) {
                render_rect_border(x, y, width, height, (COLOR32){255, 255, 255, 255});
            } else {
                render_rect_border(x, y, width, height, (COLOR32){200, 200, 200, 255});
            }
        }
        
        // 递归渲染子元素
        xmlNode *child = node->children;
        while (child != NULL) {
            render_html_element(ctx, child, depth + 1);
            child = child->next;
        }
    } else if (node->type == XML_TEXT_NODE) {
        // 文本节点 - 渲染文本
        const char *text_content = node->content ? (char*)node->content : "";
        if (strlen(text_content) > 0) {
            // 检查是否是style标签内的文本（避免渲染CSS内容）
            xmlNode *parent = node->parent;
            if (parent && parent->name && strcmp((char*)parent->name, "style") == 0) {
                return; // 不渲染style标签内的文本
            }
            
            // 清理文本内容（去除前后空白和换行符）
            char *clean_text = strdup(text_content);
            if (clean_text) {
                // 去除前导空白
                char *start = clean_text;
                while (*start && isspace(*start)) start++;
                
                // 去除尾部空白
                char *end = clean_text + strlen(clean_text) - 1;
                while (end > start && isspace(*end)) end--;
                *(end + 1) = '\0';
                
                // 如果文本不为空，则渲染
                if (strlen(start) > 0) {
                    html_render_text(node, start,x + 5, y + 5,  (COLOR32){0, 0, 0, 255}); // 黑色文本
                }
                
                free(clean_text);
            }
        }else{
			printf("DEBUG: Empty text node content\n");
		}
    }
}


// 渲染HTML元素
void html_process_styles_and_scripts(context *ctx, xmlNode *node, int depth) {
    if (!ctx || !node) return;
    
    lay_id layout_id = GETLAYID(node);
    if (layout_id == LAY_INVALID_ID) return;
    
    // 获取元素布局信息
    lay_scalar x, y, width, height;
    lay_get_rect_xywh(ctx->layout_ctx, layout_id, &x, &y, &width, &height);
    
    if (node->type == XML_ELEMENT_NODE) {
        const char *element_name = node->name ? (char*)node->name : "unknown";

        // Check if element has animation and apply animated values
        anim_value_t anim_value;
        bool has_animation = get_element_animation_value(ctx, node, 0, &anim_value);
        float animated_opacity = 1.0f; // Default to full opacity
        if (has_animation) {
            // Apply opacity animation (anim_value.number is used for opacity)
            animated_opacity = anim_value.number;
        }

        // 根据元素类型进行渲染
        // First, draw background if it exists
        userdata *ud = (userdata *)node->_private;

        if (strcmp(element_name, "style") == 0) {
			process_style_node(ctx, node, depth);
            return;
        } else if (strcmp(element_name, "script") == 0) {
			process_script_node(ctx, node, depth);
            return;
        }
        // 递归渲染子元素
        xmlNode *child = node->children;
        while (child != NULL) {
            html_process_styles_and_scripts(ctx, child, depth + 1);
            child = child->next;
        }
    }
}

// 初始化HTML渲染

int html_init(LPCSTR filename)
{
	error_code error;
	context *c;
	hubbub_parser_optparams params;
	FILE *input;
	uint8_t *buf;
	size_t len;

	/* Read input file into memory. If we wanted to, we could read into
	 * a fixed-size buffer and pass each chunk to the parser sequentially.
	 */
	input = fopen(filename, "r");
	if (input == NULL) {
		fprintf(stderr, "Failed opening %s\n", filename);
		return 1;
	}

	fseek(input, 0, SEEK_END);
	len = ftell(input);
	fseek(input, 0, SEEK_SET);

	buf = malloc(len);
	if (buf == NULL) {
		fclose(input);
		fprintf(stderr, "No memory for buf\n");
		return 1;
	}

	fread(buf, 1, len, input);

	/* Create our parsing context */
	error = create_context(NULL, &c);
	if (error != OK) {
		free(buf);
		fclose(input);
		fprintf(stderr, "Failed creating parsing context\n");
		return 1;
	}

	/* Attempt to parse the document */
	error = parse_chunk(c, buf, len);
	assert(error == OK || error == ENCODINGCHANGE);
	if (error == ENCODINGCHANGE) {
		/* During parsing, we detected that the charset of the 
		 * input data was different from what was auto-detected
		 * (see the change_encoding callback for more details).
		 * Therefore, we must destroy the current parser and create
		 * a new one using the newly-detected charset. Then we
		 * reparse the data using the new parser. 
		 *
		 * change_encoding() will have put the new charset into
		 * c->encoding.
		 */
		context *c2;

		error = create_context(c->encoding, &c2);
		if (error != OK) {
			destroy_context(c2);
			free(buf);
			fclose(input);
			fprintf(stderr, "Failed recreating context\n");
			return 1;
		}

		destroy_context(c);

		c = c2;

		/* Retry the parse */
		error = parse_chunk(c, buf, len);
	}

	if (error != OK) {
		destroy_context(c);
		free(buf);
		fclose(input);
		fprintf(stderr, "Failed parsing document\n");
		return 1;
	}


	/* Tell hubbub that we've finished */
	error = parse_completed(c);
	if (error != OK) {
		destroy_context(c);
		free(buf);
		fclose(input);
		fprintf(stderr, "Failed parsing document\n");
		return 1;
	}

	/* We're done with this */
	free(buf);

	/* At this point, the DOM tree can be accessed through c->document */
	/* Let's dump it to stdout */
	/* In a real application, we'd probably want to grab the document
	 * from the parsing context, then destroy the context as it's no
	 * longer of any use */
	
	/* Run layout calculations after parsing is complete */
	printf("Running layout calculation...\n");
	printf("Layout context count: %d\n", lay_items_count(c->layout_ctx));
	

	/* Find the html element's layout ID and run layout from there */
	// lay_run_item(c->layout_ctx, GETLAYID(c->document)); 

	lay_run_context(c->layout_ctx);
	
	/* Print layout information */
	// printf("=== Layout Information ===\n");
	// print_layout_info(c->layout_ctx, c->document, c);
	// printf("=========================\n");


    g_html_render_context[g_html_pages_count] = c;
	g_html_pages_count++;
    g_html_frame_count = 0;

    /* Scan and store @keyframes rules */
    html_scan_and_store_keyframes();

    /* Reapply animations after keyframes are loaded */
    html_reapply_all_animations();

	fclose(input);

	return 0;
}

int html_destroy(){
	for (int i = 0; i < g_html_pages_count; i++) {
		context *ctx = g_html_render_context[i];
		if (ctx) {
			destroy_context(ctx);
			g_html_render_context[i] = NULL;
		}
	}
	g_html_pages_count = 0;
	return 0;
}

/**
 * 递归处理XML节点的keyframes扫描
 */
static void scan_node_for_keyframes(context *ctx, xmlNode *node) {
	if (!node) return;

	// 查找style标签
	if (node->type == XML_ELEMENT_NODE) {
		const char *name = (const char *)node->name;
		if (strcasecmp(name, "style") == 0) {
			// 提取style内容
			xmlChar *content = xmlNodeGetContent(node);
			if (content) {
				const char *css_content = (const char *)content;
				printf("Found <style> tag, scanning for @keyframes...\n");

				// 扫描@keyframes规则
				const char *keyframes_pos = strstr(css_content, "@keyframes");
				const char *block_end = NULL; // 提前声明，以便在循环外使用
				while (keyframes_pos && ctx->num_keyframes < MAX_KEYFRAMES) {
					// 查找@keyframes后的名称
					const char *name_start = keyframes_pos + 10; // 跳过"@keyframes"
					while (*name_start && isspace(*name_start)) name_start++;

					// 查找名称结束（在{之前）
					const char *name_end = strchr(name_start, '{');
					if (!name_end) break;

					// 提取动画名称
					int name_len = name_end - name_start;
					if (name_len > 0 && name_len < 64) {
						strncpy(ctx->keyframes_store[ctx->num_keyframes].name,
							   name_start, name_len);
						ctx->keyframes_store[ctx->num_keyframes].name[name_len] = '\0';

						// 提取完整的@keyframes内容
						const char *block_start = name_end;
						int brace_count = 0;
						block_end = block_start;

						for (; *block_end; block_end++) {
							if (*block_end == '{') brace_count++;
							else if (*block_end == '}') {
								brace_count--;
								if (brace_count == 0) {
									block_end++; // 包含最后的}
									break;
								}
							}
						}

						// 创建完整的keyframes字符串
						int block_len = block_end - keyframes_pos;
						char *keyframes_text = malloc(block_len + 1);
						if (keyframes_text) {
							strncpy(keyframes_text, keyframes_pos, block_len);
							keyframes_text[block_len] = '\0';

							// 使用css_parse_keyframes解析
							int count = css_parse_keyframes(
								keyframes_text,
								ctx->keyframes_store[ctx->num_keyframes].name,
								ctx->keyframes_store[ctx->num_keyframes].keyframes,
								32
							);

							if (count > 0) {
								ctx->keyframes_store[ctx->num_keyframes].keyframe_count = count;
								printf("  -> Stored keyframes: '%s' with %d keyframes\n",
								   ctx->keyframes_store[ctx->num_keyframes].name, count);
								ctx->num_keyframes++;
							}

							free(keyframes_text);
						}
					}

					// 继续查找下一个@keyframes
					if (block_end) {
						keyframes_pos = strstr(block_end, "@keyframes");
					} else {
						break;
					}
				}

				xmlFree(content);
			}
		}
	}

	// 递归处理子节点
	if (node->children) {
		xmlNode *child = node->children;
		while (child) {
			scan_node_for_keyframes(ctx, child);
			child = child->next;
		}
	}
}

/**
 * 扫描并存储@keyframes规则
 * 遍历DOM树查找<style>标签，提取@keyframes并存储到context中
 */
void html_scan_and_store_keyframes(void) {
	FOR_LOOP(i, g_html_pages_count) {
		context *ctx = g_html_render_context[i];
		if (!ctx || !ctx->document) {
			continue;
		}
		ctx->num_keyframes = 0;
		memset(ctx->keyframes_store, 0, sizeof(ctx->keyframes_store));

		printf("Scanning for @keyframes in page %d...\n", i);
		xmlNode *root = xmlDocGetRootElement(ctx->document);
		if (root) {
			scan_node_for_keyframes(ctx, root);
		}
		printf("Total keyframes stored in page %d: %d\n", i, ctx->num_keyframes);
	}
}

/**
 * @brief 重新应用节点上的动画属性
 * 在keyframes加载完成后调用此函数
 */
static void reapply_node_animations(context *ctx, xmlNode *node) {
	if (!node || node->type != XML_ELEMENT_NODE) {
		return;
	}

	// 检查 animation 属性
	xmlAttr *attr = node->properties;
	while (attr) {
		const char *attr_name = (const char *)attr->name;
		if (attr_name && strcmp(attr_name, "animation") == 0) {
			const char *attr_value = (const char *)xmlNodeGetContent(attr->children);
			if (attr_value) {
				printf("Reapplying animation attribute: '%s'\n", attr_value);
				apply_animation_attribute(ctx, node, attr_value);
			}
		}
		attr = attr->next;
	}

	// 检查 style 属性中的 animation
	attr = node->properties;
	while (attr) {
		const char *attr_name = (const char *)attr->name;
		if (attr_name && strcmp(attr_name, "style") == 0) {
			const char *attr_value = (const char *)xmlNodeGetContent(attr->children);
			if (attr_value && strstr(attr_value, "animation:")) {
				printf("Reapplying animation from style: '%s'\n", attr_value);
				/* Note: Inline style is now handled by html_getnodestyle() via css_select_style
				   Animation will be extracted and applied there too */
				// apply_css_to_layout(ctx, node, attr_value); // REMOVED - handled by libcss
			}
		}
		attr = attr->next;
	}

	// 递归处理子节点
	if (node->children) {
		xmlNode *child = node->children;
		while (child) {
			reapply_node_animations(ctx, child);
			child = child->next;
		}
	}
}

/**
 * @brief 在加载keyframes后重新应用所有动画
 */
void html_reapply_all_animations(void) {
	FOR_LOOP(i, g_html_pages_count) {
		context *ctx = g_html_render_context[i];
		if (!ctx || !ctx->document) {
			continue;
		}
		printf("Reapplying all animations for page %d after keyframes loaded...\n", i);
		xmlNode *root = xmlDocGetRootElement(ctx->document);
		if (root) {
			reapply_node_animations(ctx,root);
		}
	}
}

// 渲染HTML文档背景
void draw_html_background(context *ctx) {
    if (!ctx || !ctx->document) return;
    
    // 获取窗口大小
    size2_t vpsize = R_GetViewPortSize();
    
    // 尝试从body元素获取背景色
    COLOR32 bg_color = (COLOR32){255, 255, 255, 255}; 
    
    xmlNode *root = xmlDocGetRootElement(ctx->document);
    if (root) {
        xmlNode *body = root->children;
        while (body) {
            if (body->type == XML_ELEMENT_NODE && strcmp((char*)body->name, "body") == 0) {
                userdata *ud = (userdata *)body->_private;
                if (ud && ud->has_bg_color) {
                    bg_color = ud->bg_color;
                }
                break;
            }
            body = body->next;
        }
    }
    
    // 渲染背景
    render_rect_fill(0, 0, 400, 600, bg_color);
    printf("Drawing HTML background: %dx%d, color=(%d,%d,%d,%d)\n", 
           vpsize.width, vpsize.height, bg_color.r, bg_color.g, bg_color.b, bg_color.a);
}


// 主要的HTML渲染帧函数
void html_update_and_layout(float delta_time,int page_index) {

	context *ctx = g_html_render_context[page_index];
	if (!ctx || !ctx->document) {
		return;
	}
	
	xmlNode *root = xmlDocGetRootElement(ctx->document);
    if (root) {
        html_process_styles_and_scripts(ctx, root, 0);
	}

    // 0. 更新动画管理器（如果提供了delta_time）
    if (delta_time > 0.0f && ctx->anim_mgr) {
        anim_manager_update(ctx->anim_mgr, delta_time);
    }
    
    // 2. 重新计算布局（总是需要）
    lay_run_context(ctx->layout_ctx);


	/* Print layout information */
	printf("=== Layout Information ===\n");
	print_layout_info(ctx->layout_ctx, ctx->document, ctx);
	// printf("=========================\n");
    
}


void html_update(float delta_time) {
	FOR_LOOP(i, g_html_pages_count) {
		html_update_and_layout(delta_time,i);
	}
}

void html_render(){
	FOR_LOOP(i, g_html_pages_count){
		context *ctx = g_html_render_context[i];
		 xmlNode *root = xmlDocGetRootElement(ctx->document);
        if (root) {
		    // draw_html_background(ctx);
            render_html_element(ctx, root, 0);
        }
	}
}

// 清理HTML渲染
void html_render_cleanup() {
    printf("Cleaning up HTML Render...\n");
	FOR_LOOP(i, g_html_pages_count) {
		context *ctx = g_html_render_context[i];
		if (ctx->document) {
			xmlFreeDoc(ctx->document);
			ctx->document = NULL;
		}
		if (ctx->layout_ctx) {
			lay_destroy_context(ctx->layout_ctx);
			free(ctx->layout_ctx);
			ctx->layout_ctx = NULL;
		}
		if (ctx->anim_mgr) {
			anim_manager_cleanup(ctx->anim_mgr);
			free(ctx->anim_mgr);
			ctx->anim_mgr = NULL;
		}
	}
    g_html_frame_count = 0;
    g_html_render_enabled = false;
}

// 设置HTML渲染模式
void html_render_set_mode(html_render_mode_t mode) {
    g_html_render_mode = mode;
    printf("HTML Render mode set to: %s\n",
           mode == HTML_RENDER_MODE_ANIMATED ? "ANIMATED" : "INTERACTIVE");
}

// 启用/禁用HTML渲染
void html_render_set_enabled(bool enabled) {
    g_html_render_enabled = enabled;
    printf("HTML rendering %s\n", enabled ? "enabled" : "disabled");
}
