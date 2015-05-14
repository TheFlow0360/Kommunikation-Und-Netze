#include <stdlib.h>

#include "request.h"

char const isPathValid(struct Request const req)
{
    return 1;
}


void freeRequest(struct Request const req)
{
    free( req.command );
    free( req.path );
    free( req.headers );
}


struct Request newRequest()
{
    struct Request result;
    result.invalid = 0;
    result.command = NULL;
    result.path = NULL;
    result.headers = NULL;
    return result;
}
