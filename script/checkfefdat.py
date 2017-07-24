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
    parser.add_argument('--savedir', type=str, help='directory to save plots')
    return parser.parse_args()


def checkfefdat():
    """function to plot fef"""
    args = parse_args()
    paths = ScatterPath(args.infile)
    fef = fefdat(paths)
    for k in range(5000, 5500):
        for l in range(5000, 5500):
            assert(fef.efric[k,l,:].T == fef.efric[k,l,:]).all() 
            if k % 50 == 0 and l % 50 == 0:
                print(fef.efric[k,l,:])
    adsf

    rarr = tuple(np.linspace(a, b, c, endpoint=False)
                    for a, b, c in zip(fef.rmin, fef.rmax, fef.Nr))

    delta = 1e-10
    lim = {
            'force' : (-2, 2),
            'efric' : (-1e2, 1e2),
            'fBCME' : (-0.2, 0.2),
            }

    fignum = {
                'force' : fef.dim,
                'efric' : fef.dim**2,
                'fBCME' : fef.dim,
            }

    for name, Nfig in fignum.items():
        for i in range(Nfig):
            savename = '{}{}'.format(name, i)
            print('now processing ' + savename)
            data = getattr(fef, name)[:,:,i]
            levels = np.linspace(
                        max(np.min(data) - delta, lim[name][0]),
                        min(np.max(data) + delta, lim[name][1]),
                        100)

            fig, ax = plt.subplots(1, 1)
            cf = ax.contourf(rarr[0], rarr[1], data,
                                levels=levels, cmap=plt.cm.jet)
            plt.colorbar(cf)

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
    checkfefdat()

# END
