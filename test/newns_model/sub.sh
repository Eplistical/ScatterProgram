#!/bin/bash
#PBS -N L('w')D
#PBS -l walltime=100:00:00
#PBS -l mem=100GB
#PBS -l nodes=1:ppn=48
cd $PBS_O_WORKDIR
ROOT=/data/home/Eplistical/code/ScatterProgram
SCRIPT=$ROOT/script
SCRATCHDIR=/scratch/Eplistical/${PBS_JOBID}

mkdir -p $SCRATCHDIR

if [ -f ".pbs.log" ]; then
	rm .pbs.log
fi

jobname=newns_model
jobtype=simulation
infile=${jobname}.in
base=${jobname}.${jobtype}

CMD=`$SCRIPT/main --infile ${infile} --outdir ${SCRATCHDIR} --base=${base} --jobtype ${jobtype} --nproc ${PBS_NP}`
echo running:
echo $CMD

eval $CMD
mv $SCRATCHDIR/* .
