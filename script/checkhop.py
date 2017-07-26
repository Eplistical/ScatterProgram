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


def checkhop():
    """function to plot hop"""
    args = parse_args()
    config = ScatterConfig()
    paths = ScatterPath(args.infile)
    hop = hopdat(paths, filt_dim=[1])

    fig, axes = plt.subplots(1,1)
    Egap = list()
    colors = list()
    lgd = list()
    for i in range(hop.Nalgorithm):
        print(i)
        if hop.hop_energy_gap[i] is not None:
            Egap.append(hop.hop_energy_gap[i])
            colors.append(config.colors[i])
            lgd.append(hop.algorithms[i])

    ax = axes
    ax.hist(Egap, color=colors,
            bins=30,
            align='left', rwidth= 0.8,
            range=(-0.12, 0.03), normed=False)
    ax.legend(lgd)

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
    checkhop()


# END
