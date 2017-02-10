#include "../include/misc.h"
#include "../include/force_energy.h"

/* a few physical constants */
const double kboltz=0.0019872067;     /* boltzman constant in kcal/mol/K */
const double mvsq2e=2390.05736153349; /* m*v^2 in kcal/mol */
const double invmvsq2e=1/2390.05736153349; /* m*v^2 in kcal/mol */

/* compute kinetic energy */
void ekin(mdsys_t *sys)
{
    int i;

    sys->ekin=0.0;
    for (i=0; i<sys->natoms; ++i) {
        sys->ekin += 0.5*mvsq2e*sys->mass*(sys->vx[i]*sys->vx[i] + sys->vy[i]*sys->vy[i] + sys->vz[i]*sys->vz[i]);
    }
    sys->temp = 2.0*sys->ekin/(3.0*sys->natoms-3.0)/kboltz;
}

/* compute forces */
void force(mdsys_t *sys)
{
    double rsq,ffac;
    double rx,ry,rz;
    int i,j;

    /* zero energy and forces */
    sys->epot=0.0;
    azzero(sys->fx,sys->natoms);
    azzero(sys->fy,sys->natoms);
    azzero(sys->fz,sys->natoms);

    double c12 = 4.0*sys->epsilon*pow(sys->sigma,12.0);
    double c6 = 4.0*sys->epsilon*pow(sys->sigma,6.0);
    double rcsq = sys->rcut*sys->rcut;
    double boxby2 = 0.5*sys->box;
    
    for(i=0; i < (sys->natoms)-1; ++i) {
        for(j=i+1; j < (sys->natoms); ++j) {

            rx = sys->rx[i] - sys->rx[j];
            while (rx>  boxby2) rx-= sys->box;
            while (rx< -boxby2) rx+= sys->box;

            ry = sys->ry[i] - sys->ry[j];
            while (ry>  boxby2) ry-= sys->box;
            while (ry< -boxby2) ry+= sys->box;

            rz = sys->rz[i] - sys->rz[j];
            while (rz>  boxby2) rz-= sys->box;
            while (rz< -boxby2) rz+= sys->box;

            rsq = rx*rx + ry*ry + rz*rz;

            /* compute force and energy if within cutoff */
            if (rsq < rcsq) {

                double r6,rinv;
                rinv = 1.0/rsq;
                r6 = rinv*rinv*rinv;
                ffac = (12.0*c12*r6-6.0*c6)*r6*rinv;
                sys->epot += r6*(c12*r6-c6);

                sys->fx[i] += rx*ffac;
                sys->fx[j] -= rx*ffac;
                sys->fy[i] += ry*ffac;
                sys->fy[j] -= ry*ffac;
                sys->fz[i] += rz*ffac;
                sys->fz[j] -= rz*ffac;
            }
        }
    }
}

/* velocity verlet */
void velverlet(mdsys_t *sys)
{
    int i;
    const double invmass = 1.0/sys->mass;

    /* first part: propagate velocities by half and positions by full step */
    // #pragma omp parallel for
    for (i=0; i<sys->natoms; ++i) {
        // sys->vx[i] += 0.5*sys->dt / mvsq2e * sys->fx[i] / sys->mass;
        // sys->vy[i] += 0.5*sys->dt / mvsq2e * sys->fy[i] / sys->mass;
        // sys->vz[i] += 0.5*sys->dt / mvsq2e * sys->fz[i] / sys->mass;

        sys->vx[i] += 0.5*sys->dt * invmvsq2e * sys->fx[i] * invmass;
        sys->vy[i] += 0.5*sys->dt * invmvsq2e * sys->fy[i] * invmass;
        sys->vz[i] += 0.5*sys->dt * invmvsq2e * sys->fz[i] * invmass;
        sys->rx[i] += sys->dt*sys->vx[i];
        sys->ry[i] += sys->dt*sys->vy[i];
        sys->rz[i] += sys->dt*sys->vz[i];
    }

    /* compute forces and potential energy */
    force(sys);

    /* second part: propagate velocities by another half step */
    for (i=0; i<sys->natoms; ++i) {
        sys->vx[i] += 0.5*sys->dt * invmvsq2e * sys->fx[i] * invmass;
        sys->vy[i] += 0.5*sys->dt * invmvsq2e * sys->fy[i] * invmass;
        sys->vz[i] += 0.5*sys->dt * invmvsq2e * sys->fz[i] * invmass;
    }
}
