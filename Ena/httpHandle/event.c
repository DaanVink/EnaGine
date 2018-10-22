#include <winsock2.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "get/handleGet.h"
#include "post/handlePost.h"
#include "../log.h"

struct TempData {

    SOCKET connected;
    int sock;

};

void eventThread(int sock) {
    printlog("[event.c:eventThread] Succesfully received request\n");
    char request[9999];
    char method[4];
    char fileRequest[1024];
    long nbytes;

    recv(sock,request,sizeof(request), 0);
    strncpy(method, request, 4);
    request[strlen(request) - 1] = '\0';
    
    printlog(request);
    printlog("\n");

    int pos_search = 0;
    int pos_text = 0;
    int len_search = 4;
    int fileRequestOffset = 0;
    int len_text = sizeof(request)/sizeof(request[0]);

    char httpFind[5] = "HTTP/";
    for (pos_text = 0; pos_text < len_text - len_search;++pos_text)
    {
        if(request[pos_text] == httpFind[pos_search])
        {
            ++pos_search;
            if(pos_search == len_search)
            {
                // match
                fileRequestOffset = pos_text - len_search;
                //printlog("match from %d to %d\n",pos_text-len_search,pos_text);
                break;
            }
        }
        else
        {
           pos_text -=pos_search;
           pos_search = 0;
        }
    }

    //printlog("%d\n", fileRequestOffset);
    memcpy(fileRequest, &request[4], fileRequestOffset);
    fileRequest[fileRequestOffset - 4] = '\0';
    //printlog("%s\n", fileRequest);

    if (strcmp(method, "GET ")) {
        //printlog("get\n");
        printlog("[event.c:eventThread] Sending request over to (handleGet.c:HandleGet)\n");
        printlog(fileRequest);
        printlog("\n");
        HandleGet(sock, fileRequest, nbytes);
    }

    else if (strcmp(method, "POST")) {
        //printlog("post\n");
    }

}
