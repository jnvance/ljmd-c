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

#include "global.h"
#include "io.h"
#include "misc.h"
#include "force_energy.h"

/* generic file- or pathname buffer length */
/* #define BLEN 200 */

int mdsim(int nprint,int natoms, int nsteps, double mass, double epsilon, 
          double sigma, double box, double rcut, double dt, char restfile[BLEN], 
          char trajfile[BLEN], char ergfile[BLEN])
{
    int i;
    FILE *fp,*traj,*erg;
    mdsys_t sys;

    sys.natoms=natoms;
    sys.mass=mass;
    sys.epsilon=epsilon;
    sys.sigma=sigma;
    sys.rcut=rcut;
    sys.box=box;
    sys.nsteps=nsteps;
    sys.dt=dt;

    /* allocate memory */
    sys.rx=(double *)malloc(sys.natoms*sizeof(double));
    sys.ry=(double *)malloc(sys.natoms*sizeof(double));
    sys.rz=(double *)malloc(sys.natoms*sizeof(double));
    sys.vx=(double *)malloc(sys.natoms*sizeof(double));
    sys.vy=(double *)malloc(sys.natoms*sizeof(double));
    sys.vz=(double *)malloc(sys.natoms*sizeof(double));
    sys.fx=(double *)malloc(sys.natoms*sizeof(double));
    sys.fy=(double *)malloc(sys.natoms*sizeof(double));
    sys.fz=(double *)malloc(sys.natoms*sizeof(double));

    /* read restart */
    fp=fopen(restfile,"r");
    if(fp) {
        for (i=0; i<sys.natoms; ++i) {
            fscanf(fp,"%lf%lf%lf",sys.rx+i, sys.ry+i, sys.rz+i);
        }
        for (i=0; i<sys.natoms; ++i) {
            fscanf(fp,"%lf%lf%lf",sys.vx+i, sys.vy+i, sys.vz+i);
        }
        fclose(fp);
        azzero(sys.fx, sys.natoms);
        azzero(sys.fy, sys.natoms);
        azzero(sys.fz, sys.natoms);
    } else {
        perror("cannot read restart file");
        return 3;
    }

    /* initialize forces and energies.*/
    sys.nfi=0;
    force(&sys);
    ekin(&sys);

    erg=fopen(ergfile,"w");
    traj=fopen(trajfile,"w");

    printf("Starting simulation with %d atoms for %d steps.\n",sys.natoms, sys.nsteps);
    printf("     NFI            TEMP            EKIN                 EPOT              ETOT\n");
    output(&sys, erg, traj);

    /**************************************************/
    /* main MD loop */
    for(sys.nfi=1; sys.nfi <= sys.nsteps; ++sys.nfi) {

        /* write output, if requested */
        if ((sys.nfi % nprint) == 0)
            output(&sys, erg, traj);

        /* propagate system and recompute energies */
        velverlet(&sys);
        ekin(&sys);
    }
    /**************************************************/

    /* clean up: close files, free memory */
    printf("Simulation Done.\n");
    fclose(erg);
    fclose(traj);

    free(sys.rx);
    free(sys.ry);
    free(sys.rz);
    free(sys.vx);
    free(sys.vy);
    free(sys.vz);
    free(sys.fx);
    free(sys.fy);
    free(sys.fz);

    return 0;
}






/* main */
int main(int argc, char **argv)
{

    int nprint; 
    int natoms, nsteps;
    double mass, epsilon, sigma, box, rcut, dt;
    
    char restfile[BLEN], trajfile[BLEN], ergfile[BLEN], line[BLEN];

    /* read input file */
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

    mdsim( nprint, natoms,  nsteps,  mass,  epsilon, 
           sigma,  box,  rcut,  dt, restfile, 
           trajfile, ergfile);

    return 0;
}
