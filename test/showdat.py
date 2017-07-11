#!/usr/bin/env python3
import numpy as np

fname = '.test2d.dat'

with open(fname, 'rb') as f:
    tmp = np.fromfile(f, np.int64, 2)
print(tmp)

