#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"

typedef struct {
    char *ext;
    char *type;
} EXT_FINDER;

void typeResolve(char* returnaddr[], char* input[]) {

    EXT_FINDER WEB_EXTENSIONS[] ={
    {"gif", "image/gif" },
    {"txt", "text/plain" },
    {"jpg", "image/jpg" },
    {"jpeg","image/jpeg"},
    {"png", "image/png" },
    {"ico", "image/ico" },
    {"htm", "text/html" },
    {"html","text/html" },
    {"php", "text/html" },
    {"pdf","application/pdf"},
    {"zip","application/octet-stream"},
    {"rar","application/octet-stream"},
    {"gz",  "application/octet-stream"  },
    {"tar", "application/octet-stream" },
    {0,0} };

    int index = 0;
    strcpy(returnaddr, "text/plain");
    while (1) {
        if (WEB_EXTENSIONS[index].ext == 0) { 
            strcpy(returnaddr, "text/html");
            break;
        }
        else if(strcmp(WEB_EXTENSIONS[index].ext, input) == 0) {
            strcpy(returnaddr, WEB_EXTENSIONS[index].type);
            break;
        }
        else {
            index++;
        }
    }

}
