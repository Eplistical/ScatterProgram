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
    parser.add_argument('--savedir', type=str, help='directory to save plots')
    return parser.parse_args()


def checkdyn():
    """function to plot dyn"""
    args = parse_args()
    paths = ScatterPath(args.infile)
    dyn = dyndat(paths)

    fig, axes = plt.subplots(2,2)
    vibfig, vibaxes = plt.subplots(1,1)
    for i in range(dyn.Nalgorithm):
        # dynamic
        dyn_Nout = np.zeros(dyn.Nrecord)
        for j in range(dyn.Nrecord):
            dyn_Nout[j] = sum((1 if (k < -20 or k > 3) else 0
                                   for k in dyn.dyn_r[i,:,j,1]))

        final_E_dist = dyn.final_Ek_dist + dyn.final_Ep_dist

        # 
        final_Ex_dist_filt = final_E_dist[i,:,0][np.where(dyn.final_r_dist[i,:,1] < -20)]
        final_n_vib_dist = np.round((final_Ex_dist_filt / 0.008) - 0.5)

        axes[0,0].plot(dyn.tarr, dyn.dyn_ravg[i,:,0])
        axes[1,0].plot(dyn.tarr, dyn.dyn_ravg[i,:,1])
        axes[0,1].plot(dyn.tarr, dyn_Nout)

        ax = vibaxes
        ax.hist(final_n_vib_dist, bins=21, align='left', rwidth= 0.5, range=(0, 20), normed=True)
        ax.set_xlim(0, 20)
        ax.set_ylim(0, 0.5)

        ## -- save fig -- ##
        saveto=args.savedir
        if not saveto and not HAVE_DISPLAY:
            saveto = './'

        if saveto:
            if not args.savedir.endswith('/'):
                args.savedir += '/'
            fig.savefig(args.savedir + savename + '.png', dpi=2 * fig.dpi)

    if HAVE_DISPLAY:
        plt.show()


if __name__ == '__main__':
    checkdyn()


# END