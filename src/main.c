/*
 * simple lennard-jones potential MD code with velocity verlet.
 * units: Length=Angstrom, Mass=amu; Energy=kcal
 *
 * baseline c version.
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#include "global.h"
#include "io.h"
#include "misc.h"
#include "force_energy.h"
#include "mdsim.h"

/* generic file- or pathname buffer length */
/* #define BLEN 200 */

/* main */
int main(int argc, char **argv)
{
    int nprint;
    int natoms, nsteps;
    double mass, epsilon, sigma, box, rcut, dt;

    char restfile[BLEN], trajfile[BLEN], ergfile[BLEN], line[BLEN];


    if(get_a_line(stdin,line)) return 1;
    natoms=atoi(line);
    if(get_a_line(stdin,line)) return 1;
    mass=atof(line);
    if(get_a_line(stdin,line)) return 1;
    epsilon=atof(line);
    if(get_a_line(stdin,line)) return 1;
    sigma=atof(line);
    if(get_a_line(stdin,line)) return 1;
    rcut=atof(line);
    if(get_a_line(stdin,line)) return 1;
    box=atof(line);
    if(get_a_line(stdin,restfile)) return 1;
    if(get_a_line(stdin,trajfile)) return 1;
    if(get_a_line(stdin,ergfile)) return 1;
    if(get_a_line(stdin,line)) return 1;
    nsteps=atoi(line);
    if(get_a_line(stdin,line)) return 1;
    dt=atof(line);
    if(get_a_line(stdin,line)) return 1;
    nprint=atoi(line);

    MPI_Init( &argc, &argv );
    mdsim( nprint, natoms,  nsteps,  mass,  epsilon,
           sigma,  box,  rcut,  dt, restfile,
           trajfile, ergfile);

    MPI_Finalize();
    return 0;
}
