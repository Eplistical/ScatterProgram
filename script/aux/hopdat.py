#!/usr/bin/env python3
import numpy as np
from .types import *
from .ScatterPath import *
from .ScatterJson import *
from .dyndat import *

class hopdat(object):
    """class to load & parse .dyn.dat data in the scatter program"""
    def __init__(self, paths, filt_dim=None):
        """init, paths must be a ScatterPath object"""
        assert isinstance(paths, ScatterPath)
        jsondata = ScatterJson(paths.jsondata)
        if filt_dim:
            dyn = dyndat(paths)

        # load data in hop.dat file
        a = np.fromfile(paths.hop_file, UINT_T, 10)
        with open(paths.hop_file, "r") as f:
            ## -- check header -- ##
            self.dim = np.fromfile(f, UINT_T, 1)[0]
            self.Ntraj = np.fromfile(f, UINT_T, 1)[0]
            self.Nalgorithm = np.fromfile(f, UINT_T, 1)[0]

            assert self.dim == jsondata.dim
            assert self.Ntraj == jsondata.Ntraj
            assert self.Nalgorithm == len(jsondata.algorithms)

            ## -- load data -- ##
            self.infopiece_size = np.fromfile(f, UINT_T, 1)[0]
            self.Nhops = np.fromfile(f, UINT_T, self.Nalgorithm)

            self.hop_data = list()
            self.algorithms = jsondata.algorithms
            for i, N in enumerate(self.Nhops):
                if N == 0:
                    self.hop_data.append(None)
                else:
                    tmp = np.fromfile(f, DOUBLE_T, self.infopiece_size * N)
                    tmp = tmp.reshape(N, self.infopiece_size)
                    trajID = UINT_T(tmp[:,0])
                    idx = list()
                    if filt_dim:
                        for k, ID in enumerate(trajID):
                            valid = True
                            for d in filt_dim:
                                # traj in boundary at the end
                                if (dyn.final_r_dist[i, ID, d] < dyn.boundary_rmax[d]) and (dyn.final_r_dist[i, ID, d] > dyn.boundary_rmin[d]):
                                    valid = False
                                    break
                            if valid:
                                idx.append(k)
                    self.hop_data.append(tmp[idx, :])

        ## -- extract from data -- ##
        self.hop_trajID = [None if k is None else UINT_T(k[:,0]) for k in self.hop_data]
        self.hop_from   = [None if k is None else UINT_T(k[:,1]) for k in self.hop_data]
        self.hop_to     = [None if k is None else UINT_T(k[:,2]) for k in self.hop_data]
        self.hop_r      = [None if k is None else k[:,3:3+self.dim] for k in self.hop_data]
        self.hop_p      = [None if k is None else k[:,3+self.dim:3+self.dim*2] for k in self.hop_data]
        self.hop_gamma  = [None if k is None else k[:,-2] for k in self.hop_data]
        self.hop_energy_gap  = [None if k is None else k[:,-1] for k in self.hop_data]

# END
