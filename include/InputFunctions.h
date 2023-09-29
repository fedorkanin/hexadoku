#ifndef INPUT_FUNCTIONS_H
#define INPUT_FUNCTIONS_H

#include <stdbool.h>
#include <stdint.h>

#include "Constants.h"

bool     isDelimiterStringValid(char* string, bool is_dashed);
bool     isLetterValid(char a);
uint8_t* strToUint8t(char* string);
void     stripString(char* string);
char*    getString();

#endif  // INPUT_FUNCTIONS_H
