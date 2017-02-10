#ifndef GLOBAL_H_
#define GLOBAL_H_

#ifndef BLEN
#define BLEN 200
#endif /* ifndef BLEN */
#include <mpi.h>

/* structure to hold the complete information
 * about the MD system */
struct _mdsys {
    int natoms,nfi,nsteps;
    double dt, mass, epsilon, sigma, box, rcut;
    double ekin, epot, temp;
    double *rx, *ry, *rz;
    double *vx, *vy, *vz;
    double *fx, *fy, *fz;
    double *cx, *cy, *cz;
    int nsize, mpirank;
};
typedef struct _mdsys mdsys_t;


#endif
