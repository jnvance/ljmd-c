/*
 * simple lennard-jones potential MD code with velocity verlet.
 * units: Length=Angstrom, Mass=amu; Energy=kcal
 *
 * unit testing
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>

#include "../include/global.h"
#include "../include/io.h"
#include "../include/misc.h"
#include "../include/force_energy.h"

/* generic file- or pathname buffer length */
/* #define BLEN 200 */

/* main */
int main(int argc, char **argv)
{
    int nprint, i;
    char restfile[BLEN], trajfile[BLEN], ergfile[BLEN], line[BLEN];
    FILE *fp,*traj,*erg;
    mdsys_t sys;

    /* read input file */
    if(get_a_line(stdin,line)) return 1;
    sys.natoms=atoi(line);
    if(get_a_line(stdin,line)) return 1;
    sys.mass=atof(line);
    if(get_a_line(stdin,line)) return 1;
    sys.epsilon=atof(line);
    if(get_a_line(stdin,line)) return 1;
    sys.sigma=atof(line);
    if(get_a_line(stdin,line)) return 1;
    sys.rcut=atof(line);
    if(get_a_line(stdin,line)) return 1;
    sys.box=atof(line);
    if(get_a_line(stdin,restfile)) return 1;
    if(get_a_line(stdin,trajfile)) return 1;
    if(get_a_line(stdin,ergfile)) return 1;
    if(get_a_line(stdin,line)) return 1;
    sys.nsteps=atoi(line);
    if(get_a_line(stdin,line)) return 1;
    sys.dt=atof(line);
    if(get_a_line(stdin,line)) return 1;
    nprint=atoi(line);

#ifdef __TESTS__
    printf("Parameters from input file:\n");
    printf("    natoms                =  %d \n", sys.natoms);
    printf("    mass       (AMU)      =  %f \n", sys.mass);
    printf("    epsilon    (kcal/mol) =  %f \n", sys.epsilon);
    printf("    sigma      (angstrom) =  %f \n", sys.sigma);
    printf("    rcut       (angstrom) =  %f \n", sys.rcut);
    printf("    boxlength  (angstrom) =  %f \n", sys.box);
    printf("    restart    (file)     =  %s \n", restfile);
    printf("    traject    (file)     =  %s \n", trajfile);
    printf("    energies   (file)     =  %s \n", ergfile);
    printf("    nsteps                =  %d \n", sys.nsteps);
    printf("    dt         (fs)       =  %f \n", sys.dt);
    printf("    print freq            =  %d \n", nprint);
    printf("\n");
#endif

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

#ifdef __TESTS__
    printf("Restart with %d particles from %s\n", sys.natoms, restfile);
    for (i=0; i<sys.natoms; ++i) {
        printf("    Particle %d\n",i);
        printf("      pos = %lf  %lf  %lf  \n",*(sys.rx+i), *(sys.ry+i), *(sys.rz+i) );
        printf("      vel = %lf  %lf  %lf  \n",*(sys.vx+i), *(sys.vy+i), *(sys.vz+i) );
    }
    printf("\n");
#endif

    /* initialize forces and energies.*/
    sys.nfi=0;
    force(&sys);
    ekin(&sys);

#ifdef __TESTS__
    printf("TEST initial variables\n");
    printf("    force = %lf  %lf  %lf  \n",*(sys.fx), *(sys.fy), *(sys.fz));
    printf("    Ekin  = %lf  \n",(sys.ekin));
    
    char correctfile[BLEN];
    if(get_a_line(stdin,correctfile)) return 1;
    
    FILE *cF;
    cF=fopen(correctfile,"r");
    
    double cfx, cfy, cfz, cekin;
    fscanf(cF,"%lf %lf %lf ",&cfx,&cfy,&cfz);
    fscanf(cF,"%lf",&cekin);
    fclose(cF);
    
    double eps = 1.0e-1;
    if (deq(cfx,*sys.fx,eps) && deq(cfy,*sys.fy,eps) &&
        deq(cfz,*sys.fz,eps) && deq(cekin,sys.ekin,eps)) {
        printf("PASSED\n");
    } else {
        printf("FAILED\n");
    }
    printf("\n");
#endif

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
    printf("Simulation Done.\n\n");
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
