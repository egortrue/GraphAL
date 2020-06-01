#ifndef STDIO_SH_H
#define STDIO_SH_H

#define  STR_LEN_MAX 100
#define  OBJ_COUNT_MAX 100 // max number of string in block of data

#include <stdio.h>
#include "graphs.h"

FILE*  FileOpen(const char* name,  const char* mode);

GRAPH* GraphRead(FILE* input);

#endif
