#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#include "../../globals.h"

void readSettings(char file) {

    FILE *settingsFile;
    int procErrorHandling = 0;

    if (access( "enaConfig.conf", F_OK) != 0) {
        char message[PATH_MAX + 100];
        char cwdBuffer[PATH_MAX];
        strcpy(message, "Could not access settings file in ");
        strcat(message, getcwd(cwdBuffer, sizeof(cwdBuffer)));
        strcat(message, "/enaConfig.conf");
        printf(message);
    }
    else {
        settingsFile = fopen("enaConfig.conf", "r+");
        const size_t line_size = 256;
        char *currentLine = malloc(line_size);

        while (fgets(currentLine, line_size, settingsFile) != NULL)  {

            char setting[256];
            strcpy(setting, currentLine);
            char content[256];

            if (strcmp(setting, "\n") == 0) {
                continue; //this line is empty
            }

            else if (strcmp(setting, "+ErrorHandling\n") == 0) {
                procErrorHandling = 1;
                //printf("start\n");
                continue; //skip to the next line
            }

            else if (procErrorHandling) {
                if (strcmp(setting, "-ErrorHandling\n") == 0) {
                    //printf("end\n");
                    procErrorHandling = 0;
                    continue; //skip to the next line
                }
                else {
                    strcpy(setting, strtok(setting, " = "));
                    strcpy(content, &currentLine[strlen(setting) + 3]);

                    //printf("%s", setting);
                    //printf("%s", content);
                    if (strcmp(setting, "404") == 0) {
                        strncpy(SETTINGS_ERROR_HANDLING_404, SETTINGS_CONTENT_ROOT_PATH, strlen(SETTINGS_CONTENT_ROOT_PATH) - 1);
                        strcat(SETTINGS_ERROR_HANDLING_404, "\\");
                        strncat(SETTINGS_ERROR_HANDLING_404, content, strlen(content) -1);
                        SETTINGS_ERROR_HANDLING_404[strlen(SETTINGS_ERROR_HANDLING_404)+1] = "\0";
                    }
                    if (strcmp(setting, "500") == 0) {
                        strncpy(SETTINGS_ERROR_HANDLING_500, SETTINGS_CONTENT_ROOT_PATH, strlen(SETTINGS_CONTENT_ROOT_PATH) - 1);
                        strcat(SETTINGS_ERROR_HANDLING_500, "\\");
                        strncat(SETTINGS_ERROR_HANDLING_500, content, strlen(content) -1);
                        SETTINGS_ERROR_HANDLING_500[strlen(SETTINGS_ERROR_HANDLING_500)+1] = "\0";
                    }
                }
            }

            else {
                strcpy(setting, strtok(setting, " = "));
                strcpy(content, &currentLine[strlen(setting) + 3]);

                //printf("%s ", setting);
                //printf("%s\n", content);
                if (strcmp(setting, "ContentDirectory") == 0) {
                    strcat(SETTINGS_CONTENT_ROOT, content);
                    strcat(SETTINGS_CONTENT_ROOT_PATH, "\\");
                    strcat(SETTINGS_CONTENT_ROOT_PATH, SETTINGS_CONTENT_ROOT);
                }

                else if (strcmp(setting, "DefaultFile") == 0) {
                    strncpy(SETTINGS_CONTENT_DEFAULT_FILE, content, strlen(content) - 1);
                }

                else if (strcmp(setting, "FileBufferSize") == 0) {
                    SETTINGS_FILE_BUFFER_SIZE = atoi(content);
                }
                else if (strcmp(setting, "URLBufferSize") == 0) {
                    SETTINGS_URL_BUFFER_SIZE = atoi(content);
                }
                else if (strcmp(setting, "URLDataBufferSize") == 0) {
                    SETTINGS_URL_DATA_BUFFER_SIZE = atoi(content);
                }

                else if (strcmp(setting, "DisplayDirectoryIndex") == 0) {
                    if (strcmp(content, "true\n") == 0) {
                        SETTINGS_ERROR_DISPLAY_INDEX_ON_EMPTY_DIR = 1;
                    }
                    else if (strcmp(content, "false\n") == 0) {
                        SETTINGS_ERROR_DISPLAY_INDEX_ON_EMPTY_DIR = 0;
                    }
                    else {
                        SETTINGS_ERROR_DISPLAY_INDEX_ON_EMPTY_DIR = 1;
                        printf("Invalid case for DisplayDirectoryIndex in config. Defaulting to true");
                    }
                }
            }
        }
    }
}
