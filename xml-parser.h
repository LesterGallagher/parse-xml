
#ifndef __XML_PARSER_H__
#define __XML_PARSER_H__

typedef enum xml_type { NODE, PCDATA, ATTR } xml_type;

union xml_node_val;

typedef struct xml_node {
    unsigned short children_len;
    union xml_node_val *children;
    struct xml_node *parent;
    xml_type type;
    char *name;
} xml_node;

typedef union xml_node_val {
    xml_node *node;
    char *pcdata;
    char *attr;
} xml_node_val;

xml_node *parse_xml(char* xml);

#endif
