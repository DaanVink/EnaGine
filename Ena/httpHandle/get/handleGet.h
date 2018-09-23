#ifndef HANDLEGET_H_INCLUDED
#define HANDLEGET_H_INCLUDED

void HandleGet (SOCKET sock, char fileRequest[], long tempLong);
void buildResponse(char* returnaddr[], char* status[], int length, char* type[], char* data[]);

#endif
