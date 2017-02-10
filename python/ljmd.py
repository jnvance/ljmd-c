#!/usr/bin/env python

import ctypes
import os, sys

def run_mdsim(inputfile):
	
	# parse inputfile
	f = open(inputfile, 'r')
	natoms,mass,epsilon,sigma,rcut,box,restfile,\
	trajfile,ergfile,nsteps,dt,nprint = [line.split('#')[0] for line in f.readlines()]

	# convert input to ctype
	c_nprint   = ctypes.c_int(int(nprint))
	c_natoms   = ctypes.c_int(int(natoms))
	c_nsteps   = ctypes.c_int(int(nsteps))
	c_mass     = ctypes.c_double(float("".join(mass.split())))
	c_epsilon  = ctypes.c_double(float("".join(epsilon.split())))
	c_sigma    = ctypes.c_double(float("".join(sigma.split())))
	c_box      = ctypes.c_double(float("".join(box.split())))
	c_rcut     = ctypes.c_double(float("".join(rcut.split())))
	c_dt       = ctypes.c_double(float("".join(dt.split())))
	c_restfile = ctypes.c_char_p("".join(restfile.split()).encode())
	c_trajfile = ctypes.c_char_p("".join(trajfile.split()).encode())
	c_ergfile  = ctypes.c_char_p("".join(ergfile.split()).encode())

	# run simulation
	try:
		mdso = ctypes.cdll.LoadLibrary("../obj/ljmd.so")
		mdso.mdsim(c_nprint,c_natoms,c_nsteps,c_mass,c_epsilon,c_sigma,
			   c_box,c_rcut,c_dt,c_restfile.value,c_trajfile.value,c_ergfile.value)
	except (OSError):
		print("Shared object `../obj/ljmd.so` not found.\nRun `make` and `make shared` from parent directory.")

if __name__ == '__main__':
	
	if len(sys.argv) > 1:
		inputfile = str(sys.argv[1])
		run_mdsim(inputfile)
	else:
		pass
