#include <winsock2.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>

#include "../../globals.h"
#include "specialPages/index.h"
#include "specialPages/defaultError.h"
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

    printlog("[handleGet.c:HandleGet] Succcesfully received request\n", 0);

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

    printlog("[handleGet.c:HandleGet] Finished setting up for request\n", 1);

    printlog("[handleGet.c:HandleGet] fileRequest is: ", 1);
    printlog(fileRequest, 1);
    printlog("\n", 1);

    strcpy(URLPath, "");
    strncat(URLPath, SETTINGS_CONTENT_ROOT_PATH, strlen(SETTINGS_CONTENT_ROOT_PATH) - 1);

    printlog("[handleGet.c:HandleGet] Path is:", 2);
    printlog(fileRequest, 2);
    printlog("\n", 2);
    
    strcpy(URLTempPath, strtok(fileRequest, "?"));
    strcpy(URLData, &fileRequest[strlen(URLPath) + 1]);
    strcat(URLPath, URLTempPath);

    printlog("[handleGet.c:HandleGet] URL data is: ", 1);
    printlog(URLData, 1);
    printlog("\n", 1);

    token = strtok(URLTempPath, ".");
    while (token != NULL) {
        strcpy(URLTempType, token);
        token = strtok(NULL, ".");
    }
    strcpy(fileType, URLTempType);
    typeResolve(httpType, fileType);

    printlog("[handleGet.c:HandleGet] Path to requested data is: ", 2);
    printlog(URLPath, 2);
    printlog("\n", 2);
    printlog("[handleGet.c:HandleGet] Found filetype ", 2);
    printlog(httpType, 2);
    printlog("\n", 2);
    char temptype[4];
    strncpy(temptype, httpType, 4);
    if (strcmp(temptype, "text") != 0) {
        isText = 0;
    }

    requestIsFolder = IOCheckFolder(URLPath);
    printlog("[handleGet.c:HandleGet] requestIsFolder: ", 2);
    if (requestIsFolder == 1) { printlog("true\n", 2); } 
    else { printlog("false\n", 2); }

    if (!requestIsFolder) {
        if ( access( URLPath, F_OK ) != -1 ) {
            canRead = 1;
            if (isText) {
                printf("ISTEXT");
                status = 200;
                char IOStatus[5] = "0";
                IOReadText(fileBuffer, IOStatus, URLPath);
                printf(IOStatus);
                status = atoi(IOStatus);
                buffer = fileBuffer;
            }
        }
        else {
            status = 404;
        }
    }
    printf("%i", status);

    if (requestIsFolder) {
        strcpy(folderPath, URLPath);
        strcat(URLPath, SETTINGS_CONTENT_DEFAULT_FILE);
        printlog("[handleGet.c:HandleGet] Generating index page\n", 1);
        char indexPage[SETTINGS_FILE_BUFFER_SIZE];
        buildIndexPage(indexPage, folderPath, fileRequest);
        status = 200;
        buildResponse(response, "200 Ok", strlen(indexPage), "text/html", indexPage);
        indexPage[0] = '\0';
    }

    else if (status == 200) {
        printf("Lapped 200\n");
        if (isText) {
            buildResponse(response, "200 Ok", strlen(buffer), httpType, buffer);
        }
        else {
            buildResponse(response, "200 Ok", 255, httpType, "");
        }
    }
    else if (status == 404) {
        printlog("[handleGet.c:HandleGet] Entering 404 handler\n", 1);
        if (requestIsFolder == 0) {
            char IOStatus[5] = "0";
            printlog("[handleGet.c:HandleGet] Generating 404 page\n", 1);
            IOReadText(response, IOStatus, SETTINGS_ERROR_HANDLING_404);
            if (strcmp(IOStatus, "500") == 0) {
                printlog("[handleGet.c:HandleGet] Error in reading 404 page, falling back to hardcoded\n", 0);
                buildResponse(response, "404 Not Found", strlen(page_404), "text/html", page_404);
            }
            else {
                fclose(fp);
                buildResponse(response, "404 Not Found", strlen(errorPage), "text/html", errorPage);
            }
        }
        else if (requestIsFolder == 1) {
            printlog("[handleGet.c:HandleGet] Generating index page\n", 1);
            char indexPage[SETTINGS_FILE_BUFFER_SIZE];
            buildIndexPage(indexPage, folderPath, fileRequest);
            status = 200;
            buildResponse(response, "200 Ok", strlen(indexPage), "text/html", indexPage);
            indexPage[0] = '\0';
        }
    }
    if (status == 500) {
        printlog("[handleGet.c:HandleGet] Entering 500 handler\n", 1);
        if (requestIsFolder == 0) {
            char IOStatus[5] = "0";
            printlog("[handleGet.c:HandleGet] Generating 500 page\n", 1);
            IOReadText(response, IOStatus, SETTINGS_ERROR_HANDLING_404);
            if (strcmp(IOStatus, "500") == 0) {
                printlog("[handleGet.c:HandleGet] Error in reading 500 page, falling back to hardcoded\n", 0);
                buildResponse(response, "500 Internal Server Error", strlen(page_404), "text/html", page_404);
            }
            else {
                fclose(fp);
                buildResponse(response, "500 Internal Server Error", strlen(errorPage), "text/html", errorPage);
            }
        }
        else if (requestIsFolder == 1) {
            printlog("[handleGet.c:HandleGet] Generating index page\n", 1);
            char indexPage[SETTINGS_FILE_BUFFER_SIZE];
            buildIndexPage(indexPage, folderPath, fileRequest);
            status = 200;
            buildResponse(response, "200 Ok", strlen(indexPage), "text/html", indexPage);
            indexPage[0] = '\0';
        }
    }

    printlog(response, 2);
    printlog("\n", 2);
    send(sock, response, sizeof(response), 0);
}
