#!/usr/bin/env python3
import sys
import os
import argparse
import json
import numpy as np
from .types import *
from .ScatterPath import *

class fefdat(object):
    """class to load & parse .fef.dat data in the scatter program"""
    def __init__(self, paths):
        """init, paths must be a ScatterPath object"""
        assert isinstance(paths, ScatterPath)
        jsondata = paths.jsondata

        # load header data in json file
        mass = float(jsondata['rem']['mass']['value'])
        surfnum = int(jsondata['surfaces']['surfnum']['value'][0])
        Ngammapara = (sum([len(k) for k in jsondata['surfaces']['gammapara']['value']]))
        Nsurfpara = (sum([len(k) for k in jsondata['surfaces']['surfpara']['value']]))

        Ntot = 1
        for k in jsondata['grid']['nr']['value']:
            Ntot *= int(k)

        # load data in fef.dat file
        with open(paths.datfile, "r") as f:
            ## -- check header -- ##
            dim = np.fromfile(f, UINT_T, 1)[0]
            surfnum = np.fromfile(f, UINT_T, 1)[0]
            kT = np.fromfile(f, DOUBLE_T, 1)[0]
            Gamma0 = np.fromfile(f, DOUBLE_T, 1)[0]
            mass = np.fromfile(f, DOUBLE_T, dim)

            assert dim == UINT_T(jsondata['rem']['dim']['value'][0])
            assert surfnum == UINT_T(jsondata['rem']['surfnum']['value'][0])
            assert kT == DOUBLE_T(jsondata['rem']['kt']['value'][0])
            assert Gamma0 == DOUBLE_T(jsondata['rem']['gamma0']['value'][0])
            assert mass == DOUBLE_T(jsondata['simulation']['mass']['value'])

            Ngammapara = (sum([len(k) for k in jsondata['surfaces']['gammapara']['value']]))
            Nsurfpara = (sum([len(k) for k in jsondata['surfaces']['surfpara']['value']]))

            gammapara = np.fromfile(f, DOUBLE_T, Ngammapara)
            surfpara = np.fromfile(f, DOUBLE_T, Nsurfpara)


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
