#!/usr/bin/env python3
import sys
import os
import argparse
import json
import numpy as np

import matplotlib as mpl
cmd = 'python -c "import matplotlib.pyplot as plt;plt.figure()"'
HAVE_DISPLAY = ('save' not in sys.argv) and (os.system(cmd) == 0)
if not HAVE_DISPLAY:
    mpl.use('Agg')

from matplotlib import pyplot as plt
from aux import *

def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('infile', type=str, help='<XXX>.in infile')
    return parser.parse_args()


def checkdat():
    args = parse_args()
    paths = ScatterPath(args.infile)

    with open(paths.jsonfile, 'r') as f:
        jsoninfo = json.load(f)

    dim = int(jsoninfo['rem']['dim']['value'][0])
    surfnum = int(jsoninfo['surfaces']['surfnum']['value'][0])
    Ngammapara = (sum([len(k) for k in jsoninfo['surfaces']['gammapara']['value']]))
    Nsurfpara = (sum([len(k) for k in jsoninfo['surfaces']['surfpara']['value']]))

    Ntot = 1
    for k in jsoninfo['grid']['nr']['value']:
        Ntot *= int(k)

    # load data
    with open(paths.datfile, "r") as f:
        dim = np.fromfile(f, UINT_T, 1)[0]
        surfnum = np.fromfile(f, UINT_T, 1)[0]
        kT = np.fromfile(f, DOUBLE_T, 1)[0]
        Gamma0 = np.fromfile(f, DOUBLE_T, 1)[0]
        mass = np.fromfile(f, DOUBLE_T, dim)
        para = np.fromfile(f, DOUBLE_T, Ngammapara + Nsurfpara)
        rmin = np.fromfile(f, DOUBLE_T, dim)
        rmax = np.fromfile(f, DOUBLE_T, dim)
        dr = np.fromfile(f, DOUBLE_T, dim)
        force = np.fromfile(f, DOUBLE_T, dim * Ntot)
        efric = np.fromfile(f, DOUBLE_T, dim**2 * Ntot)
        fBCME = np.fromfile(f, DOUBLE_T, dim * Ntot)

    Nr = list([int(k) for k in (rmax - rmin) / dr])

    rarr = []
    for a, b, c in zip(rmin, rmax, Nr):
        rarr.append(np.linspace(a, b, c, endpoint=False))

    forceshape = [dim] + Nr
    forceshape.reverse()
    efricshape = [dim**2] + Nr
    efricshape.reverse()
    fBCMEshape = [dim] + Nr
    fBCMEshape.reverse()

    print(force.size, forceshape)
    force = force.reshape(forceshape)
    efric = efric.reshape(efricshape)
    fBCME = fBCME.reshape(fBCMEshape)

    forcelim = [-2, 2]
    efriclim = [-1e2, 1e2]
    fBCMElim = [-0.2, 0.2]

    datalim = fBCMElim
    delta = 1e-10
    for i in range(dim):
        data = fBCME[:,:,i]
        levels = np.linspace(
                    max(np.min(data) - delta, datalim[0]),
                    min(np.max(data) + delta, datalim[1]),
                    100)

        fig = plt.figure()
        ax = fig.add_subplot(111, projection=None)
        cf = ax.contourf(rarr[0], rarr[1], data, levels=levels, cmap=plt.cm.jet)
        plt.colorbar(cf)
        saveto = 'fBCME{}.png'.format(i)
        fig.savefig(saveto, dpi=2 * fig.dpi)

    datalim = forcelim
    delta = 1e-10
    for i in range(dim):
        data = force[:,:,i]
        levels = np.linspace(
                    max(np.min(data) - delta, datalim[0]),
                    min(np.max(data) + delta, datalim[1]),
                    100)

        fig = plt.figure()
        ax = fig.add_subplot(111, projection=None)
        cf = ax.contourf(rarr[0], rarr[1], data, levels=levels, cmap=plt.cm.jet)
        plt.colorbar(cf)
        saveto = 'force{}.png'.format(i)
        fig.savefig(saveto, dpi=2 * fig.dpi)

    datalim = efriclim
    delta = 1e-10
    for i in range(dim**2):
        data = efric[:,:,i]
        levels = np.linspace(
                    max(np.min(data) - delta, datalim[0]),
                    min(np.max(data) + delta, datalim[1]),
                    100)

        fig = plt.figure()
        ax = fig.add_subplot(111, projection=None)
        cf = ax.contourf(rarr[0], rarr[1], data, levels=levels, cmap=plt.cm.jet)
        plt.colorbar(cf)
        saveto = 'efric{}.png'.format(i)
        fig.savefig(saveto, dpi=2 * fig.dpi)

checkdat()
