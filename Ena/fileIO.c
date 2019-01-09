#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "log.h"

int IOGetSize(char* path[]) {
    FILE *fp = fopen(path, "r");
    fseek(fp, 0L, SEEK_END) == 0;
    int size = ftell(fp);
    fclose(fp);
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

int IOReadText(char* returnaddr[], char* filename[], int *requestsize) {
    FILE *fp = fopen ( filename , "r" );
    long lSize;
    int status = 0;

    fseek(fp, 0L, SEEK_END) == 0;
    int filesize = ftell(fp);
    if (filesize == 0) {status = 1;}
    rewind(fp);

    *requestsize = filesize;

    if (fp != NULL && status == 0) {
        /* Read the entire file into memory. */
        size_t newSize = fread(returnaddr, 1, filesize, fp);
        if ( ferror( fp ) != 0 ) {
            status = 2;
        }
        returnaddr[newSize - 1] = '\0';
        fclose(fp);
    }
    else {
        status = 3;
    }
    switch (status) {
        case 0:
            printlog("[fileIO.c:IOReadText] OK\n", 2);
            return 200;
            break;
        case 1:
            printlog("[fileIO.c:IOReadText] Error seeking file start\n", 2);
            return 500;
        case 2:
            printlog("[fileIO.c:IOReadText] Error copying to buffer\n", 2);
            return 500;
        case 3:
            printlog("[fileIO.c:IOReadText] Error finding file\n", 2);
            return 500;
        default:
            printlog("[fileIO.c:IOReadText] An unknown status code was returned\n", 2);
            return 500;
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
