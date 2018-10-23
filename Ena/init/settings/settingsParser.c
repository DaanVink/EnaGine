#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>

#include "../../globals.h"
#include "../../log.h"

typedef struct {
    char *setting;
    int *var;
    char *type;
} proto_settings_dict;

void readSettings(char file) {

    proto_settings_dict SettingsDict[] = {
    {"FileBufferSize", &SETTINGS_FILE_BUFFER_SIZE, "int" },
    {"URLBufferSize", &SETTINGS_URL_BUFFER_SIZE, "int"},
    {"URLDataBufferSize", &SETTINGS_URL_DATA_BUFFER_SIZE, "int"},
    {"DebugLevel", &SETTINGS_DEBUG_LEVEL, "int"},
    {"DisplayDirectoryIndex", &SETTINGS_ERROR_DISPLAY_INDEX_ON_EMPTY_DIR, "switch"},
    {0,0,0}
    };

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

            strcpy(setting, strtok(setting, " = "));
            strcpy(content, &currentLine[strlen(setting) + 3]);
            strcpy(content, strtok(content, "\n"));

            if (strcmp(setting, "\n") == 0) {
                continue; //this line is empty
            }

            else if (strcmp(setting, "<ErrorHandling>\n") == 0) {
                procErrorHandling = 1;
                continue; //skip to the next line
            }

            else if (procErrorHandling) {
                if (strcmp(setting, "</ErrorHandling>\n") == 0) {
                    procErrorHandling = 0;
                    continue; //skip to the next line
                }
                else {

                    if (strcmp(setting, "404") == 0) {
                        strncpy(SETTINGS_ERROR_HANDLING_404, SETTINGS_CONTENT_ROOT_PATH, strlen(SETTINGS_CONTENT_ROOT_PATH) - 1);
                        strcat(SETTINGS_ERROR_HANDLING_404, "\\");
                        strcat(SETTINGS_ERROR_HANDLING_404, content);
                    }
                    if (strcmp(setting, "500") == 0) {
                        strncpy(SETTINGS_ERROR_HANDLING_500, SETTINGS_CONTENT_ROOT_PATH, strlen(SETTINGS_CONTENT_ROOT_PATH) - 1);
                        strcat(SETTINGS_ERROR_HANDLING_500, "\\");
                        strcat(SETTINGS_ERROR_HANDLING_500, content);
                    }
                }
            }

            else {

                int found = 0;
                int index = 0;
                while (found == 0) {
                    if (SettingsDict[index].setting == 0) {
                        break; // end of dict
                    }
                    else if (strcmp(setting, SettingsDict[index].setting) == 0) {
                        if (strcmp(SettingsDict[index].type, "int") == 0) {
                            *SettingsDict[index].var = atoi(content);
                        }
                        else if (strcmp(SettingsDict[index].type, "switch") == 0) {
                            for(int i = 0; content[i]; i++){ //convert content to lowercase
                                content[i] = tolower(content[i]);
                            }
                            if (strcmp(content, "true") == 0 || strcmp(content, "1") == 0) {
                                *SettingsDict[index].var = 1;
                            }
                            else if (strcmp(content, "false") == 0 || strcmp(content, "0") == 0) {
                                *SettingsDict[index].var = 0;
                            }
                            else {
                                printlog("[settingsParser.c:readSettings] Invalid case for ", 0);
                                printlog(setting, 0);
                                printlog(" in config, defaulting to 0 \n", 0);
                                *SettingsDict[index].var = 0;
                            }
                        }
                    }
                    index++;
                }
                if (strcmp(setting, "ContentDirectory") == 0) {
                    strcat(SETTINGS_CONTENT_ROOT, content);
                    strcat(SETTINGS_CONTENT_ROOT_PATH, "\\");
                    strcat(SETTINGS_CONTENT_ROOT_PATH, SETTINGS_CONTENT_ROOT);
                }
            }
        }
    }
}
