#!/usr/bin/env python3
import numpy as np
from .types import *
from .ScatterPath import *
from .ScatterJson import *

class dyndat(object):
    """class to load & parse .dyn.dat data in the scatter program"""
    def __init__(self, paths):
        """init, paths must be a ScatterPath object"""
        assert isinstance(paths, ScatterPath)
        jsondata = ScatterJson(paths.jsondata)

        # load data in dyn.dat file
        with open(paths.dyn_file, "r") as f:
            ## -- check header -- ##
            self.dim = np.fromfile(f, UINT_T, 1)[0]
            self.Ntraj = np.fromfile(f, UINT_T, 1)[0]
            self.Nalgorithm = np.fromfile(f, UINT_T, 1)[0]

            assert self.dim == jsondata.dim
            assert self.Ntraj == jsondata.Ntraj
            assert self.Nalgorithm == len(jsondata.algorithms)

            ## -- load data -- ##
            self.Nrecord = UINT_T(jsondata.Nstep / jsondata.Anastep)
            self.infopiece_size = np.fromfile(f, UINT_T, 1)[0]
            self.batch = np.fromfile(f, UINT_T, self.Ntraj)
            data = np.fromfile(f, DOUBLE_T, self.Ntraj * self.Nrecord * self.infopiece_size * self.Nalgorithm)
            data = data.reshape(self.Nalgorithm, self.Ntraj, self.Nrecord, self.infopiece_size)

        ## -- extract from data -- ##
        self.algorithms = jsondata.algorithms
        self.dyn_r  = data[:,:,:,0 * self.dim + 1 : 1 * self.dim + 1]
        self.dyn_p  = data[:,:,:,1 * self.dim + 1 : 2 * self.dim + 1]
        self.dyn_Ep = data[:,:,:,2 * self.dim + 1 : 3 * self.dim + 1]
        self.dyn_Ek = data[:,:,:,3 * self.dim + 1 : 4 * self.dim + 1]

        # dynamic
        self.dyn_ravg  = np.mean(self.dyn_r, 1)
        self.dyn_pavg  = np.mean(self.dyn_p, 1)
        self.dyn_Epavg = np.mean(self.dyn_Ep, 1)
        self.dyn_Ekavg = np.mean(self.dyn_Ek, 1)

        # final
        self.final_r_dist = self.dyn_r[:,:,-1,:]
        self.final_p_dist = self.dyn_p[:,:,-1,:]
        self.final_Ep_dist = self.dyn_Ep[:,:,-1,:]
        self.final_Ek_dist = self.dyn_Ek[:,:,-1,:]

        # other info
        self.EndT = jsondata.EndT
        self.Nstep = jsondata.Nstep
        self.dt = self.EndT / self.Nstep
        self.tarr = np.arange(self.Nrecord) * self.dt
        self.boundary_rmin = jsondata.boundary_rmin
        self.boundary_rmax = jsondata.boundary_rmax


# END
