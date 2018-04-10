#include <stdlib.h>
#include <stdio.h>

#include "config.h"
#include "xml-parser.h"
#include "helpers.h"

#include "CMemLeak.h"

char t (int i) {
    return i && !(i & (i - 1));
}

int main(int argc, char* argv[]) {
    for(int i = 0; i < argc; i++) {
        printf("%s\n", argv[i]);
    }
    xml_node *xml = parse_xml_file("example.xml");
    print_xml_tree(xml);
    free_xml(xml);

    #ifdef _DEBUG
    XWBReport("Done");
    #endif

    getchar();
}







