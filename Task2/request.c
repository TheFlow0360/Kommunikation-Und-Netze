#include "request.h"

const char isPathValid(struct Request const req)
{

}


void freeRequest(struct Request const req)
{
    free( req.command );
    free( req.path );
    free( req.headers );
}


struct Request newRequest()
{
    struct Request result { .invalid = 0, .command = NULL, .path = NULL, .header = NULL };
    return result;
}
