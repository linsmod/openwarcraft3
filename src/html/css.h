#pragma  once

#include "common/shared.h"
#include <libcss/libcss.h>
#include <libcss/select.h>
KNOWN_AS(css_select_results, CSS);
css_select_results* css_parse_style(const char *style_str, const char *element_name);
const char* css_get_property_string(css_select_results *results, uint32_t property);
int css_get_property_int(css_select_results *results, uint32_t property, int default_value);

/* Get CSS unit context for use in CSS selection */
css_unit_ctx* css_get_unit_ctx(void);

/* External CSS selection handler for use in CSS style selection */
extern css_select_handler select_handler;