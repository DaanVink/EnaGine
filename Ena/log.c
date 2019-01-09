#include <string.h>
#include <stdio.h>
#include "globals.h"

void printlog(char* value[], int mode) {
    if (SETTINGS_DEBUG_LEVEL >= mode) {
        FILE *fp = fopen("log.txt", "a");
        printf(value);
        fputs(value, fp);
        fclose(fp);
        fflush(stdout);
    }
}
