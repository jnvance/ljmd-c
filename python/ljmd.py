#!/usr/bin/env python

from ctypes import *
import os

mdso = cdll.LoadLibrary("../obj/ljmd.so")

def run_mdsim(inputfile):

	# parse inputfile
	f = open(inputfile, 'r')
	natoms,mass,epsilon,sigma,rcut,box,restfile,\
	trajfile,ergfile,nsteps,dt,nprint = [line.split('#')[0] for line in f.readlines()]

	# convert input to ctype
	c_nprint   = c_int(int(nprint))
	c_natoms   = c_int(int(natoms))
	c_nsteps   = c_int(int(nsteps))
	c_mass     = c_double(float("".join(mass.split())))
	c_epsilon  = c_double(float("".join(epsilon.split())))
	c_sigma    = c_double(float("".join(sigma.split())))
	c_box      = c_double(float("".join(box.split())))
	c_rcut     = c_double(float("".join(rcut.split())))
	c_dt       = c_double(float("".join(dt.split())))
	c_restfile = c_char_p("".join(restfile.split()).encode())
	c_trajfile = c_char_p("".join(trajfile.split()).encode())
	c_ergfile  = c_char_p("".join(ergfile.split()).encode())

	# run simulation
	mdso.mdsim(c_nprint,c_natoms,c_nsteps,c_mass,c_epsilon,c_sigma,
			   c_box,c_rcut,c_dt,c_restfile.value,c_trajfile.value,c_ergfile.value)


if __name__ == '__main__':
	
	
	inputfile = "argon_108.inp"
	run_mdsim(inputfile)