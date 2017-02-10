#!/usr/bin/env python

from ctypes import *

class _mdsys(Structure):
	_fields_ = [("natoms",c_int),("nfi",c_int),("nsteps",c_int),
				("dt",c_double),("mass",c_double),("epsilon",c_double),
				("sigma",c_double),("box",c_double),("rcut",c_double),
				("ekin",c_double),("epot",c_double),("temp",c_double),
				("rx",POINTER(c_double)),("ry",POINTER(c_double)),("rz",POINTER(c_double)),
				("vx",POINTER(c_double)),("vy",POINTER(c_double)),("vz",POINTER(c_double)),
				("fx",POINTER(c_double)),("fy",POINTER(c_double)),("fz",POINTER(c_double))]

class _syss(Structure):
	_fields_ = [("natoms",c_int),("nfi",c_int),("nsteps",c_int)]

class _posns(Structure):
	_fields_ = [("rx",c_double * 5),("ry",c_double * 5),("rz",c_double * 5)]

if __name__ == '__main__':
	
	# mdso = cdll.LoadLibrary("../obj/ljmd.so")
	# sys = _mdsys(3,0,3,1,1,1,2,10,5,0,0,0)

	# sys = _syss()

	# print(sys.sigma)
	# mdso.force(sys)

	# print(_mdsys.natoms)
	
	pos = _posns()

	print(pos.rx)