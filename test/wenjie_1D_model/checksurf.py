#!/usr/bin/env python3
import sys
import os
import argparse
import json
import numpy as np
from matplotlib import pyplot as plt
from aux import *

def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('infile', type=str, help='<XXX>.in infile')
    return parser.parse_args()


def checksurf():
    """main"""
    args = parse_args()
    paths = ScatterPath(args.infile)

    with open(paths.jsonfile, 'r') as f:
        jsoninfo = json.load(f)
    dim = int(jsoninfo['rem']['dim']['value'][0])
    surfnum = int(jsoninfo['surfaces']['surfnum']['value'][0])

    fig, axes = plt.subplots(dim)

    with open(paths.surface_outfile, 'r') as f:
        for d in range(dim):
            ax = axes[d] if dim > 1 else axes
            for i in range(surfnum):
                data = loadmatrix(f, start="d = ", printheader=False)
                ax.plot(data[:,0], data[:,1], label='d{}i{}'.format(d, i))
    plt.legend()
    plt.show()

if __name__ == '__main__':
    checksurf()
