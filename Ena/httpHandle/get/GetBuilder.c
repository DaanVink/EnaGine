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
#include "../../tools.h"
#include "../request.h"

void HandleGet (SOCKET sock, char* fileRequest[], REQUEST request) {

    printlog("[handleGet.c:HandleGet] Succcesfully received request\n", 0);

    char response[SETTINGS_FILE_BUFFER_SIZE];
    char errorPage[SETTINGS_FILE_BUFFER_SIZE];
    char folderPath[SETTINGS_URL_BUFFER_SIZE];

    char URLTempType[SETTINGS_URL_BUFFER_SIZE];
    char URLTempPath[SETTINGS_URL_BUFFER_SIZE];
    char httpType[64];
    char fileType[64];

    char tempLen[8];

    char *token;
    char *buffer;
    long lSize;

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
    printlog(request.HTTPTYPE, 2);
    printlog("\n", 2);

    char temptype[4];
    strncpy(temptype, request.HTTPTYPE, 4);
    temptype[4] = '\0';
    if (strcmp(temptype, "text") == 0) {
        request.ISBINARY = 0;
    }
    else {
        request.ISBINARY = 1;
    }

    request.ISFOLDER = IOCheckFolder(request.FILEPATH);
    printlog("[handleGet.c:HandleGet] request.ISFOLDER: ", 2);
    if (request.ISFOLDER == 1) { printlog("true\n", 2); } 
    else { printlog("false\n", 2); }

    if (!request.ISFOLDER) {
        if (IOCheckValid(request.FILEPATH)) {
            request.STATUS = 200;
        }
        else {
            request.STATUS = 404;
        }
    }
    if (request.ISFOLDER) {
        strcpy(request.FOLDERPATH, request.FILEPATH);
        strcat(request.FILEPATH, SETTINGS_CONTENT_DEFAULT_FILE);
        printlog("[handleGet.c:HandleGet] Generating index page\n", 1);
        char indexPage[SETTINGS_FILE_BUFFER_SIZE];
        buildIndexPage(indexPage, request.FOLDERPATH, request.REQUESTQUERY);
        request.STATUS = 200;
        buildResponse(response, "200 Ok", strlen(indexPage), "text/html", indexPage);
        indexPage[0] = '\0';
    }

    if (request.STATUS == 200) {
        printf("Lapped 200\n");
        if (!request.ISBINARY) {
            buildResponse(response, "200 Ok", strlen(buffer), request.HTTPTYPE, buffer);
        }
        else {
            buildResponse(response, "200 Ok", 255, request.HTTPTYPE, "");
        }
    }
    else if (request.STATUS == 404) {
        printlog("[handleGet.c:HandleGet] Entering 404 handler\n", 1);
        if (!request.ISFOLDER) {
            char IOStatus[5] = "0";
            IOReadText(errorPage, IOStatus, SETTINGS_ERROR_HANDLING_404);
            if (strcmp(IOStatus, "404") == 0) {
                printlog("[handleGet.c:HandleGet] Error in reading 404 page, falling back to hardcoded\n", 0);
                buildResponse(response, "404 Not Found", strlen(page_404), "text/html", page_404);
            }
            else if (strcmp(IOStatus, "500") == 0) {
                buildResponse(response, "500 Not Found", strlen(errorPage), "text/html", page_500);
            }
            else {
                buildResponse(response, "404 Not Found", strlen(errorPage), "text/html", errorPage);
            }
        }
        else if (request.ISFOLDER) {
            printlog("[handleGet.c:HandleGet] Generating index page\n", 1);
            char indexPage[SETTINGS_FILE_BUFFER_SIZE];
            buildIndexPage(indexPage, folderPath, fileRequest);
            request.STATUS = 200;
            buildResponse(response, "200 Ok", strlen(indexPage), "text/html", indexPage);
            indexPage[0] = '\0';
        }
    }
    if (request.STATUS == 500) {
        printlog("[handleGet.c:HandleGet] Entering 500 handler\n", 1);
        if (request.ISFOLDER == 0) {
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
        else if (request.ISFOLDER == 1) {
            printlog("[handleGet.c:HandleGet] Generating index page\n", 1);
            char indexPage[SETTINGS_FILE_BUFFER_SIZE];
            buildIndexPage(indexPage, folderPath, fileRequest);
            request.STATUS = 200;
            buildResponse(response, "200 Ok", strlen(indexPage), "text/html", indexPage);
            indexPage[0] = '\0';
        }
    }

    printlog(response, 2);
    printlog("\n", 2);
    send(sock, response, sizeof(response), 0);
    close(sock);
    
    printlog("[handleGet.c:HandleGet] Returning to invoker", 1);
}
