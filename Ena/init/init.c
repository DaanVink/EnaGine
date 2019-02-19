#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "settings/settingsParser.h"
#include "../globals.h"
#include "build/buildFileTree.h"
#include "../MIMEResolver.h"
#include "../log.h"

void init() {
    SETTINGS_FILE_BUFFER_SIZE = 1024;
    getcwd(ENV_CWD, sizeof(ENV_CWD));
    printlog("[init.c:init] Working directory is ", 1);
    printlog(ENV_CWD, 1);
    printlog("\n", 1);
    strcpy(SETTINGS_CONTENT_ROOT_PATH, ENV_CWD);
    readSettings(*"enaConfig.conf");
    printlog("[init.c:init] Finished parsing settings\n", 0);
    remove("log.txt");
}
