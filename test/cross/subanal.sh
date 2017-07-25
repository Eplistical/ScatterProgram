#!/bin/bash
#PBS -N L('w')D
#PBS -l walltime=310:00:00
#PBS -l mem=20GB
#PBS -l nodes=1:ppn=1
cd $PBS_O_WORKDIR
BIN=/data/home/Eplistical/code/ScatterProgram/install/bin
SCRATCHDIR=/scratch/Eplistical/${PBS_JOBID}

mkdir -p $SCRATCHDIR

if [ -f ".pbs.log" ]; then
	rm .pbs.log
fi

jobname=cross
infile=${jobname}.in

../../script/checkfefdat.py ${infile} --savedir=.