#include "settings/settingsParser.h"
#include "../globals.h"

void init() {

    getcwd(ENV_CWD, sizeof(ENV_CWD));
    strcpy(SETTINGS_CONTENT_ROOT_PATH, ENV_CWD);
    readSettings("enaConfig.conf1");

}
