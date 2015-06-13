#ifndef HTTPHANDLER_H
#define HTTPHANDLER_H

#include "connectionhandler.h"
#include "request.h"

struct Request const parseRequest(char const* const aInput);

char readHttpRequest(connection aCon, char** const aBuffer);
char const sendHttpResponse(connection aCon, unsigned int const aCode, char const * aText);

char parseDriveCmd( char const* const cmd , float* const left, float* const right);

#endif
