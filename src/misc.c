
#include "misc.h"
/* helper function: zero out an array */
void azzero(double *d, const int n)
{
    int i;
    for (i=0; i<n; ++i) {
        d[i]=0.0;
    }
}

/* helper function: apply minimum image convention */
double pbc(double x, const double boxby2)
{
    while (x >  boxby2) x -= 2.0*boxby2;
    while (x < -boxby2) x += 2.0*boxby2;
    return x;
}

/* determine whether difference falls within eps */
int deq(double a, double b, double eps)
{
    double diff = a - b;
    if ((diff > 0.0 && diff < eps) || (diff < 0.0 && -1.0*diff < eps) )
    {
        return 1;
    } else {
        return 0;
    }
}