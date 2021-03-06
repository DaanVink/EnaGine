#ifndef ENA_REQUEST
#define ENA_REQUEST

#include "../globals.h"

typedef struct {
    char REQUESTQUERY[1024];
    char REQUESTDATA[1024];
    char FILEPATH[PATH_MAX];
    char FOLDERPATH[PATH_MAX];
    char FILETYPE[64];
    char MIMETYPE[64];
    int  STATUS;
    int  ISBINARY;
    int  ISFOLDER;
    int* FILESIZE;
} REQUEST;

#endif