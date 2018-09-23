#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "resolver.h"
#include "globals.h"

void resolverInit() {

    struct node *root;
    root = malloc(sizeof(struct node));
    struct node *conductor;
    root->next = 0;
    conductor = root;
    GLOBAL_ROOT = root;
    FILE* file;

    file = fopen("EnaData/specialTypes.txt", "r");

    char *currentLine = malloc(64);
    while (fgets(currentLine, 64, file) != NULL)  {
        char type[64];
        strcpy(type, currentLine);
        char resolved[64];

        strcpy(type, strtok(type, "|"));
        strcpy(resolved, &currentLine[strlen(type)]+1);

        strcpy(conductor->type, type);
        strcpy(conductor->resolved, resolved);

        conductor->next = malloc(sizeof(struct node));
        conductor = conductor->next;
    }
    conductor->next = 0;
    conductor = GLOBAL_ROOT;
}

void typeResolve(char* returnaddr[], char* input[]) {

    struct node *cond;
    int found;
    found = 1;
    cond = GLOBAL_ROOT;
    while (strcmp(cond->type, input) != 0) {
        if (cond->next == 0) {
            found = 0;
            break;
        }
        else {
            cond = cond->next;
        }
    }
    if (found == 1) {
        strcpy(returnaddr, cond->resolved);
    }
    else {
        return;
    }

}
