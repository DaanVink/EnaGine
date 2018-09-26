#include <winsock2.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "httpHandle/event.h"
#include "globals.h"
#include "init/init.h"

void initSocket() {

    WSADATA wsaData;
    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
    }
}

//function specific for windows-to be called after all socket communication is complete
void clean() {
    WSACleanup();
}

void server() {
    SOCKET sock, connected = 0;
    int true = 1;

    struct sockaddr_in server_addr,client_addr;
    int sin_size;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket");
        exit(1);
    }

    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &true, sizeof(int)) == -1) {
        perror("Setsockopt");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    memset(&(server_addr.sin_zero),0,8);
        if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))== -1)
    {
            perror("Unable to bind");
            exit(1);
        }

        if (listen(sock, 5) == -1)
    {
            perror("Listen");
            exit(1);
        }

    fflush(stdout);


    sin_size = sizeof(struct sockaddr_in);
    printf("[main.c:server] Ready for request\n");
    connected = accept(sock, (struct sockaddr *)&client_addr,&sin_size);
    printf("[main.c:server] Inbound request\n");
    printf("[main.c:server] Sending request to (event.c:eventThread)\n");
    
    eventThread(connected);

    close(sock);
}

int main() {
    printf("[main.c:main] Starting server\n");
    printf("[main.c:main] Initialising\n");
    init();
    printf("[main.c:main] Init finished\n");
    while(1) {
        //TODO: create thread pool
        //TODO: add img and json support to http response
        initSocket();
        server();
        clean();
    }
    return 0;
}   