#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"

typedef struct {
    char *ext;
    char *type;
} EXT_FINDER;

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

void IOReadText(char* returnaddr[], int status, char* filename[]) {
    FILE *fp;
    char *buffer;
    long lSize;

    fp = fopen ( filename , "r" );
    if( !fp ) perror(filename), fclose(fp);

    fseek( fp , 0L , SEEK_END);
    lSize = ftell( fp );
    rewind( fp );

    /* allocate memory for entire content */
    buffer = calloc( 1, lSize+1 );
    if( !buffer ) fclose(fp), status = 500;

    /* copy the file into the buffer */
    if( 1!=fread( buffer , lSize, 1 , fp) ) fclose(fp), free(buffer), status = 500;
    fclose(fp);
    strcpy(returnaddr, buffer);
}

void IOReadBinary(char* returnaddr[], int status, char* filename[]) {

    unsigned char buffer[243];
    FILE *ptr;

    ptr = fopen(filename,"rb");  // r for read, b for binary

    fread(buffer,sizeof(buffer),1,ptr); // read 10 bytes to our buffer
    memcpy(returnaddr, buffer, sizeof(buffer));
    fclose(ptr);
}
