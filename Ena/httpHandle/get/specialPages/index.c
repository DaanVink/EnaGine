#include <dirent.h>
#include <stdio.h>
#include <string.h>

#include "../../../globals.h"

void buildIndexPage(char inputPage[], char location[], char requestLocation[]) {

    struct dirent *ent;
    char page[SETTINGS_FILE_BUFFER_SIZE];
    strcpy(page, "<h1> Index of ");
    strcat(page, requestLocation);
    strcat(page, "</h1><ul>");

    DIR* dir = opendir(location);
    if (dir != NULL) {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL) {
            if (strcmp(ent->d_name, ".") == 0) {
                continue;
            }
            strcat(page, "<li><a href='");
            strcat(page, ent->d_name);
            strcat(page, "'>");
            strcat(page, ent->d_name);
            strcat(page, "</a></li>");
        }
        closedir (dir);
    }
    else {
        /* could not open directory */
        perror ("");
    }
    strcpy(inputPage, page);
    inputPage[strlen(inputPage)] = '\0';

}
