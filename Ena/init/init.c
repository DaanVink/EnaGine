#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "settings/settingsParser.h"
#include "../globals.h"
#include "build/buildFileTree.h"
#include "../httpTypeResolver.h"

void init() {
    SETTINGS_FILE_BUFFER_SIZE = 1024;
    getcwd(ENV_CWD, sizeof(ENV_CWD));
    strcpy(SETTINGS_CONTENT_ROOT_PATH, ENV_CWD);
    readSettings(*"enaConfig.conf1");
    remove("log.txt");
}
