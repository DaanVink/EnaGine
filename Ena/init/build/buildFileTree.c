#define _GNU_SOURCE    // includes _BSD_SOURCE for DT_UNKNOWN etc.
#include <dirent.h>
#include <stdint.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../globals.h"

void buildFileTree() {

    printf(SETTINGS_CONTENT_ROOT_PATH);
    printf(ENV_CWD);
    DIR *dirhandle = opendir(ENV_CWD);     // POSIX doesn't require this to be a plain file descriptor.  Linux uses open(".", O_DIRECTORY); to implement this
    //^Todo: error check
    struct dirent *de;

    while (de = readdir(dirhandle)) {
        _Bool is_dir;
    #ifdef _DIRENT_HAVE_D_TYPE // This means we are on a POSIX system
        if (de->d_type != DT_UNKNOWN && de->d_type != DT_LNK) {
           // don't have to stat if we have d_type info, unless it's a symlink (since we stat, not lstat)
           is_dir = (de->d_type == DT_DIR);
        } else
    #endif
        {  // the only method if d_type isn't available,
           // otherwise this is a fallback for FSes where the kernel leaves it DT_UNKNOWN.
           struct stat stbuf;
           // stat follows symlinks, lstat doesn't.
           stat(de->d_name, &stbuf);              // TODO: error check
           is_dir = S_ISDIR(stbuf.st_mode);
        }

        if (is_dir) {
           printf("%s/\n", de->d_name);
        }
    }
}
