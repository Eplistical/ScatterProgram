#!/bin/bash
#PBS -N L('w')D
#PBS -l walltime=310:00:00
#PBS -l mem=120GB
#PBS -l nodes=2:ppn=48
cd $PBS_O_WORKDIR
BIN=/data/home/Eplistical/code/ScatterProgram/install/bin
SCRATCHDIR=/scratch/Eplistical/${PBS_JOBID}

mkdir -p $SCRATCHDIR

if [ -f ".pbs.log" ]; then
	rm .pbs.log
fi

jobname=nontrap
infile=${jobname}.in

jobtype=preparedat
base=${jobname}.${jobtype}
CMD=`python3 $BIN/scatter_gen_cmd.py --infile ${infile} --outdir ${SCRATCHDIR} --base=${base} --jobtype ${jobtype} --nproc ${PBS_NP}`
echo $CWD
eval $CMD
mv $SCRATCHDIR/* .

<< EOF
jobtype=simulation
base=${jobname}.${jobtype}
CMD=`python3 $BIN/scatter_gen_cmd.py --infile ${infile} --outdir ${SCRATCHDIR} --base=${base} --jobtype ${jobtype} --nproc ${PBS_NP}`
echo $CWD
eval $CMD
mv $SCRATCHDIR/* .
EOF
