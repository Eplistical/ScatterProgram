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


def adhoc():
    """adhoc"""
    args = parse_args()
    paths = ScatterPath(args.infile)
    fef = fefdat(paths, rawdata=True)
    Gamma=8e-2
    data = np.array([
       2.00000000e+00,   1.00000000e+00,   2.00000000e-01,  -6.70000000e-01,
       0.00000000e+00,   1.00000000e+00,   2.00000000e-01,   6.70000000e-01,
      -3.20000000e+00,   5.00000000e-01,  -1.60000000e+00,   8.00000000e-03,
       8.00000000e-03,   1.40000000e+04,   2.00000000e-01,   1.00000000e-03,
       5.50000000e+04,   Gamma,           -3.00000000e+00,   3.00000000e+00,
       9.00000000e+03,  -1.20000000e+01,   8.00000000e+00,   1.50000000e+04,
       ] + fef.force.tolist() + fef.efric.tolist() + fef.fBCME.tolist())
    data.tofile('aaa')


if __name__ == '__main__':
    adhoc()

# END
