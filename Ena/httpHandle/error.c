#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "../globals.h"

typedef struct {
    int code;
    char *content;
} ERROR_ARRAY;

ERROR_ARRAY ERRORS_4[] = {
    {400, "<html><head><title>400 Bad Request</title></head><body><h1>Error 400</h1>Bad Request</body></html>" },
    {401, "<html><head><title>401 Unauthorized</title></head><body><h1>Error 401</h1>Unauthorized</body></html>" },
    {402, "<html><head><title>402 Payment required</title></head><body><h1>Error 402</h1>Payment required</body></html>" },
    {403, "<html><head><title>403 Forbidden</title></head><body><h1>Error 403</h1>Forbidden</body></html>" },
    {404, "<html><head><title>404 Not Found</title></head><body><h1>Error 404</h1>Page not found</body></html>" },
    {405, "<html><head><title>405 Method not allowed</title></head><body><h1>Error 405</h1>Method not allowed</body></html>" },
    {406, "<html><head><title>406 Not Acceptable</title></head><body><h1>Error 406</h1>Not Acceptable</body></html>" },
    {407, "<html><head><title>407 Proxy Authentication Required</title></head><body><h1>Error 407</h1>Proxy Authentication Required</body></html>" },
    {408, "<html><head><title>408 Request Timeout</title></head><body><h1>Error 408</h1>Request Timeout</body></html>" },
    {409, "<html><head><title>409 Conflict</title></head><body><h1>Error 409</h1>Conflict</body></html>" },
    {410, "<html><head><title>410 Gone</title></head><body><h1>Error 410</h1>Gone</body></html>" },
    {411, "<html><head><title>411 Length Required</title></head><body><h1>Error 411</h1>Length Required</body></html>" },
    {412, "<html><head><title>412 Precondition Failed</title></head><body><h1>Error 412</h1>Precondition Failed</body></html>" },
    {413, "<html><head><title>413 Payload Too Large</title></head><body><h1>Error 413</h1>Payload Too Large</body></html>" },
    {414, "<html><head><title>414 URI Too Long</title></head><body><h1>Error 414</h1>URI Too Long</body></html>" },
    {415, "<html><head><title>415 Unsupported Media Type</title></head><body><h1>Error 415</h1>Unsupported Media Type</body></html>" },
    {416, "<html><head><title>416 Range Not Satisfiable</title></head><body><h1>Error 416</h1>Range Not Satisfiable</body></html>" },
    {417, "<html><head><title>417 Expectation Failed</title></head><body><h1>Error 417</h1>Expectation Failed</body></html>" },
    {418, "<html><head><title>418 I'm a teapot</title></head><body><h1>Error 418</h1>I'm a teapot<p><a href='https://tools.ietf.org/html/rfc2324#section-2.3.2'>Learn more</a></p></body></html>" },
    {419, "<html><head><title>419 Unassigned</title></head><body><h1>Error 419<h1>Error code unassigned</h1></body></html>" },
    {420, "<html><head><title>420 Unassigned</title></head><body><h1>Error 420<h1>Error code unassigned</h1></body></html>" },
    {421, "<html><head><title>421 Misdirected Request</title></head><body><h1>Error 421</h1>Misdirected Request</body></html>" },
    {422, "<html><head><title>422 Unprocessable Entity</title></head><body><h1>Error 422</h1>Unprocessable Entity</body></html>" },
    {423, "<html><head><title>423 Locked</title></head><body><h1>Error 423</h1>Locked</body></html>" },
    {424, "<html><head><title>424 Failed Dependency</title></head><body><h1>Error 424</h1>Failed Dependency</body></html>" },
    {425, "<html><head><title>415 Unassigned</title></head><body><h1>Error 415<h1>Error code unassigned</h1></body></html>" },
    {426, "<html><head><title>426 Upgrade Required</title></head><body><h1>Error 426</h1>Upgrade Required</body></html>" },
    {427, "<html><head><title>427 Unassigned</title></head><body><h1>Error 427<h1>Error code unassigned</h1></body></html>" },
    {428, "<html><head><title>428 Precondition Required</title></head><body><h1>Error 428</h1>Precondition Required</body></html>" },
    {429, "<html><head><title>429 Too Many Requests</title></head><body><h1>Error 429</h1>Too Many Requests</body></html>" },
    {430, "<html><head><title>430 Unassigned</title></head><body><h1>Error 430<h1>Error code unassigned</h1></body></html>" },
    {431, "<html><head><title>431 Request Header Fields Too Large</title></head><body><h1>Error 431</h1>Request Header Fields Too Large</body></html>" },
    {451, "<html><head><title>451 Unavailable for Legal Reasons</title></head><body><h1>Error 451</h1>Unavailable for Legal Reasons</body></html>" }
    };

void defaultError(char* returnaddr[], int statuscode) {
    char str[3];
    sprintf(str, "%d", statuscode);
    int index = 0;
    switch (atoi(&str[0])) {
        case 4:
            if (statuscode == 451) {
                index = 0;
            }
            else {
                index = atoi(memmove(str, str+1, strlen(str)));
            }
            printf("%d", index);
    }
    strcpy(returnaddr, ERRORS_4[index].content);
}