#include <winsock2.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>

#include "../../globals.h"
#include "specialPages/index.h"
#include "specialPages/defaultError.h"
#include "../../MIMETYPEResolver.h"
#include "../../fileIO.h"
#include "../../log.h"
#include "../../tools.h"
#include "../request.h"

void HandleGet (SOCKET sock, char* fileRequest[], REQUEST request) {

    printlog("[GetBuilder.c:HandleGet] Succcesfully received request\n", 0);

    char response[SETTINGS_FILE_BUFFER_SIZE];
    char errorPage[SETTINGS_FILE_BUFFER_SIZE];
    char folderPath[SETTINGS_URL_BUFFER_SIZE];

    char URLTempType[SETTINGS_URL_BUFFER_SIZE];
    char URLTempPath[SETTINGS_URL_BUFFER_SIZE];
    char MIMETYPE[64];
    char fileType[64];

    char tempLen[8];

    char *token;
    char *buffer = malloc(SETTINGS_FILE_BUFFER_SIZE);
    long lSize;

    int len = 0;
    int tempsize = 0;

    printlog("[GetBuilder.c:HandleGet] Finished setting up for request\n", 1);

    strcpy(request.REQUESTQUERY, fileRequest);

    strcpy(request.FILEPATH, "");
    strncat(request.FILEPATH, SETTINGS_CONTENT_ROOT_PATH, strlen(SETTINGS_CONTENT_ROOT_PATH) - 1);
    request.FILEPATH[strlen(request.FILEPATH)] = '\0';

    strcpy(URLTempPath, strtok(request.REQUESTQUERY, "?"));
    strcpy(request.REQUESTDATA, &request.REQUESTQUERY[strlen(request.REQUESTQUERY) + 1]);
    strcat(request.FILEPATH, URLTempPath);

    printlog("[GetBuilder.c:HandleGet] Relative path is:", 2);
    printlog(request.REQUESTQUERY, 2);
    printlog("\n", 2);

    printlog("[GetBuilder.c:HandleGet] Path is:", 2);
    printlog(request.FILEPATH, 2);
    printlog("\n", 2);

    printlog("[GetBuilder.c:HandleGet] URL data is: ", 1);
    printlog(request.REQUESTDATA, 1);
    printlog("\n", 1);

    token = strtok(URLTempPath, ".");
    while (token != NULL) {
        strcpy(URLTempType, token);
        token = strtok(NULL, ".");
    }
    strcpy(request.FILETYPE, URLTempType);
    MIMEResolve(request.MIMETYPE, request.FILETYPE);

    printlog("[GetBuilder.c:HandleGet] Found filetype: ", 2);
    printlog(request.MIMETYPE, 2);
    printlog("\n", 2);

    char temptype[4];
    strncpy(temptype, request.MIMETYPE, 4);
    temptype[4] = '\0';
    if (strcmp(temptype, "text") == 0) {
        request.ISBINARY = 0;
    }
    else {
        request.ISBINARY = 1;
    }

    request.ISFOLDER = IOCheckFolder(request.FILEPATH);
    printlog("[GetBuilder.c:HandleGet] request.ISFOLDER: ", 2);
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
        printlog("[GetBuilder.c:HandleGet] Generating index page\n", 1);
        
        request.FILESIZE = buildIndexPage(buffer, len, request.FOLDERPATH, request.REQUESTQUERY);
        request.STATUS = 200;
    }
    else {
        if(request.STATUS == 200) {
            if (request.ISBINARY) {
                
            }
            else {
                printlog("[GetBuilder.c:HandleGet] Reading file\n", 1);
                request.STATUS = IOReadText(buffer, request.FILEPATH, &request.FILESIZE);
            }
        }
    }

    if (request.STATUS == 200) {
        printf("Lapped 200\n");
        if (!request.ISBINARY) {
            printlog("\ntesst123\n", 0 );
            buildResponse(response, "200 Ok", request.FILESIZE, request.MIMETYPE, buffer);
        }
        else {
            buildResponse(response, "200 Ok", 255, request.MIMETYPE, "");
        }
    }
    else if (request.STATUS == 404) {
        printlog("[GetBuilder.c:HandleGet] Entering 404 handler\n", 1);
        if (!request.ISFOLDER) {
            int IOStatus = 0;
            printlog("[GetBuilder.c:404Handler] Trying to read 404 file\n", 1);
            IOStatus = IOCheckValid(SETTINGS_ERROR_HANDLING_404);
            if (IOStatus) {
                IOStatus = IOReadText(errorPage, SETTINGS_ERROR_HANDLING_404, &request.FILESIZE);
            }
            else {
                IOStatus = 404;
            }
            printlog("[GetBuilder.c:404Handler] Received status: ", 1);
            printf("%d", IOStatus);
            fflush(stdout);
            if (IOStatus == 404) {
                printlog("[GetBuilder.c:HandleGet] Error in reading 404 page, falling back to hardcoded\n", 0);
                *request.FILESIZE = strlen(page_404);
                printf("%d", request.FILESIZE);
                buildResponse(response, "404 Not Found", request.FILESIZE, "text/html", page_404);
            }
            else if (IOStatus == 500) {
                request.STATUS = 500;
            }
            else {
                buildResponse(response, "404 Not Found", request.FILESIZE, "text/html", errorPage);
            }
        }
        else if (request.ISFOLDER) {
            printlog("[GetBuilder.c:HandleGet] Generating index page\n", 1);
            char indexPage[SETTINGS_FILE_BUFFER_SIZE];
            request.FILESIZE = buildIndexPage(indexPage, strlen(indexPage), folderPath, fileRequest);
            request.STATUS = 200;
            buildResponse(response, "200 Ok", request.FILESIZE, "text/html", indexPage);
            indexPage[0] = '\0';
        }
    }
    if (request.STATUS == 500) {
        printlog("[GetBuilder.c:HandleGet] Entering 500 handler\n", 1);
        if (request.ISFOLDER == 0) {
            int IOStatus = 0;
            printlog("[GetBuilder.c:HandleGet] Generating 500 page\n", 1);
            IOStatus = IOCheckValid(SETTINGS_ERROR_HANDLING_404);
            if (IOStatus) {
                IOStatus = IOReadText(errorPage, SETTINGS_ERROR_HANDLING_500, &request.FILESIZE);
            }
            else {
                IOStatus = 404;
            }

            if (IOStatus == 404) {
                printlog("[GetBuilder.c:HandleGet] Error in reading 500 page, falling back to hardcoded\n", 0);
                *request.FILESIZE = strlen(page_500);
                buildResponse(response, "500 Internal Server Error", request.FILESIZE, "text/html", page_500);
            }
            else {
                buildResponse(response, "500 Internal Server Error", request.FILESIZE, "text/html", errorPage);
            }
        }
        else if (request.ISFOLDER == 1) {
            printlog("[GetBuilder.c:HandleGet] Generating index page\n", 1);
            char indexPage[SETTINGS_FILE_BUFFER_SIZE];
            request.FILESIZE = buildIndexPage(indexPage, sizeof(indexPage), folderPath, fileRequest);
            request.STATUS = 200;
            buildResponse(response, "200 Ok", request.FILESIZE, "text/html", indexPage);
            indexPage[0] = '\0';
        }
    }

    printlog(response, 2);
    printlog("\n", 2);
    send(sock, response, sizeof(response), 0);
    close(sock);
    
    printlog("[GetBuilder.c:HandleGet] Returning to invoker", 1);
}
