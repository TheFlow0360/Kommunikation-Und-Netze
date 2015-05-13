#ifndef HTTPHANDLER_H
#define HTTPHANDLER_H

#include "request.h"

namespace HttpHandler {

    Request const parseRequest(char const * const aInput);

}

#endif // HTTPHANDLER_H
