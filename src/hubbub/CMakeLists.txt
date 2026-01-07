# Hubbub HTML Parser Library
cmake_minimum_required(VERSION 3.10)
project(hubbub C)

# Static library
add_library(hubbub STATIC
    # Main parser
    parser.c

    # Charset detection
    charset/detect.c

    # Tokeniser
    tokeniser/entities.c
    tokeniser/tokeniser.c

    # Treebuilder
    treebuilder/treebuilder.c
    treebuilder/initial.c
    treebuilder/before_html.c
    treebuilder/before_head.c
    treebuilder/in_head.c
    treebuilder/in_head_noscript.c
    treebuilder/after_head.c
    treebuilder/in_body.c
    treebuilder/in_table.c
    treebuilder/in_caption.c
    treebuilder/in_column_group.c
    treebuilder/in_table_body.c
    treebuilder/in_row.c
    treebuilder/in_cell.c
    treebuilder/in_select.c
    treebuilder/in_select_in_table.c
    treebuilder/in_foreign_content.c
    treebuilder/after_body.c
    treebuilder/in_frameset.c
    treebuilder/after_frameset.c
    treebuilder/after_after_body.c
    treebuilder/after_after_frameset.c
    treebuilder/generic_rcdata.c
    treebuilder/element-type.c

    # Utils
    utils/errors.c
    utils/string.c
)

# Include directories
target_include_directories(hubbub PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

# Link dependencies (parserutils is required)
find_library(PARSERUTILS_LIB parserutils HINTS ${CUSTOM_LIB_DIR}/lib ${CMAKE_PREFIX_PATH}/lib)
if(PARSERUTILS_LIB)
    target_link_libraries(hubbub ${PARSERUTILS_LIB})
    target_include_directories(hubbub PRIVATE ${CUSTOM_LIB_DIR}/include ${CMAKE_PREFIX_PATH}/include)
else()
    message(FATAL_ERROR "parserutils library not found. Please set CUSTOM_LIB_DIR or install libparserutils.")
endif()

# Compiler warnings
if(MSVC)
    target_compile_options(hubbub PRIVATE /W4)
else()
    target_compile_options(hubbub PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Position independent code (required for static library that will be linked into shared objects)
set_target_properties(hubbub PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)
