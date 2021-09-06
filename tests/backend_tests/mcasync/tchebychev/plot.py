#!/usr/bin/python3

############################################################################
# Plots mean (mu), standard deviation (sigma), and significant digits (s)  #
# from a set of stochastic outputs from tchebychev.                        #
############################################################################

import math
import matplotlib.pyplot as plt
import sys
import re
import numpy as np

import matplotlib
# Use Agg backend for headless runs
matplotlib.use('Agg')


def plot_samples(FPType: str, input_filename: str):

    # Set title
    title = f"MCA Tests for {FPType} type"
    plt.figure(title, figsize=(10, 8))
    plt.suptitle(title)

    # Convert binary to decimal precision
    prec_dec = float(32 if FPType == "float" else 64) * math.log(2, 10)

    D = np.loadtxt(input_filename,
                   dtype=dict(names=('i', 'x', 'T'),
                              formats=('i4', 'f8', 'f8')))

    # Compute all statistics (mu, sigma, s)
    x_values = np.unique(D['x'])
    mu_values = []
    mu_sigma_values = []
    sigma_values = []
    s_values = []

    for x in x_values:
        # select all T samples for given x
        T_samples = D[D['x'] == x]['T']

        # Compute mu and sigma statistics
        mu = np.mean(T_samples)
        sigma = np.std(T_samples)

        # Compute significant digits
        if sigma == 0:
            s = prec_dec
        elif mu == 0:
            s = 0
        else:
            # Stott Parker's formula
            s = min(-math.log10(sigma/abs(mu)), prec_dec)

        mu_values.append(mu)
        sigma_values.append(sigma)
        s_values.append(s)

    # Plot significant digits
    plt.subplot(311)
    plt.ylabel("$s$")
    plt.plot(x_values, s_values, '.')

    # Plot standard deviation
    plt.subplot(312)
    plt.ylabel("$\hat \sigma$")
    plt.plot(x_values, sigma_values, '.')

    # Plot samples and mean
    plt.subplot(313)
    plt.xlabel("$x$")
    plt.ylabel("$T(x)$ and $\hat \mu$")
    plt.plot(D['x'], D['T'], 'k.', alpha=0.5)
    plt.plot(x_values, mu_values, '--', color='r')
    # Save plot as pdf
    plotname = "results_" + FPType + ".pdf"
    plt.savefig(plotname, format='pdf')


plot_samples("float", "out_float.dat")
plot_samples("double", "out_double.dat")
plot_samples("longdouble", "out_longdouble.dat")
