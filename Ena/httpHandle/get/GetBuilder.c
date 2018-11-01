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
#include "../request.h"

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
    int isText = 0;
    int canRead = 0;
    int contentSize = 0;

    char response[SETTINGS_FILE_BUFFER_SIZE];
    char errorPage[SETTINGS_FILE_BUFFER_SIZE];
    char folderPath[SETTINGS_URL_BUFFER_SIZE];

    REQUEST request;

    char URLTempType[SETTINGS_URL_BUFFER_SIZE];
    char URLTempPath[SETTINGS_URL_BUFFER_SIZE];
    char httpType[64];
    char fileType[64];

    char tempLen[8];

    char *token;
    char *buffer;
    long lSize;

    int requestIsFolder = 0;

    printlog("[handleGet.c:HandleGet] Finished setting up for request\n", 1);

    strcpy(request.REQUESTQUERY, fileRequest);

    strcpy(request.FILEPATH, "");
    strncat(request.FILEPATH, SETTINGS_CONTENT_ROOT_PATH, strlen(SETTINGS_CONTENT_ROOT_PATH) - 1);
    request.FILEPATH[strlen(request.FILEPATH)] = '\0';

    strcpy(URLTempPath, strtok(request.REQUESTQUERY, "?"));
    strcpy(request.REQUESTDATA, &request.REQUESTQUERY[strlen(request.REQUESTQUERY) + 1]);
    strcat(request.FILEPATH, URLTempPath);

    printlog("[handleGet.c:HandleGet] Relative path is:", 2);
    printlog(request.REQUESTQUERY, 2);
    printlog("\n", 2);

    printlog("[handleGet.c:HandleGet] Path is:", 2);
    printlog(request.FILEPATH, 2);
    printlog("\n", 2);

    printlog("[handleGet.c:HandleGet] URL data is: ", 1);
    printlog(request.REQUESTDATA, 1);
    printlog("\n", 1);

    token = strtok(URLTempPath, ".");
    while (token != NULL) {
        strcpy(URLTempType, token);
        token = strtok(NULL, ".");
    }
    strcpy(request.FILETYPE, URLTempType);
    typeResolve(request.HTTPTYPE, request.FILETYPE);

    printlog("[handleGet.c:HandleGet] Found filetype: ", 2);
    printlog(httpType, 2);
    printlog("\n", 2);

    char temptype[4];
    strncpy(temptype, httpType, 4);
    temptype[4] = '\0';
    if (strcmp(temptype, "text") == 0) {
        isText = 1;
    }

    requestIsFolder = IOCheckFolder(request.FILEPATH);
    printlog("[handleGet.c:HandleGet] requestIsFolder: ", 2);
    if (requestIsFolder == 1) { printlog("true\n", 2); } 
    else { printlog("false\n", 2); }

    if (!requestIsFolder) {
        if (IOCheckValid(request.FILEPATH)) {
            status = 200;
        }
        else {
            status = 404;
        }
    }
    printf("%i", status);
    strcpy(response, "test123");
    if (requestIsFolder) {
        strcpy(request.FOLDERPATH, request.FILEPATH);
        strcat(request.FILEPATH, SETTINGS_CONTENT_DEFAULT_FILE);
        printlog("[handleGet.c:HandleGet] Generating index page\n", 1);
        char indexPage[SETTINGS_FILE_BUFFER_SIZE];
        buildIndexPage(indexPage, request.FOLDERPATH, request.REQUESTQUERY);
        status = 200;
        buildResponse(response, "200 Ok", strlen(indexPage), "text/html", indexPage);
        indexPage[0] = '\0';
    }

    if (status == 200) {
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
        if (!requestIsFolder) {
            char IOStatus[5] = "0";
            IOReadText(errorPage, IOStatus, SETTINGS_ERROR_HANDLING_404);
            if (strcmp(IOStatus, "404") == 0) {
                printlog("[handleGet.c:HandleGet] Error in reading 404 page, falling back to hardcoded\n", 0);
                buildResponse(response, "404 Not Found", strlen(page_404), "text/html", page_404);
            }
            else if (strcmp(IOStatus, "500") == 0) {
                status = 500;
            }
            else {
                buildResponse(response, "404 Not Found", strlen(errorPage), "text/html", errorPage);
            }
        }
        else if (requestIsFolder) {
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
            IOReadText(response, IOStatus, SETTINGS_ERROR_HANDLING_500);
            if (strcmp(IOStatus, "500") == 0) {
                printlog("[handleGet.c:HandleGet] Error in reading 500 page, falling back to hardcoded\n", 0);
                buildResponse(response, "500 Internal Server Error", strlen(page_500), "text/html", page_500);
            }
            else {
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
    close(sock);
}
