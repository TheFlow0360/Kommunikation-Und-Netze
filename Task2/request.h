#ifndef REQUEST_H
#define REQUEST_H

struct Request {
    char  invalid;
    char* command;
    char* path;
    char* httpVersion;
    char* headers;
};

struct Request newRequest();
void freeRequest(struct Request* const req);

void checkPathValid(struct Request* const req);
void checkCommandValid(struct Request* const req);
void getFullPath(struct Request* const req);
void printRequestData(struct Request* const  req);

#endif
