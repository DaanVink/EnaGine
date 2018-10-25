#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "log.h"

int IOCheckFolder(char* path[]) {
    DIR* dir = opendir(path);
    if (dir) {
        closedir(dir);
        return 1;
    }
    else {
        closedir(dir);
        return 0;
    }
}

void IOReadText(char* returnaddr[], char* IOStatus[], char* filename[]) {
    FILE *fp = fopen ( filename , "r" );
    char *buffer = NULL;
    long lSize;
    int status = 0;
    if (fp != NULL) {
        /* Go to the end of the file. */
        if (fseek(fp, 0L, SEEK_END) == 0) {
            /* Get the size of the file. */
            long bufsize = ftell(fp);
            if (bufsize == -1) { status = 1; }

            /* Allocate our buffer to that size. */
            buffer = malloc(sizeof(char) * (bufsize + 1));

            /* Go back to the start of the file. */
            if (fseek(fp, 0L, SEEK_SET) != 0) { status = 2; }

            /* Read the entire file into memory. */
            size_t newLen = fread(buffer, sizeof(char), bufsize, fp);
            if ( ferror( fp ) != 0 ) {
                status = 3;
            } else {
                buffer[newLen] = '\0';
            }
        } 
        else {
           status = 5;
        }
        fclose(fp);
    }
    else {
        status = 6;
    }
    strcpy(returnaddr, buffer);
    switch (status) {
        case 0:
            printlog("[fileIO.c:IOReadText] OK\n", 2);
            strcpy(IOStatus, "200");
            break;
        case 1:
            printlog("[fileIO.c:IOReadText] Error telling filesize\n", 2);
            strcpy(IOStatus, "500");
            break;
        case 2:
            printlog("[fileIO.c:IOReadText] Error seeking file start\n", 2);
            strcpy(IOStatus, "500");
            break;
        case 3:
            printlog("[fileIO.c:IOReadText] Error copying to buffer\n", 2);
            strcpy(IOStatus, "500");
            break;
        case 4:
            printlog("[fileIO.c:IOReadText] Error allocating buffer in memory\n", 2);
            strcpy(IOStatus, "500");
            break;
        case 5:
            printlog("[fileIO.c:IOReadText] Unable to find file", 2);
            strcpy(IOStatus, "404");
            break;
        case 6:
            printlog("[fileIO.c:IOReadText] OK", 2);
            strcpy(IOStatus, "200");
            break;
    }
}

void IOReadBinary(char* returnaddr[], int status, char* filename[]) {

    unsigned char buffer[243];
    FILE *ptr;

    ptr = fopen(filename,"rb");  // r for read, b for binary

    fread(buffer,sizeof(buffer),1,ptr); // read 10 bytes to our buffer
    memcpy(returnaddr, buffer, sizeof(buffer));
    fclose(ptr);
}
