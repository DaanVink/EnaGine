#include <winsock2.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "get/GetBuilder.h"
#include "post/PostBuilder.h"
#include "../log.h"
#include "../tools.h"
#include "request.h"

struct TempData {
    SOCKET connected;
    int sock;
};

void eventThread(int sock) {
    printlog("[event.c:eventThread] Succesfully received request\n", 1);
    char requestText[9999];
    char method[4];
    char fileRequest[1024];
    long nbytes;

    recv(sock,requestText,sizeof(requestText), 0);
    strncpy(method, requestText, 4);
    requestText[strlen(requestText) - 1] = '\0';
    
    //printlog(request, 2);
    //printlog("\n", 2);

    int pos_search = 0;
    int pos_text = 0;
    int len_search = 4;
    int fileRequestOffset = 0;
    int len_text = sizeof(requestText)/sizeof(requestText[0]);

    char httpFind[5] = "HTTP/";
    for (pos_text = 0; pos_text < len_text - len_search;++pos_text)
    {
        if(requestText[pos_text] == httpFind[pos_search])
        {
            ++pos_search;
            if(pos_search == len_search)
            {
                // match
                fileRequestOffset = pos_text - len_search;
                break;
            }
        }
        else
        {
           pos_text -=pos_search;
           pos_search = 0;
        }
    }

    memcpy(fileRequest, &requestText[4], fileRequestOffset);
    fileRequest[fileRequestOffset - 4] = '\0';

    REQUEST request;

    if (strcmp(method, "GET ")) {
        //printlog("get\n");
        printlog("[event.c:eventThread] Sending request over to (handleGet.c:HandleGet)\n", 0);
        request.FILESIZE = 0;
        HandleGet(sock, fileRequest, request);
    }

    else if (strcmp(method, "POST")) {
        //printlog("post\n");
    }

}
