#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "Constants.h"
#include "InputFunctions.h"
#include "stdio.h"
#include "stdlib.h"

bool isHexadokuValid(uint8_t** hexadoku);
void printHexadoku(uint8_t** hexadoku);
