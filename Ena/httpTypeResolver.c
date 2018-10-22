#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"

void typeResolve(char* returnaddr[], char* input[]) {

    EXT_FINDER WEB_EXTENSIONS[] ={
    {"gif", "image/gif" },
    {"txt", "text/plain" },
    {"jpg", "image/jpg" },
    {"jpeg","image/jpeg"},
    {"png", "image/png" },
    {"ico", "image/ico" },
    {"zip", "image/zip" },
    {"gz",  "image/gz"  },
    {"tar", "image/tar" },
    {"htm", "text/html" },
    {"html","text/html" },
    {"php", "text/html" },
    {"pdf","application/pdf"},
    {"zip","application/octet-stream"},
    {"rar","application/octet-stream"},
    {0,0} };

    int found = 0;
    int index = 0;
    strcpy(returnaddr, "text/plain");
    while (found == 0) {
        if(WEB_EXTENSIONS[index].ext == "0") {
            found = 1;
            strcpy(returnaddr, "text/html");
        }
        if(strcmp(WEB_EXTENSIONS[index].ext, input) == 0) {
            found = 1;
            strcpy(returnaddr, "text/test");
        }
        else {
            found++;
        }
    }

}
