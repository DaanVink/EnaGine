#include <winsock2.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <setjmp.h>

#include "../../globals.h"

void HandleGet (int connected, SOCKET sock, char fileRequest[]) {

    int status = 200;

    static jmp_buf jumpBuffer;

    char response[9999];
    char fileBuffer[9999];
    char filePath[1024];
    DIR *dir;
    FILE *fp;
    long lSize;
    char* buffer;
    char* errorPage;
    struct dirent *ent;
    char filesInDirectory;
    strcpy(filePath, "");
    strncat(filePath, SETTINGS_CONTENT_ROOT_PATH, strlen(SETTINGS_CONTENT_ROOT_PATH) - 1);
    strcat(filePath, fileRequest);
    printf(filePath);

    if( access( filePath, F_OK ) != -1 ) {
        fp = fopen ( filePath , "rb" );
        if( !fp ) perror(filePath), fclose(fp);

        fseek( fp , 0L , SEEK_END);
        lSize = ftell( fp );
        rewind( fp );

        /* allocate memory for entire content */
        buffer = calloc( 1, lSize+1 );
        if( !buffer ) fclose(fp), status = 500;

        /* copy the file into the buffer */
        if( 1!=fread( buffer , lSize, 1 , fp) ) fclose(fp), free(buffer), status = 500;
        fclose(fp);
    }
    else {
        status = 404;
    }

    int pos_search = 0;
    int pos_text = 0;
    int len_search = 4;
    int fileRequestOffset = 0;
    int len_text = sizeof(fileRequest)/sizeof(fileRequest[0]);

    if (status == 200) {
        strcpy(response, "HTTP/1.1 200 Ok\n"
                         "Content-length: 47\n"
                         "Content-Type: text/html; charset=ISO-8859-4\n"
                         "\n");
        strcat(response, buffer);
        strcat(response, "\0");
        free(buffer);
    }
    if (status == 404) {
        fp = fopen ( SETTINGS_ERROR_HANDLING_404 , "rb" );
        printf(SETTINGS_ERROR_HANDLING_404);
        printf("\nTEST\n");
        if( !fp ) perror(SETTINGS_ERROR_HANDLING_404);

        fseek( fp , 0L , SEEK_END);
        lSize = ftell( fp );
        rewind( fp );

        /* allocate memory for entire content */
        errorPage = calloc( 1, lSize+1 );
        if( !errorPage ) fclose(fp), status = 500;

        /* copy the file into the buffer */
        if( 1!=fread( errorPage , lSize, 1 , fp) ) fclose(fp), free(errorPage), status = 500;
        fclose(fp);
        strcpy(response, "HTTP/1.1 404 Not Found\n"
                         "Content-length: 47\n"
                         "Content-Type: text/html; charset=ISO-8859-4\n"
                         "\n");
        strcat(response, errorPage);
        strcat(response, "\0");
        free(errorPage);
    }
    if (status == 500) {
        strcpy(response, "HTTP/1.1 500 Internal Server Error\n"
                         "Content-length: 47\n"
                         "Content-Type: text/html; charset=ISO-8859-4\n"
                         "\n"
                         "500");
        strcat(response, "\0");
    }

    send(connected,response,sizeof(response),0);
}
