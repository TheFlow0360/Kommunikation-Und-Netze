#ifndef REQUEST_H
#define REQUEST_H

struct Request {
    char  invalid;
    char* command;
    char* path;
    char* headers;
};

struct Request newRequest();
void freeRequest(struct Request const req);

char const isPathValid(struct Request const req);


#endif // REQUEST_H
