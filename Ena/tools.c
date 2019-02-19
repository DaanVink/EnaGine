#include <string.h>
#include <stdio.h>

int* buildResponse(char* returnaddr[], char* status[], int *length, char* type[]) {

    char lengthBuffer[128];
    strcpy(returnaddr, "HTTP/1.1 ");
    strcat(returnaddr, status);
    strcat(returnaddr, "\nContent-length: ");
    sprintf(lengthBuffer, "%d", length);
    strcat(returnaddr, lengthBuffer);
    strcat(returnaddr, "\nContent-Type: ");
    strcat(returnaddr, type);
    strcat(returnaddr, "\n\n");
    returnaddr[strlen(returnaddr) - 1] = '\0';
    int len = strlen(returnaddr) + length;
    return len;

}