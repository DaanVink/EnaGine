#ifndef ENA_FILEIO
#define ENA_FILEIO

int IOGetSize(char* path[]);
int IOCheckFolder(char* path[]);
void IOReadText(int* returnaddr, char* IOStatus[], char* filename[]);
void IOReadBinary(char* returnaddr[], char* IOStatus[], char* filename[], int filesize);
int IOCheckValid(char* path);

#endif
