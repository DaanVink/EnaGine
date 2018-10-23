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
#include "log.h"

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
    printlog("[main.c:server] Ready for request\n", 0);
    connected = accept(sock, (struct sockaddr *)&client_addr,&sin_size);
    printlog("[main.c:server] Inbound request\n", 0);
    printlog("[main.c:server] Sending request to (event.c:eventThread)\n", 0);
    
    eventThread(connected);

    close(sock);
    printlog("[main.c:server] Closed thread\n", 0);
}

int main() {
    printlog("[main.c:main] Starting server\n", 0);
    printlog("[main.c:main] Initialising\n", 0);
    init();
    printlog("[main.c:main] Init finished\n", 0);
    while(1) {
        //TODO: create thread pool
        //TODO: add img and json support to http response
        printlog("[main.c:main] Initialising socket\n", 1);
        initSocket();
        printlog("[main.c:main] Waiting for request\n", 1);
        server();
        printlog("[main.c:main] Cleaning\n", 1);
        clean();
        printlog("==============================\n", 0);
    }
    return 0;
}