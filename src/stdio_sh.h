#ifndef STDIO_SH_H
#define  STDIO_SH_H

#define  NUM_ARGS_CMD 3
#define  STR_LEN_MAX 100
#define  OBJ_COUNT_MAX 100 // Максимальное количество элементов в блоке

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "graphs_.h"

  int   ArgLineCheck  (const int   given);
 FILE*  FileOpen      (const char* name,  const char* mode);
GRAPH*  FileRead      (FILE* fr);

#endif
