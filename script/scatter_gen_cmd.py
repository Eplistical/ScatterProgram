#!/usr/bin/env python3
import json
import os
import argparse
from pprint import pprint
from aux.qctojson import *
import logging
from datetime import datetime

FILE_DIR = os.path.dirname(os.path.abspath(__file__))
BIN_DIR = FILE_DIR

TIMEFMT = ' %Y-%m-%d %H:%M:%S'

def now():
    t = datetime.now()
    return t.strftime(TIMEFMT)


def parsearg():
    """parse commandline arguments
    """
    parser = argparse.ArgumentParser()
    parser.add_argument('--infile', required=True, type=str,
                            help='input file name')

    parser.add_argument('--jobtype', required=True, type=str,
                            help='jobtype, \
                            (surface, simulation, preparedat, prepareinit)')

    parser.add_argument('--outdir', type=str,
                            help='output directory')

    parser.add_argument('--nproc', type=int, default=1,
                            help='process number')

    parser.add_argument('--base', type=str,
                            help='out/log file basename')

    parser.add_argument('--cfgfile', type=str,
                            default=FILE_DIR + '/../config/remlist.cfg',
                            help='config file name')

    args = parser.parse_args()
    return args


def main():
    args = parsearg()
    infile = os.path.abspath(args.infile)
    infile_DIR = os.path.dirname(infile)
    jsonfile = infile + '.json'

    # parse infile, create json file
    dataframe = parse_config_file(args.cfgfile)
    jsondata = parse_infile(args.infile, dataframe)
    with open(jsonfile, 'w') as f:
        json.dump(jsondata, f, indent=4)

    # paths
    jobname = jsondata['rem']['jobname']['value'][0]
    initfile = infile_DIR + '/' + jobname + '.init'
    datfile = infile_DIR + '/' + jobname + '.fef.dat'
    logfile = infile_DIR + '/' + jobname + '.log'
    outfile = infile_DIR + '/' + jobname + '.out'

    # switcher
    jobtype = args.jobtype.lower()
    assert jobtype in ( 'simulation',
                        'surface',
                        'preparedat',
                        'prepareinit',
                        )
    nproc = args.nproc
    EXE = BIN_DIR + '/run_' + jobtype
    if nproc > 1 and jobtype in ('simulation', 'preparedat'):
        EXE = 'mpirun -n {} '.format(nproc) + EXE

    if not args.base:
        base = jobname + '.' + jobtype
    else:
        base = args.base

    if base == 'STDOUT':
        outfile = 'STDOUT'
        logfile = 'STDOUT'
    else:
        outfile = base + '.out'
        logfile = base + '.log'

    if not args.outdir:
        outdir = infile_DIR
    else:
        outdir = args.outdir

    command = "{EXE} -i {INFILE} -o {OUTFILE} -l {LOGFILE} -s {OUTDIR}".format(
                EXE=EXE,
                INFILE=infile,
                OUTFILE=outfile,
                LOGFILE=logfile,
                OUTDIR=outdir,
                )
    print(command)


if __name__ == '__main__':
    main()

# END
