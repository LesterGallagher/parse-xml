
#ifndef __XML_PARSER_HELPERS_H__
#define __XML_PARSER_HELPERS_H__

#include "xml-parser.h"

#define INDENTATION_SPACES 4

void print_xml_tree(xml_node *n);

void free_xml(xml_node *n);

xml_node *parse_xml_file(char *filename);

#endif
