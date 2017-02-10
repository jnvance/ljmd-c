#ifndef MISC_H_
#define MISC_H_

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include "global.h"

void azzero(double *d, const int n);
double pbc(double x, const double boxby2);
int deq(double a, double b, double eps);
double seconds();

#endif
