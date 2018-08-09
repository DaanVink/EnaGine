#include <winsock2.h>
#include <stdlib.h>
#include <string.h>

#include "get/handleGet.h"
#include "post/handlePost.h"

struct TempData {

    SOCKET connected;
    int sock;

};

void eventThread(int connected, int sock) {
    char request[9999];
    char method[4];
    char fileRequest[1024];
    recv(connected,request,sizeof(request),0);
    strncpy(method, request, 4);
    printf("%s", request);

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
    printf("%s\n", fileRequest);

    if (strcmp(method, "GET ")) {
        printf("get\n");
        HandleGet(connected, sock, fileRequest);
    }

    else if (strcmp(method, "POST")) {
        printf("post\n");
    }

}

void eventThread2(LPVOID ArgumentData) {

    struct TempData *data = (struct TempData *)ArgumentData;

    char request[9999];        printf("%s",request);
    char response[9999];
    int connected = data->connected;
    SOCKET sock = data->sock;
    recv(connected,request,sizeof(request),0);
    printf("%s",request);

    strcpy(response, "HTTP/1.1 200 OK\nContent-length: 47\nContent-Type: text/html\n\n<html>test</html>");
    int status=send(connected,response,sizeof(response),0);
    printf("%d", status, request);
    WSACleanup();
    close(sock);
}
