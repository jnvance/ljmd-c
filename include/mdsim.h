#ifndef MDSIM_H_
#define MDSIM_H_

int mdsim(int nprint,int natoms, int nsteps, double mass, double epsilon, 
          double sigma, double box, double rcut, double dt, char restfile[BLEN], 
          char trajfile[BLEN], char ergfile[BLEN]);


#endif