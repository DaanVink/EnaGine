#ifndef ENA_ERROR_INCLUDED
#define ENA_ERROR_INCLUDED

void err404();
void err500();
void unknown_error_4x(char* returnaddr[], int statuscode);

#endif
