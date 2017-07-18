#!/bin/bash
#PBS -N L('w')D
#PBS -l walltime=100:00:00
#PBS -l mem=200GB
#PBS -l nodes=2:ppn=10

cd $PBS_O_WORKDIR
ROOT=/data/home/Eplistical/code/ScatterProgram
BIN=$ROOT/build/bin
SCRIPT=$ROOT/script

SCRATCHDIR=/scratch/Eplistical/${PBS_JOBID}
mkdir -p $SCRATCHDIR

if [ -f ".pbs.log" ]; then
	rm .pbs.log
fi

jobname=newns_model

$SCRIPT/main ${jobname}.in

base=${jobname}.simulation
mpirun -n 20 $BIN/run_simulation -t simulation -i ${jobname}.in #-s $SCRATCHDIR -o ${base}.out -l ${base}.log
#mv $SCRATCHDIR/${base}.out .
#mv $SCRATCHDIR/${base}.log .

<<EOF
base=${jobname}.surface
$BIN/run_surface -t surface -i ${jobname}.in -s $SCRATCHDIR -o ${base}.out -l ${base}.log
mv $SCRATCHDIR/${base}.out .
mv $SCRATCHDIR/${base}.log .

base=${jobname}.prepareinit
$BIN/run_surface -t preparinit -i ${jobname}.in -s $SCRATCHDIR -o ${base}.out -l ${base}.log
mv $SCRATCHDIR/${base}.out .
mv $SCRATCHDIR/${base}.log .

base=${jobname}.preparedat
mpirun -n 96 $BIN/run_preparedat -t preparedat -i ${jobname}.in -s $SCRATCHDIR -o ${base}.out -l ${base}.log
mv $SCRATCHDIR/${base}.out .
mv $SCRATCHDIR/${base}.log .

EOF
