#!/usr/bin/env python3
import sys
import os
import argparse
import json
from pprint import pprint
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
    config = ScatterConfig()
    paths = ScatterPath(args.infile)
    dyn = dyndat(paths)

    fig, axes = plt.subplots(1,1)
    nvib = list()
    for i in range(dyn.Nalgorithm):
        print(i)
        final_E_dist = dyn.final_Ek_dist + dyn.final_Ep_dist
        final_Ex_dist_filt = \
            final_E_dist[i,:,0][np.where(np.logical_or(
                                    dyn.final_r_dist[i,:,1] < dyn.boundary_rmin[1],
                                    dyn.final_r_dist[i,:,1] > dyn.boundary_rmax[1])
                                    )]
        final_n_vib_dist = np.round((final_Ex_dist_filt / 0.008) - 0.5)
        nvib.append(final_n_vib_dist)

    ax = axes
    ax.hist(nvib, color=config.colors[:dyn.Nalgorithm],
            bins=tuple(range(0,21,1)),
            align='left', rwidth= 0.8,
            range=(0, 21), normed=True)
    ax.legend(dyn.algorithms)
    ax.set_xlim(0, 20)
    ax.set_xticks([i for i in range(0,21,1)])
    ax.set_xticklabels([str(int(i)) for i in range(0,21,1)])
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
