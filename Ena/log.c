#include <string.h>
#include <stdio.h>

void printlog(char* value[]) {
    FILE *fp = fopen("log.txt", "a");
    printf(value);
    fputs(value, fp);
    fclose(fp);
}
