#ifndef ENA_FILEIO
#define ENA_FILEIO

int IOCheckFolder(char* path[]);
void IOReadText(char* returnaddr[], char* IOStatus[], char* filename[]);
void IOReadBinary(char* returnaddr[], char* IOStatus[], char* filename[]);

#endif
