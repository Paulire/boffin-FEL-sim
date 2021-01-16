#!/usr/bin/python3
import sys

if len(sys.argv) <= 2:
	exit(2)

import numpy as np
from matplotlib import pyplot as plt

z = np.genfromtxt( sys.argv[1], delimiter=',')[0,:]
#theta = np.genfromtxt( sys.argv[1], delimiter=',')[3:(ELECTRON_NUM+3),:]
#p = np.genfromtxt( sys.argv[1], delimiter=',')[(ELECTRON_NUM+3):,:]


for i in range(2, len(sys.argv)):
	if sys.argv[i] == "-a":
		a = np.genfromtxt( sys.argv[1], delimiter=',')[1,:]
		plt.plot(z,a,'-r')
		plt.yscale('log')
		plt.xlabel("$z\u0305$", size='x-large')
		plt.ylabel("$\u0251$", size='x-large')
		plt.grid(which='both')
		plt.show()
	elif sys.argv[i] == "-phi":
		phi = np.genfromtxt( sys.argv[1], delimiter=',')[2,:]
		plt.plot(z,phi,'-r')
		plt.xlabel("$z\u0305$", size='x-large')
		plt.ylabel("$\u03C6$", size='x-large')
		plt.grid(which='both')
		plt.show()
	elif sys.argv[i] == "-pha":
		print("phase")


