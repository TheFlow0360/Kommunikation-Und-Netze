#ifndef HTTPHANDLER_H
#define HTTPHANDLER_H

#include <cnaiapi.h>
#include "request.h"

struct Request const parseRequest(char const* const aInput);

char readHttpRequest(connection aCon, char** const aBuffer);
char const sendHttpResponse(connection aCon, unsigned int const aCode, char const * aText);

#endif
