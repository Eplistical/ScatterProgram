#!/bin/bash
#PBS -N L('w')D
#PBS -l walltime=100:00:00
#PBS -l mem=120GB
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
infile=${jobname}.in

jobtype=prepareinit
base=${jobname}.${jobtype}
CMD=`$SCRIPT/main --infile ${infile} --outdir ${SCRATCHDIR} --base=${base} --jobtype ${jobtype}`
echo $CWD
eval $CMD
mv $SCRATCHDIR/* .

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

python3 checkdat.py ${infile}

<<EOF
jobtype=surface
base=${jobname}.${jobtype}
CMD=`$SCRIPT/main --infile ${infile} --outdir ${SCRATCHDIR} --base=${base} --jobtype ${jobtype} --nproc ${PBS_NP}`
echo $CWD
eval $CMD
mv $SCRATCHDIR/* .
EOF
