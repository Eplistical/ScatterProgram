#!/usr/bin/env python3
import numpy as np
import matplotlib as mpl
from matplotlib import pyplot as plt

fname = 'test1d.dyn.dat'

with open(fname, 'rb') as f:
    tmp = np.fromfile(f, np.uint32, 4)

    Ntraj = tmp[0]
    dim = tmp[1]
    Nalgorithm = tmp[2]
    single_traj_info_size = tmp[3]

    job_info = np.fromfile(f, np.uint32, Ntraj)
    dyn = list()
    for alg in range(Nalgorithm):
        data = np.fromfile(f, np.float64, single_traj_info_size * Ntraj)
        data = data.reshape(Ntraj, single_traj_info_size / (dim * 2 + 1), dim * 2 + 1)
        dyn.append(data)

print(tmp)
print(job_info)

print(dyn[0][0])
t = np.arange(dyn[0][0].shape[0])
plt.plot(t, dyn[0][0][:,1], t, dyn[0][0][:,2])
plt.show()
