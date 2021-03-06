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


def checksurf():
    """function to plot surf"""
    args = parse_args()
    paths = ScatterPath(args.infile)
    jsondata = ScatterJson(paths.jsondata)

    fig, axes = plt.subplots(jsondata.dim)

    with open(paths.surface_outfile, 'r') as f:
        goforward(f, '#####')
        for d in range(jsondata.dim):
            ax = axes[d] if jsondata.dim > 1 else axes
            for i in range(jsondata.surfnum):
                data = loadmatrix(f, start="d = ", printheader=False)
                ax.plot(data[:,0], data[:,1], label='d{}i{}'.format(d, i))

            ax.plot(data[:,0], data[:,3], '--', label='d{}g'.format(d))
            ax.set_xlim(jsondata.rmin[d], jsondata.rmax[d])
            # boundary
            ax.set_ylim(ax.get_ylim())
            ylim = ax.get_ylim()
            ax.plot(np.ones(2) * jsondata.boundary_rmin[d], ylim, '-k')
            ax.plot(np.ones(2) * jsondata.boundary_rmax[d], ylim, '-k')


    ## -- format plot -- ##
    plt.legend()

    ## -- save fig -- ##
    if not args.save and HAVE_DISPLAY:
        plt.show()
    else:
        saveto = 'surf.png' if not args.save else args.save
        fig.savefig(args.saveto, dpi=fig.dpi * 2)


if __name__ == '__main__':
    checksurf()


# END
