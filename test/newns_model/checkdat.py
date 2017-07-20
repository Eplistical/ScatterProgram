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


def checkdat():
    args = parse_args()
    paths = ScatterPath(args.infile)

    with open(paths.jsonfile, 'r') as f:
        jsoninfo = json.load(f)

    dim = int(jsoninfo['rem']['dim']['value'][0])
    surfnum = int(jsoninfo['surfaces']['surfnum']['value'][0])

    datatype = np.dtype(
                [
                    ('dim', (UINT_T, 1)),
                    ('surfnum', (UINT_T, 1)),
                    ('kT', (DOUBLE_T, 1)),
                    ('Gamma0', (DOUBLE_T, 1)),
                    ('mass', (DOUBLE_T, dim)),
                    ('surfpara', (DOUBLE_T, 30)),
                ]
                )

    print(1)
    print(paths.datfile)
    info = np.fromfile(paths.datfile, dtype=datatype, count=1)
    assert dim == info['dim'][0]
    print(info)

checkdat()
