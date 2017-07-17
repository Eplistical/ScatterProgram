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


def checkinit():
    """main"""
    args = parse_args()
    paths = ScatterPath(args.infile)

    with open(paths.jsonfile, 'r') as f:
        jsoninfo = json.load(f)

    dim = int(jsoninfo['rem']['dim']['value'][0])
    Ntraj = int(jsoninfo['simulation']['ntraj']['value'][0])

    # datatype
    init_dt = np.dtype(
                [
                    ('dim', (UINT_T, 1)),
                    ('mass', (DOUBLE_T, dim)),
                    ('inittemp', (DOUBLE_T, 1)),
                    ('r0p0', (DOUBLE_T, (Ntraj, dim * 2))),
                ]
                )

    info = np.fromfile(paths.initfile, dtype=init_dt)
    assert dim == info['dim'][0]

    dim = info['dim'][0]
    mass = info['mass']
    inittemp = info['inittemp'][0]
    r0p0 = info['r0p0'][0]

    fig, axes = plt.subplots(dim, 2)
    for d in range(dim):
        ax = axes[d, 0] if dim > 1 else axes[0]
        ax.hist(r0p0[:, d], bins=60)
        ax = axes[d, 1] if dim > 1 else axes[1]
        ax.hist(r0p0[:, d + dim], bins=60)
    plt.show()

if __name__ == '__main__':
    checkinit()
