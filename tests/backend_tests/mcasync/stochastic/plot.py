#!/usr/bin/python3

import sys
import numpy as np

import matplotlib
# Use Agg backend for headless runs
matplotlib.use('Agg')

import matplotlib.pyplot as plt
import math


FloatValues = np.loadtxt('out_float.dat', skiprows=1)
DoubleValues = np.loadtxt('out_double.dat', skiprows=1)

# Set title
title="Stochastic tests"
plt.figure(title, figsize=(10,8))
plt.suptitle(title)

# Plot one tests
fig, ax = plt.subplots(1, 2)
ax[0].hist(FloatValues, bins=2, histtype='step', color='b', label='float')
ax[1].hist(DoubleValues , bins=2, histtype='step', color='b', label='float')

# Set layout
plt.tight_layout()
plt.subplots_adjust(top=0.9)

# Save plot as pdf
plotname="out.pdf"
plt.savefig(plotname, format='pdf')
