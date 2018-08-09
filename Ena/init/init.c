#include "settings/settingsParser.h"
#include "../globals.h"

void init() {

    getcwd(ENV_CWD, sizeof(ENV_CWD));
    readSettings("enaConfig.conf1");

}
