#include <string.h>
#include <stdio.h>

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