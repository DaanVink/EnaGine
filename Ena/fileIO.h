#ifndef ENA_FILEIO
#define ENA_FILEIO

int IOGetSize(char* path[]);
int IOCheckFolder(char* path[]);
int IOReadText(char* returnaddr[], char* filename[], int *requestsize);
void IOReadBinary(char* returnaddr[], char* IOStatus[], char* filename[], int *filesize);
int IOCheckValid(char* path);

#endif
