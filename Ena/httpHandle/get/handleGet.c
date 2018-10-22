#include <winsock2.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>

#include "../../globals.h"
#include "specialPages/index.h"
#include "../../httpTypeResolver.h"
#include "../../fileIO.h"
#include "../../log.h"

#define BUFSIZE 255

void buildResponse(char* returnaddr[], char* status[], int length, char* type[], char* data[]) {

    char lengthBuffer[128];
    strcpy(returnaddr, "HTTP/1.1 ");
    strcat(returnaddr, status);
    strcat(returnaddr, "\nContent-length: ");
    sprintf(lengthBuffer, "%d", length);
    strcat(returnaddr, lengthBuffer);
    strcat(returnaddr, "\nContent-Type: ");
    strcat(returnaddr, type);
    strcat(returnaddr, "\n\n");
    strcat(returnaddr, data);
    returnaddr[strlen(returnaddr) - 1] = '\0';

}

void HandleGet (SOCKET sock, char* fileRequest[], long tempLong) {

    printlog("[handleGet.c:HandleGet] Succcesfully received request\n");

    int status = 200;
    int isText = 1;
    int canRead = 0;

    char response[SETTINGS_FILE_BUFFER_SIZE];
    char fileBuffer[SETTINGS_FILE_BUFFER_SIZE];
    char URLData[SETTINGS_URL_DATA_BUFFER_SIZE];
    char URLPath[SETTINGS_URL_BUFFER_SIZE];
    char URLTempPath[SETTINGS_URL_BUFFER_SIZE];
    char folderPath[SETTINGS_URL_BUFFER_SIZE];

    char URLTempType[SETTINGS_URL_BUFFER_SIZE];
    char httpType[128];
    char fileType[128];

    char tempLen[8];

    FILE *fp;
    char* buffer;
    char* errorPage;
    char *token;
    long lSize;

    int requestIsFolder = 0;

    printlog("[handleGet.c:HandleGet] Finished setting up for request\n");

    printlog("[handleGet.c:HandleGet] fileRequest is: \n");
    printlog(fileRequest);
    printlog("\n");

    strcpy(URLPath, "");
    strncat(URLPath, SETTINGS_CONTENT_ROOT_PATH, strlen(SETTINGS_CONTENT_ROOT_PATH) - 1);

    printlog(fileRequest);

    printlog("[handleGet.c:HandleGet] Path is:");
    printlog(fileRequest);
    printlog("\n");
    
    strcpy(URLTempPath, strtok(fileRequest, "?"));
    strcpy(URLData, &fileRequest[strlen(URLPath) + 1]);
    strcat(URLPath, URLTempPath);

    printlog("[handleGet.c:HandleGet] URL data is: ");
    printlog(URLData);
    printlog("\n");

    token = strtok(URLTempPath, ".");
    while (token != NULL) {
        strcpy(URLTempType, token);
        token = strtok(NULL, ".");
    }
    strcpy(fileType, URLTempType);
    typeResolve(httpType, fileType);

    printlog("[handleGet.c:HandleGet] Path to requested data is: ");
    printlog(URLPath);
    printlog("\n");
    printlog("[handleGet.c:HandleGet] Found filetype ");
    printlog(httpType);
    printlog("\n");
    char temptype[4];
    strncpy(temptype, httpType, 4);
    if (strcmp(temptype, "text") != 0) {
        isText = 0;
    }

    requestIsFolder = IOCheckFolder(URLPath);
    printlog("[handleGet.c:HandleGet] requestIsFolder: ");
    printf("%i", requestIsFolder);
    printlog("\n");
    if (requestIsFolder) {
        strcpy(folderPath, URLPath);
        strcat(URLPath, SETTINGS_CONTENT_DEFAULT_FILE);
    }

    if( access( URLPath, F_OK ) != -1 ) {
        canRead = 1;
        if (isText) {
            IOReadText(fileBuffer, status, URLPath);
            buffer = fileBuffer;
        }
    }

    else {
        status = 404;
    }

    if (status == 200) {
        //printf("Lapped 200\n");
        if (isText) {
            buildResponse(response, "200 Ok", strlen(buffer), httpType, buffer);
        }
        else {
            buildResponse(response, "200 Ok", 255, httpType, "");
        }
    }
    if (status == 404) {
        if (requestIsFolder == 0) {
            //printf("Lapped 404\n");
            fp = fopen ( SETTINGS_ERROR_HANDLING_404 , "r" );
            printlog(SETTINGS_ERROR_HANDLING_404);
            printlog("\nTEST\n");
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

            buildResponse(response, "404 Not Found", strlen(errorPage), "text/html", errorPage);
        }
        else if (requestIsFolder == 1) {
            char indexPage[SETTINGS_FILE_BUFFER_SIZE];
            buildIndexPage(indexPage, folderPath, fileRequest);
            status = 200;
            buildResponse(response, "200 Ok", strlen(indexPage), "text/html", indexPage);
            indexPage[0] = '\0';
        }
    }
    if (status == 500) {
        //printf("Lapped 500\n");
        fp = fopen ( SETTINGS_ERROR_HANDLING_500 , "r" );
        printlog(SETTINGS_ERROR_HANDLING_500);
        printlog("\nTEST\n");
        if( !fp ) perror(SETTINGS_ERROR_HANDLING_500);

        fseek( fp , 0L , SEEK_END);
        lSize = ftell( fp );
        rewind( fp );

        /* allocate memory for entire content */
        errorPage = calloc( 1, lSize+1 );
        if( !errorPage ) fclose(fp), status = 500;

        /* copy the file into the buffer */
        if( 1!=fread( errorPage , lSize, 1 , fp) ) fclose(fp), free(errorPage), status = 500;
        fclose(fp);

        buildResponse(response, "500 Internal Server Error", strlen(errorPage), "text/html", errorPage);
    }

    printlog(response);
    printlog("\n");
    send(sock, response, sizeof(response), 0);
    close(sock);
}
