#!/usr/bin/env python3
import numpy as np
from .types import *
from .ScatterPath import *
from .ScatterJson import *

class fefdat(object):
    """class to load & parse .fef.dat data in the scatter program"""
    def __init__(self, paths, rawdata=False):
        """init, paths must be a ScatterPath object"""
        assert isinstance(paths, ScatterPath)
        jsondata = ScatterJson(paths.jsondata)

        # load data in fef.dat file
        with open(paths.fef_file, "r") as f:
            ## -- check header -- ##

            # basic para
            self.dim = np.fromfile(f, UINT_T, 1)[0]
            self.surfnum = np.fromfile(f, UINT_T, 1)[0]
            self.kT = np.fromfile(f, DOUBLE_T, 1)[0]
            self.Gamma0 = np.fromfile(f, DOUBLE_T, 1)[0]
            self.mass = np.fromfile(f, DOUBLE_T, self.dim)

            assert self.dim == jsondata.dim
            assert self.surfnum == jsondata.surfnum
            assert self.kT == jsondata.kT
            assert self.Gamma0 == jsondata.Gamma0
            assert tuple(self.mass) == jsondata.mass

            # surf & grid
            Ngammapara = sum(len(k) for k in jsondata.gammapara)
            Nsurfpara = sum(len(k) for k in jsondata.surfpara)
            self.gammapara = np.fromfile(f, DOUBLE_T, Ngammapara)
            self.surfpara = np.fromfile(f, DOUBLE_T, Nsurfpara)
            self.rmin = np.fromfile(f, DOUBLE_T, self.dim)
            self.rmax = np.fromfile(f, DOUBLE_T, self.dim)
            self.dr = np.fromfile(f, DOUBLE_T, self.dim)
            self.Nr = UINT_T((self.rmax - self.rmin) / self.dr)

            tmp = tuple()
            for k in jsondata.gammapara:
                tmp += k
            assert tuple(self.gammapara) == tmp

            tmp = tuple()
            for k in jsondata.surfpara:
                tmp += k
            assert tuple(self.surfpara) == tmp

            assert tuple(self.rmin) == jsondata.rmin
            assert tuple(self.rmax) == jsondata.rmax
            assert tuple(self.Nr) == jsondata.Nr

            ## -- load data -- ##
            Ntot = 1
            for k in jsondata.Nr:
                Ntot *= k

            self.force = np.fromfile(f, DOUBLE_T, self.dim * Ntot)
            self.efric = np.fromfile(f, DOUBLE_T, self.dim**2 * Ntot)
            self.fBCME = np.fromfile(f, DOUBLE_T, self.dim * Ntot)

        ## -- reshape -- ##
        if not rawdata:
            forceshape = [self.dim] + self.Nr.tolist()
            forceshape.reverse()
            efricshape = [self.dim**2] + self.Nr.tolist()
            efricshape.reverse()
            fBCMEshape = [self.dim] + self.Nr.tolist()
            fBCMEshape.reverse()

            self.force = self.force.reshape(forceshape)
            self.efric = self.efric.reshape(efricshape)
            self.fBCME = self.fBCME.reshape(fBCMEshape)

# END
