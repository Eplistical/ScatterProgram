#!/bin/bash
#PBS -N L('w')D
#PBS -l walltime=100:00:00
#PBS -l mem=60GB
#PBS -l nodes=2:ppn=20

cd $PBS_O_WORKDIR
ROOT=/data/home/Eplistical/code/ScatterProgram
BIN=$ROOT/install/bin
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

jobname=nontrap
nproc=1

<<EOF
jobtype=prepareinit
runcmd ${jobname} ${jobtype} ${nproc}

jobtype=surface
runcmd ${jobname} ${jobtype} ${nproc}
EOF

jobtype=simulation
nproc=${PBS_NP}
runcmd ${jobname} ${jobtype} ${nproc}
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

jobname=nontrap
jobtype=simulation
nproc=${PBS_NP}
runcmd ${jobname} ${jobtype} ${nproc}

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

jobname=nontrap
jobtype=simulation
nproc=${PBS_NP}
runcmd ${jobname} ${jobtype} ${nproc}

