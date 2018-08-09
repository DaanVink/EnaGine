#include <winsock2.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "Ena/httpHandle/event.h"
#include "Ena/globals.h"
#include "Ena/init/init.h"

void server();

int main() {
    init();
    while(1) {
        initSocket();
        server();
        clean();
    }
    return 0;
}

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
    int sock, connected, true = 1;

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
    //bzero(&(server_addr.sin_zero),8); --This is for POSIX based systems
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
    connected = accept(sock, (struct sockaddr *)&client_addr,&sin_size);

    //struct TempData *data = malloc(sizeof(struct TempData));
    //data->connected, connected;
    //data->sock, sock;

    eventThread(connected, sock);
    //CreateThread(NULL, 0, eventThread2, data, 0, NULL);

    WSACleanup();
    close(sock);
}
