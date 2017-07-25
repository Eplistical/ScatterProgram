#!/usr/bin/env python3
import sys
import os
import argparse
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
    parser.add_argument('--save', type=str, help='save name')
    return parser.parse_args()


def checkinit():
    """function to plot init"""
    args = parse_args()
    paths = ScatterPath(args.infile)
    init = initdat(paths)

    fig, axes = plt.subplots(init.dim, 2)
    for d in range(init.dim):
        ax = axes[d, 0] if init.dim > 1 else axes[0]
        ax.hist(init.r0[:,d], bins=60)
        ax = axes[d, 1] if init.dim > 1 else axes[1]
        ax.hist(init.p0[:, d], bins=60)

    ## -- save fig -- ##
    if not args.save and HAVE_DISPLAY:
        plt.show()
    else:
        saveto = 'init.png' if not args.save else args.save
        fig.savefig(saveto, dpi=fig.dpi * 2)


if __name__ == '__main__':
    checkinit()


# END
