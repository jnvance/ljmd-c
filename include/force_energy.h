#ifndef FORCE_ENERGY_H_
#define FORCE_ENERGY_H_

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>

#include "../include/global.h"

void ekin(mdsys_t *sys);
void force(mdsys_t *sys);
void velverlet(mdsys_t *sys);
#endif
