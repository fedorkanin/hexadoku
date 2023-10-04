#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "Constants.h"
#include "Hexadoku.h"

Hexadoku* readIn(void);

bool      isDelimiterStringValid(char* string, bool is_dashed);
bool      isLetterValid(char a);

uint8_t*  strToUint8t(char* string);
void      stripString(char* string);
char*     getString(void);
