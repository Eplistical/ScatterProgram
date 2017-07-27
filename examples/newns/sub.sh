#!/bin/bash
#PBS -N newns
#PBS -l walltime=100:00:00
#PBS -l mem=4GB
#PBS -l nodes=1:ppn=1

cd $PBS_O_WORKDIR
ROOT=/data/home/Eplistical/code/ScatterProgram
BIN=$ROOT/install/bin
SCRATCHDIR=`pwd`/scratch
mkdir -p $SCRATCHDIR

jobname=newns
jobtype=prepareinit     # possible choice: prepareinit, surface, preparedat, simulation
infile=${jobname}.in
base=${jobname}.${jobtype}
CMD=`$BIN/scatter_gen_cmd.py --infile ${infile} --outdir ${SCRATCHDIR} --base=${base} --jobtype ${jobtype} --nproc ${PBS_NP}`
echo $CWD
eval $CMD
mv $SCRATCHDIR/* .
