#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "log.h"

int IOGetSize(char* path[]) {
    FILE *fp = fopen(path, "r");
    printf("1");
    fseek(fp, 0L, SEEK_END) == 0;
    printf("2");
    int size = ftell(fp);
    printf("3");
    fclose(fp);
    printf("4\n");
    return size;
}

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

int IOCheckValid(char* path) {
    if ( access( path, F_OK ) != -1 ) {
        return 1;
    }
    else {
        return 0;
    }
}

void IOReadText(int* returnaddr, char* IOStatus[], char* filename[], int filesize) {
    FILE *fp = fopen ( filename , "r" );
    long lSize;
    char *buffer = *returnaddr;
    int status = 0;
    if (fp != NULL) {
        if (fseek(fp, 0L, SEEK_SET) != 0) { status = 1; }

        /* Read the entire file into memory. */
        size_t newLen = fread(buffer, sizeof(char), filesize, fp);
        if ( ferror( fp ) != 0 ) {
            status = 2;
        } else {
            buffer[newLen] = '\0';
        }
        fclose(fp);
    }
    else {
        status = 3;
    }
    switch (status) {
        case 0:
            printlog("[fileIO.c:IOReadText] OK\n", 2);
            strcpy(IOStatus, "200");
            break;
        case 1:
            printlog("[fileIO.c:IOReadText] Error seeking file start\n", 2);
            strcpy(IOStatus, "500");
            break;
        case 2:
            printlog("[fileIO.c:IOReadText] Error copying to buffer\n", 2);
            strcpy(IOStatus, "500");
            break;
        case 3:
            printlog("[fileIO.c:IOReadText] Error finding file\n", 2);
            strcpy(IOStatus, "404");
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
