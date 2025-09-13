#pragma  once

#include "common/shared.h"
#include <libcss/libcss.h>
#include <libcss/select.h>
KNOWN_AS(css_select_results, CSS);
css_select_results* css_parse_inline_style(const char *style_str, const char *element_name);
const char* css_get_property_string(css_select_results *results, uint32_t property);
int css_get_property_int(css_select_results *results, uint32_t property, int default_value);