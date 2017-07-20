#!/bin/bash
#PBS -N L('w')D
#PBS -l walltime=100:00:00
<<<<<<< Updated upstream
#PBS -l mem=120GB
=======
#PBS -l mem=100GB
>>>>>>> Stashed changes
#PBS -l nodes=1:ppn=48

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

<<<<<<< Updated upstream
base=${jobname}
mpirun -n 48 $BIN/run_simulation -t simulation -i ${jobname}.in -s $SCRATCHDIR -o ${base}.out -l ${base}.log
=======
base=${jobname}.simulation
mpirun -n 48 $BIN/run_simulation -i ${jobname}.in -s $SCRATCHDIR -o ${base}.out -l ${base}.log
>>>>>>> Stashed changes
mv $SCRATCHDIR/${jobname}.dyn_info.dat .
mv $SCRATCHDIR/${base}.out .
mv $SCRATCHDIR/${base}.log .


<<EOF
base=${jobname}.preparedat
<<<<<<< Updated upstream
mpirun -n 96 $BIN/run_preparedat -t preparedat -i ${jobname}.in -s $SCRATCHDIR -o ${base}.out -l ${base}.log
=======
mpirun -n 96 $BIN/run_preparedat -i ${jobname}.in -s $SCRATCHDIR -o ${base}.out -l ${base}.log
>>>>>>> Stashed changes
mv $SCRATCHDIR/${base}.out .
mv $SCRATCHDIR/${base}.log .
mv $SCRATCHDIR/.${jobname}.dat .


base=${jobname}.surface
$BIN/run_surface  -i ${jobname}.in -s $SCRATCHDIR -o ${base}.out -l ${base}.log
mv $SCRATCHDIR/${base}.out .
mv $SCRATCHDIR/${base}.log .

base=${jobname}.prepareinit
$BIN/run_surface -i ${jobname}.in -s $SCRATCHDIR -o ${base}.out -l ${base}.log
mv $SCRATCHDIR/${base}.out .
mv $SCRATCHDIR/${base}.log .

EOF
