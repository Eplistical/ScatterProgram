#!/usr/bin/env python3
import os
import numpy as np
from matplotlib import pyplot as plt
import subprocess


def goforward(f, start='', linenum=0):
    """move the cursor to the given line in a file
            Input:
                @f                    file object, file for search
                @start            string, the starting pattern of given line
                @linenum        int, line number to move forward
                if start & linenum both input, search the pattern first then
                move forward lines
            Return:
                #line                content of targetline
                #None                If error happens
    """
    # given start pattern
    if start.strip():
        while 1:
            try:
                line = next(f).strip()
                if line.startswith(start):
                    break
            except StopIteration:
                return None
    # move forward linenum lines
    if linenum > 0:
        for i in range(linenum-1):
            try:
                next(f)
            except StopIteration:
                return None
        try:
            line = next(f)
            return line
        except StopIteration:
            return None

    return line


def loadmatrix(f, start):
    """load matrix"""
    while True:
        try:
            line = next(f).strip()
            if line.startswith(start):
                break
        except StopIteration:
            return None

    # start reading
    data = []
    while True:
        try:
            line = next(f).strip()
            if line == "":
                continue
            s = [float(j) for j in line.split()]
            data.append(s)
        except StopIteration:
            break
        except ValueError:
            break
    return np.array(data)


# basic
FILE_DIR = os.path.dirname(os.path.abspath(__file__))

# global
surfaces = {
                'constant'  : "1",
                'step'      : "0        1       0",
                'harmonic'  : "0.896    0       0.2",
                'fermi'     : "1        0       0.5    -0.2",
                'gaussian'  : "1        1       2       0",
                'morse'     : "0.011   -0.011   0.64   -3.5",
                'newns'     : "1        0.2     0.67    0",
            }

surfmode = []
surfpara = []
for key, val in surfaces.items():
    surfmode.append(key)
    surfpara.append(val)

input_content = """
$rem
    jobname     test_surfaces_1d
    jobtype     surface
	loaddat		true
    DIM         1
	hbar		1
    kT          0.01
    Gamma0      0.03
$end

$grid
	rmin		-5
	rmax		5
	Nr			10000
$end

$simulation
    Ntraj       1
    mass        14000
    EndT        100
    Nstep       10000
    Anastep     100

	#-- init state relateda --#

    prepInit    false
    initTemp    0.5
    vibstate    0
	elestate	0
    initmode    delta
	initravg	0.4
	initpavg	0
$end

$surfaces
    surfnum     {SURFNUM}
	cutoff_gamma	0

    surfmode {SURFMODE}
	@surfpara
        {SURFPARA}
	@end

    gammamode	constant
	@gammapara
		1
	@end

	#-- fef related --#

	bandwidth	1.5
	derange		3e-5	3e-6
$end
""".format( SURFNUM=len(surfaces),
            SURFMODE="\t".join(surfmode),
            SURFPARA="\n\t\t".join(surfpara))


input_content = input_content[1:]


def test_surfaces_1d():
    """perform 1d surface test"""
    with open('test_surfaces_1d.in', 'w') as f:
        f.write(input_content)
    cmd = "main test_surfaces_1d.in"
    os.system(cmd)
    cmd = "scatter -i test_surfaces_1d.in > test_surfaces_1d.out"
    os.system(cmd)
    with open('test_surfaces_1d.out', 'r') as f:
        goforward(f, 'surfaces:')
        for i, mode in enumerate(surfmode):
            data = loadmatrix(f, start='i = ')
            plt.plot(data[:,0], data[:,1], label=mode)
    plt.ylim((-2,5))
    plt.legend()
    plt.show()
    cmd = "rm test_surfaces_1d.in test_surfaces_1d.in.json test_surfaces_1d.out"
    os.system(cmd)


if __name__ == "__main__":
    """main"""
    test_surfaces_1d();


# END
