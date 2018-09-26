#include <winsock2.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "get/handleGet.h"
#include "post/handlePost.h"

struct TempData {

    SOCKET connected;
    int sock;

};

void eventThread(int sock) {
    printf("[event.c:eventThread] Succesfully received request\n");
    char request[9999];
    char method[4];
    char fileRequest[1024];
    long nbytes;

    recv(sock,request,sizeof(request), 0);
    strncpy(method, request, 4);
    printf(request);

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
                //printf("match from %d to %d\n",pos_text-len_search,pos_text);
                break;
            }
        }
        else
        {
           pos_text -=pos_search;
           pos_search = 0;
        }
    }

    //printf("%d\n", fileRequestOffset);
    memcpy(fileRequest, &request[4], fileRequestOffset);
    fileRequest[fileRequestOffset - 4] = '\0';
    //printf("%s\n", fileRequest);

    if (strcmp(method, "GET ")) {
        //printf("get\n");
        printf("[event.c:eventThread] Sending request over to (handleGet.c:HandleGet)\n");
        printf(fileRequest);
        HandleGet(sock, fileRequest, nbytes);
    }

    else if (strcmp(method, "POST")) {
        //printf("post\n");
    }

}
