#ifndef LOG_H
#define LOG_H

#include "inugami/logger.h"

typedef Inugami::Logger<5> Log;         //Loading logs: 5; Debug logs: 10;
typedef Inugami::StackLog<Log> SLog;

extern Log* logger;

#endif // LOG_H
