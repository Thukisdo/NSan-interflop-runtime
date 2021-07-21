#!/usr/bin/python3

import sys
import numpy as np

import matplotlib
# Use Agg backend for headless runs
matplotlib.use('Agg')

import matplotlib.pyplot as plt
import math


one_values = np.loadtxt('output/out_one.dat', skiprows=1, dtype=np.double)
random_values = np.loadtxt('output/out_random.dat', skiprows=1, dtype=np.double)

# Set title
title="Stochastic tests"
plt.figure(title, figsize=(10,8))
plt.suptitle(title)

# Plot one test
fig, ax = plt.subplots(2)
medianprops = dict(linestyle='-', linewidth=0, color='red') # Remove median line
ax[0].boxplot(one_values, medianprops=medianprops, autorange=True, meanline=True, showmeans=True)
ax[0].set_title("Rounding a power of two")


# Plot random test
medianprops = dict(linestyle='-', linewidth=0, color='red') # Remove median line
ax[1].boxplot(random_values, medianprops=medianprops, autorange=True, meanline=True, showmeans=True)
ax[1].set_title("Rounding a random float")

# Set layout
plt.tight_layout()

# Save plot as pdf
plotname="out.pdf"
plt.savefig(plotname, format='pdf')
