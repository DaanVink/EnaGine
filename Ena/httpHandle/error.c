#include <string.h>

#include "../globals.h"

typedef struct {
    int code;
    char *content;
} ERROR_ARRAY;

void err404() {
    
}

void unknown_error_4x(char* returnaddr[], int statuscode) {
    ERROR_ARRAY ERRORS[] = {
    {400, "<html><head><title>400 Not Found</title></head><body><h1>Error 400</h1>Bad Request</body></html>" },
    {401, "<html><head><title>401 Not Found</title></head><body><h1>Error 401</h1>Unauthorized</body></html>" },
    {402, "<html><head><title>402 Not Found</title></head><body><h1>Error 402</h1>Payment required</body></html>" },
    {403, "<html><head><title>403 Not Found</title></head><body><h1>Error 403</h1>Forbidden</body></html>" },
    {404, "<html><head><title>404 Not Found</title></head><body><h1>Error 404</h1>Page not found</body></html>" },
    {405, "<html><head><title>405 Not Found</title></head><body><h1>Error 405</h1>Method not allowed</body></html>" },
    {406, "<html><head><title>406 Not Found</title></head><body><h1>Error 406</h1>Not Acceptable</body></html>" },
    {407, "<html><head><title>407 Not Found</title></head><body><h1>Error 407</h1>Proxy Authentication Required</body></html>" },
    {408, "<html><head><title>408 Not Found</title></head><body><h1>Error 408</h1>Request Timeout</body></html>" },
    {409, "<html><head><title>409 Not Found</title></head><body><h1>Error 409</h1>Conflict</body></html>" },
    {410, "<html><head><title>410 Not Found</title></head><body><h1>Error 410</h1>Gone</body></html>" },
    {411, "<html><head><title>411 Not Found</title></head><body><h1>Error 411</h1>Length Required</body></html>" },
    {412, "<html><head><title>412 Not Found</title></head><body><h1>Error 412</h1>Precondition Failed</body></html>" },
    {413, "<html><head><title>413 Not Found</title></head><body><h1>Error 413</h1>Payload Too Large</body></html>" },
    {414, "<html><head><title>414 Not Found</title></head><body><h1>Error 414</h1>URI Too Long</body></html>" },
    {415, "<html><head><title>415 Not Found</title></head><body><h1>Error 415</h1>Unsupported Media Type</body></html>" },
    {416, "<html><head><title>416 Not Found</title></head><body><h1>Error 416</h1>Range Not Satisfiable</body></html>" },
    {417, "<html><head><title>417 Not Found</title></head><body><h1>Error 417</h1>Expectation Failed</body></html>" },
    {418, "<html><head><title>418 Not Found</title></head><body><h1>Error 418</h1>I'm a teapot<p><a href='https://tools.ietf.org/html/rfc2324#section-2.3.2'>Learn more</a></p></body></html>" },
    {421, "<html><head><title>421 Not Found</title></head><body><h1>Error 421</h1>Misdirected Request</body></html>" },
    {422, "<html><head><title>422 Not Found</title></head><body><h1>Error 422</h1>Unprocessable Entity</body></html>" },
    {423, "<html><head><title>423 Not Found</title></head><body><h1>Error 423</h1>Locked</body></html>" },
    {424, "<html><head><title>424 Not Found</title></head><body><h1>Error 424</h1>Failed Dependency</body></html>" },
    {426, "<html><head><title>426 Not Found</title></head><body><h1>Error 426</h1>Upgrade Required</body></html>" },
    {428, "<html><head><title>428 Not Found</title></head><body><h1>Error 428</h1>Precondition Required</body></html>" },
    {429, "<html><head><title>429 Not Found</title></head><body><h1>Error 429</h1>Too Many Requests</body></html>" },
    {431, "<html><head><title>431 Not Found</title></head><body><h1>Error 431</h1>Request Header Fields Too Large</body></html>" },
    {451, "<html><head><title>451 Not Found</title></head><body><h1>Error 451</h1>Unavailable for Legal Reasons</body></html>" }
    };
    int index = 0;
    if (statuscode <= 418) {
        index = statuscode - 400;
    }
    else if (statuscode >= 421 && statuscode <= 424) {
        index = statuscode - 400 - 2;
    }
    else {
        switch (statuscode) {
            case 426:
                index = 23;
                break;
            case 428:
                index = 24;
                break;
            case 429:
                index = 25;
                break;
            case 431:
                index = 26;
                break;
            case 451:
                index = 27;
                break;
        }
    }
    if (index == 0) {
        strcpy(returnaddr, "Unknown error");
    }
    else {
        strcpy(returnaddr, ERRORS[index].content);
    }
}