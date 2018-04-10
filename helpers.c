
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "helpers.h"

#include "CMemLeak.h"

void print_xml_tree_rec(xml_node *n, unsigned short scope) {
    if (scope > 0) printf("%*c", scope * INDENTATION_SPACES, ' ');
    printf("%s\n", n->name);
    if (n->type == NODE) {
        for(int i = 0; i < n->children_len; i++) {
            print_xml_tree_rec(n->children[i].node, scope+1);
        }
    } else if (n->type == PCDATA) {
        printf("%*c", (scope + 1) * INDENTATION_SPACES, ' ');
        printf("%s\n", n->children[0].pcdata);
    } else if (n->type == ATTR) {
        printf("%*c", (scope + 1) * INDENTATION_SPACES, ' ');
        printf("%s\n", n->children[0].attr);
    }
}

void print_xml_tree(xml_node *n) {
    print_xml_tree_rec(n, 0);
}

void free_xml(xml_node *n) {
    if (n->type == NODE) {
        free(n->name);
        for(int i = 0; i < n->children_len; i++) {
            xml_node* t = n->children[i].node;
            free_xml(t);
        }
        if (n->children_len > 0) free(n->children);
    } else if (n->type == PCDATA) {
        // n->name ("#text") is static data so it soes not have to be freed
        free(n->children[0].pcdata);
        free(n->children);
    } else if (n->type == ATTR) {
        free(n->name);
        free(n->children[0].attr);
        free(n->children);
    }
    free(n);
}

xml_node *parse_xml_file(char *filename) {
    char *str = NULL;
    long length;
    FILE *f = fopen(filename, "rb");
    if (f == NULL) return NULL;
    else {
        if (fseek(f, 0, SEEK_END) != 0) return NULL;
        length = ftell(f);
        if (fseek(f, 0, SEEK_SET)) return NULL;
        str = malloc(length + sizeof(char));
        if (str) fread(str, 1, length, f);
        else return NULL;
        str[length] = '\0';
        long str_len = strlen(str);
        if (str_len != length) return NULL;
        printf("%s", str);
        if (fclose(f)) return NULL;
    }
    xml_node *root = parse_xml(str);
    free(str);
    return root;
}
