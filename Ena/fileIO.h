#ifndef ENA_FILEIO
#define ENA_FILEIO

int IOCheckFolder(char* path[]);
void IOReadText(char* returnaddr[], int status, char* filename[]);
void IOReadBinary(char* returnaddr[], int status, char* filename[]);

#endif
