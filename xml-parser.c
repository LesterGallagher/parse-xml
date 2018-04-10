
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "xml-parser.h"

#include "CMemLeak.h"

// TODO: CDATA Comments

#define INITIAL_ITEM_LEN 2 

void push_node(xml_node* parent, xml_node* child) {
    if (parent->children_len >= INITIAL_ITEM_LEN
        && parent->children_len && !(parent->children_len & (parent->children_len - 1))) { // power of 2
        parent->children = realloc(parent->children, sizeof(xml_node_val) * parent->children_len * 2);
    }
    parent->children[parent->children_len++].node = child;
}

char iscontent(char* buf, unsigned short start, unsigned short end) {
    if (end <= start) return 0;
    while(start < end) if (!isspace(buf[start++])) return 1;
    return 0;
}

char *substring(const char *str, size_t begin, size_t len) {
    char *subbuff = malloc(len + 1);
    memcpy(subbuff, &str[begin], len);
    subbuff[len] = '\0';
    return subbuff;
}

unsigned short str_slice_eq(const char *str, size_t begin, size_t len, const char *target) {
    char* sub = substring(str, begin, len);
    int r = strcmp(sub, target);
    free(sub);
    return r == 0;
}

xml_node *parse_xml(char* xml) {
    xml_node document;
    xml_node *curr = &document;
    curr->children = malloc(sizeof(xml_node_val));
    curr->children_len = 0;
    curr->parent = NULL;
    xml_node *child = NULL;
    int start, len = strlen(xml);
    for(int i = 0; i < len; i++) {
        switch(xml[i]) {
            case '<': 
            if (xml[i+1] == '/') {
                // trim excess
                curr->children = realloc(curr->children, sizeof(xml_node_val) * curr->children_len);
                curr = curr->parent;
                while(xml[++i] != '>') {}
                break;
            }
            // start = i+1;
            // while(xml[i+1] != '>') {i++;}
            child = malloc(sizeof(xml_node));
            
            while(isspace(xml[i])) {i++;} // skip spaces
            start = i+1;
            while(xml[i] != '>' && !isspace(xml[i])) {i++;} // attr name
            child->name = substring(xml, start, i-start);
            
            child->type = NODE;
            child->parent = curr;
            child->children_len = 0;
            if (xml[i] == '/') { // self closing
                child->children = NULL;
            } else {
                child->children = malloc(sizeof(xml_node_val) * INITIAL_ITEM_LEN);
                while(xml[i] != '>' && xml[i] != '/') {
                    i++;
                    while(isspace(xml[i])) {i++;} // skip spaces
                    start = i;
                    while(!isspace(xml[i]) && xml[i] != '=' && xml[i] != '>') {i++;} // attr name
                    if (i == start) continue;
                    xml_node *attr = malloc(sizeof(xml_node));
                    attr->name = substring(xml, start, i - start);
                    attr->type = ATTR;
                    attr->parent = child;
                    if (xml[i] == '=' && xml[i+1] == '"') {
                        i++;
                        start = i+1;
                        while(!isspace(xml[++i]) && xml[i] != '"') {}
                        attr->children_len = 1;
                        attr->children = malloc(sizeof(xml_node_val));
                        attr->children[0].attr = substring(xml, start, i-start);
                        i++;
                    } else {
                        attr->children_len = 0;
                        attr->children = NULL;
                    }
                    push_node(child, attr);
                }
            }
            push_node(curr, child);
            if (xml[i] != '/') curr = child; // not self closing
            else i++;
            break;
            default: //text
            start = i;
            while((i+1) < len && xml[i+1] != '<') {i++;}
            if (!iscontent(xml, start, i)) break;
            child = malloc(sizeof(xml_node));
            child->name = "#text";
            child->type = PCDATA;
            child->parent = curr;
            child->children_len = 1;
            child->children = malloc(sizeof(xml_node_val));
            child->children[0].pcdata = substring(xml, start, i-start+1);
            push_node(curr, child);
            break;
        }
    }
    child = curr->children[0].node;
    free(curr->children);
    return child;
}


