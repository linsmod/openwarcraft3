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
#include "libxml/tree.h"
#include "r_local.h"
#include <stdint.h>
#include <strings.h>
#include <unicode/ucnv.h>
#define _GNU_SOURCE /* for strndup */
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include <libxml/HTMLparser.h>
#include <libxml/HTMLtree.h>
#include <libxml/debugXML.h>

#include <hubbub/parser.h>
#include <hubbub/tree.h>
#define LAY_IMPLEMENTATION
#include "layout.h"

#define UNUSED(x) ((x)=(x))

#define GETLAYID(node) ((userdata*)node->_private)->layid

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

	int inhead;
} context;

typedef struct userdata{
	uint refcount;
	lay_id layid;
	LPSTR __f;
	int __ln;
} userdata;

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

/* Layout helper functions */
static int extract_number(const char *css, const char *property);
void apply_css_to_layout(context *c, xmlNode *node, const char *css);
void print_layout_info(lay_context *layout_ctx, xmlDoc *document, context *c);
static void print_node_layout(lay_context *layout_ctx, xmlNode *node, int depth, context *c);

int html_main(LPCSTR filename)
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
	printf("=== Layout Information ===\n");
	print_layout_info(c->layout_ctx, c->document, c);
	printf("=========================\n");
	
	/* Clean up */
	destroy_context(c);

	fclose(input);

	return 0;
}

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
	/* Reference count of zero, allocate userdata for document */
	userdata *doc_ud = malloc(sizeof(userdata));
	if (doc_ud == NULL) {
		hubbub_parser_destroy(c->parser);
		free(c);
		return NOMEM;
	}
	doc_ud->refcount = 0;

	// layout context 
	c->layout_ctx = malloc(sizeof(lay_context));

	/* Initialize layout context */
	lay_init_context(c->layout_ctx);
	lay_reset_context(c->layout_ctx); /* Clear any existing data */
	lay_reserve_items_capacity(c->layout_ctx, 1000); /* Pre-allocate */

	size2_t const window = R_GetWindowSize();
	
	doc_ud->layid = lay_item(c->layout_ctx);

	lay_set_size(c->layout_ctx, doc_ud->layid, (lay_vec2){window.width,window.height});

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
	/* We use the _private field of libXML's xmlNode struct for the
	 * userdata structure containing reference count and layout ID. */
	userdata *ud = malloc(sizeof(userdata));
	if (ud == NULL) {
		free(content);
		return HUBBUB_NOMEM;
	}
	ud->refcount = 1;
	ud->__f = __FILE__;
	ud->__ln= __LINE__;
	ud->layid = lay_item(c->layout_ctx);
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
	userdata *ud = malloc(sizeof(userdata));
	if (ud == NULL) {
		free(system);
		free(public);
		free(name);
		return HUBBUB_NOMEM;
	}
	ud->refcount = 1;
	ud->__f = __FILE__;
	ud->__ln= __LINE__;
	ud->layid = lay_item(c->layout_ctx);
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
	userdata *ud = malloc(sizeof(userdata));
	if (ud == NULL) {
		free(name);
		return HUBBUB_NOMEM;
	}
	ud->refcount = 1;
	ud->__f = __FILE__;
	ud->__ln= __LINE__;
	ud->layid = lay_item(c->layout_ctx);
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
	userdata *ud = malloc(sizeof(userdata));
	if (ud == NULL) {
		return HUBBUB_NOMEM;
	}
	ud->refcount = 1;
	ud->__f = __FILE__;
	ud->__ln= __LINE__;
	ud->layid = lay_item(c->layout_ctx);
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
				userdata *new_private = MemAlloc(sizeof(userdata));
				if (new_private != NULL) {
					// 只复制必要的字段，不复制 layid，因为需要重新分配
					new_private->refcount = 1;
					new_private->__f = orig_private->__f;
					new_private->__ln = orig_private->__ln;
					new_private->layid = orig_private->layid;
					chld->_private = new_private;
				}
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
				userdata *new_private = MemAlloc(sizeof(userdata));
				if (new_private != NULL) {
					new_private->refcount = 1;
					new_private->__f = orig_private->__f;
					new_private->__ln = orig_private->__ln;
					new_private->layid = orig_private->layid;
					chld->_private = new_private;
				}
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
                apply_css_to_layout(c, n, value);
                free(value);
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

void apply_css_to_layout(context *c, xmlNode *node, const char *css)
{
	lay_id layout_id = GETLAYID(node);
	
	if (layout_id == LAY_INVALID_ID) return;
	
	/* Parse simple CSS (here we need to implement CSS parser, the following is pseudo code) */
	if (strstr(css, "display: flex")) {
		if (strstr(css, "flex-direction: row")) {
			lay_set_contain(c->layout_ctx, layout_id, LAY_ROW);
		} else {
			lay_set_contain(c->layout_ctx, layout_id, LAY_COLUMN);
		}
	}
	
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
	
	if (strstr(css, "margin:")) {
		int margin = extract_number(css, "margin:");
		lay_set_margins_ltrb(c->layout_ctx, layout_id,
		                    margin, margin, margin, margin);
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

void html_render_frame(){

}
