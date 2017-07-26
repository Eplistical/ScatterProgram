#!/bin/bash
#PBS -N L('w')D
#PBS -l walltime=310:00:00
#PBS -l mem=120GB
#PBS -l nodes=1:ppn=48
cd $PBS_O_WORKDIR
BIN=/data/home/Eplistical/code/ScatterProgram/install/bin
SCRATCHDIR=/scratch/Eplistical/${PBS_JOBID}

mkdir -p $SCRATCHDIR

if [ -f ".pbs.log" ]; then
	rm .pbs.log
fi

function runcmd() {
	# args: jobname, jobtype, nproc
	infile=${1}.in
	base=${1}.${2}
	echo ${3}
	CMD=`python3 $BIN/scatter_gen_cmd.py --infile ${infile} --outdir ${SCRATCHDIR} --base=${base} --jobtype ${2} --nproc ${3}`
	echo $CMD
	eval $CMD
	mv $SCRATCHDIR/* .
}


jobname=cross_G1
jobtype=preparedat
nproc=${PBS_NP}
runcmd ${jobname} ${jobtype} ${nproc}

