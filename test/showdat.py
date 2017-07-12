#!/usr/bin/env python3
import numpy as np

fname = 'test2d.final.dat'

with open(fname, 'rb') as f:
    tmp = np.fromfile(f, np.uint32, 2)
    Ntraj = tmp[0]
    dim = tmp[1]
    job_info = np.fromfile(f, np.uint32, Ntraj)
    surf_info = np.fromfile(f, np.uint32, Ntraj)
    r_info = np.fromfile(f, np.float64, Ntraj * dim)
    r_info = r_info.reshape(Ntraj, dim)
    p_info = np.fromfile(f, np.float64, Ntraj * dim)
    p_info = p_info.reshape(Ntraj, dim)

print(tmp)
print(job_info)
print(surf_info)
print(r_info)
print(p_info)

