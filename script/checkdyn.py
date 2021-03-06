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

    fig, axes = plt.subplots(2,2)
    for i in range(dyn.Nalgorithm):
        print(i)
        # dynamic
        dyn_Nout = np.zeros(dyn.Nrecord)
        for j in range(dyn.Nrecord):
            dyn_Nout[j] = sum((1 if (k < -10 or k > 5) else 0
                                   for k in dyn.dyn_r[i,:,j,1]))

        axes[0,0].plot(dyn.tarr, dyn.dyn_ravg[i,:,0], config.colors[i])
        axes[1,0].plot(dyn.tarr, dyn.dyn_ravg[i,:,1], config.colors[i])
        axes[0,1].plot(dyn.tarr, dyn_Nout, config.colors[i])

    ## -- save fig -- ##
    saveto=args.savedir
    if not saveto and not HAVE_DISPLAY:
        saveto = './'

    if saveto:
        if not args.savedir.endswith('/'):
            args.savedir += '/'
        fig.savefig(args.savedir + savename + '.png', dpi=2*fig.dpi)

    if HAVE_DISPLAY:
        plt.show()


if __name__ == '__main__':
    checkdyn()


# END
