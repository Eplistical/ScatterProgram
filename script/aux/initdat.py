#!/usr/bin/env python3
import numpy as np
from .types import *
from .ScatterPath import *
from .ScatterJson import *

class initdat(object):
    """class to load & parse .init data in the scatter program"""
    def __init__(self, paths):
        """init, paths must be a ScatterPath object"""
        assert isinstance(paths, ScatterPath)
        jsondata = ScatterJson(paths.jsondata)

        # load data from init file
        with open(paths.init_file, "r") as f:
            ## -- check header -- ##
            self.dim = np.fromfile(f, UINT_T, 1)[0]
            self.Ntraj = np.fromfile(f, UINT_T, 1)[0]
            self.inittemp = np.fromfile(f, DOUBLE_T, 1)[0]
            self.mass = np.fromfile(f, DOUBLE_T, self.dim)

            assert self.dim == jsondata.dim
            assert self.Ntraj == jsondata.Ntraj
            assert self.inittemp == jsondata.inittemp
            assert tuple(self.mass) == jsondata.mass

            ## -- load data -- ##
            self.r0p0 = np.fromfile(f, DOUBLE_T, self.Ntraj * self.dim * 2)
            self.r0p0 = self.r0p0.reshape((self.Ntraj, self.dim * 2))
            self.r0 = self.r0p0[:,:self.dim]
            self.p0 = self.r0p0[:,self.dim:]

# END
