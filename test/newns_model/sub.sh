#!/bin/bash
#PBS -N L('w')D
#PBS -l walltime=100:00:00
#PBS -l mem=10GB
#PBS -l nodes=1:ppn=1
cd $PBS_O_WORKDIR
ROOT=/data/home/Eplistical/code/ScatterProgram
SCRIPT=$ROOT/script
SCRATCHDIR=/scratch/Eplistical/${PBS_JOBID}

mkdir -p $SCRATCHDIR

if [ -f ".pbs.log" ]; then
	rm .pbs.log
fi

jobname=newns_model
infile=${jobname}.in

python3 checkdat.py ${infile}

<<EOF
jobtype=preparedat
base=${jobname}.${jobtype}
CMD=`$SCRIPT/main --infile ${infile} --outdir ${SCRATCHDIR} --base=${base} --jobtype ${jobtype} --nproc ${PBS_NP}`
echo $CWD
eval $CMD
mv $SCRATCHDIR/* .

jobtype=simulation
base=${jobname}.${jobtype}
CMD=`$SCRIPT/main --infile ${infile} --outdir ${SCRATCHDIR} --base=${base} --jobtype ${jobtype} --nproc ${PBS_NP}`
echo $CWD
eval $CMD
mv $SCRATCHDIR/* .

jobtype=surface
base=${jobname}.${jobtype}
CMD=`$SCRIPT/main --infile ${infile} --outdir ${SCRATCHDIR} --base=${base} --jobtype ${jobtype} --nproc ${PBS_NP}`
echo $CWD
eval $CMD
mv $SCRATCHDIR/* .
EOF
