#!/usr/bin/env python3
import sys
import os
import argparse
import json
import numpy as np
from matplotlib import pyplot as plt
from aux import *

np.set_printoptions(formatter={'float': '{: 0.3f}'.format})

def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('infile', type=str, help='<XXX>.in infile')
    return parser.parse_args()


def checkdyn():
    """main"""
    args = parse_args()
    paths = ScatterPath(args.infile)

    with open(paths.jsonfile, 'r') as f:
        jsoninfo = json.load(f)
    dim = int(jsoninfo['rem']['dim']['value'][0])
    Ntraj = int(jsoninfo['simulation']['ntraj']['value'][0])
    Nstep = int(jsoninfo['simulation']['nstep']['value'][0])
    Anastep = int(jsoninfo['simulation']['anastep']['value'][0])
    Nrecord = int(Nstep / Anastep);
    dt = float(jsoninfo['simulation']['endt']['value'][0]) / float(jsoninfo['simulation']['nstep']['value'][0])
    # info piece structure: (surf, r, p, Ek, Ep) 
    infopiece_size = dim * 4 + 1

    # data structure: 
    # | alg1traj1, alg1traj2, ..., alg1trajN | 
    # | alg2traj1, alg2traj2, ..., alg2trajN | 
    #     ... 

    Nalgorithm = 1
    single_traj_info_size = Nrecord * infopiece_size

    # datatype
    datatype = np.dtype(
                    [
                        ('dim', (UINT_T, 1)),
                        ('Ntraj', (UINT_T, 1)),
                        ('Nalgorithm', (UINT_T, 1)),
                        ('single_traj_info_size', (UINT_T, 1)),
                        ('mybatch', (UINT_T, Ntraj)),
                        ('data', (DOUBLE_T, Ntraj * single_traj_info_size * Nalgorithm)),
                    ]
                    )

    info = np.fromfile(paths.dyn_info_file, dtype=datatype, count=1)
    print(info['dim'][0], info['Ntraj'][0], info['Nalgorithm'][0],
            info['single_traj_info_size'][0])
    print(dim, Ntraj, Nalgorithm, single_traj_info_size)
    assert (dim == info['dim'][0])
    assert (Ntraj == info['Ntraj'][0])
    assert (Nalgorithm == info['Nalgorithm'][0])
    assert (single_traj_info_size == info['single_traj_info_size'][0])

    data = info['data'][0].reshape(Nalgorithm, Ntraj, Nrecord, infopiece_size)
    tarr = np.arange(Nrecord) * dt;

    fig, axes = plt.subplots(2,2)
    for i in range(Nalgorithm):
        dyninfo = data[i,:,:,:]
        finalinfo = dyninfo[:,-1,:]

        dyn_ravg = np.mean(dyninfo[:,:,0 * dim + 1:1 * dim + 1], 0)
        dyn_pavg = np.mean(dyninfo[:,:,1 * dim + 1:2 * dim + 1], 0)
        dyn_Epavg = np.mean(dyninfo[:,:,2 * dim + 1:3 * dim + 1], 0)
        dyn_Ekavg = np.mean(dyninfo[:,:,3 * dim + 1:4 * dim + 1], 0)

        dyn_Nout = np.zeros(Nrecord)
        for i in range(Nrecord):
            dyn_Nout[i] = sum((1 if (k < -20 or k > 3) else 0
                                for k in dyninfo[:,i,2]))

        final_r_dist = finalinfo[:,0 * dim + 1:1 * dim + 1]
        final_p_dist = finalinfo[:,1 * dim + 1:2 * dim + 1]
        final_Ep_dist = finalinfo[:,2 * dim + 1:3 * dim + 1]
        final_Ek_dist = finalinfo[:,3 * dim + 1:4 * dim + 1]

        axes[0,0].plot(tarr, dyn_ravg[:,0])
        axes[1,0].plot(tarr, dyn_ravg[:,1])
        axes[0,1].plot(tarr, dyn_Nout)


    plt.show()



if __name__ == '__main__':
    checkdyn()
