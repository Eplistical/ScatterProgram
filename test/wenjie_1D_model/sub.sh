#!/bin/bash
#PBS -N L('w')D
#PBS -l walltime=100:00:00
#PBS -l mem=4GB
#PBS -l nodes=1:ppn=4

cd $PBS_O_WORKDIR
ROOT=/data/home/Eplistical/code/ScatterProgram
BIN=$ROOT/build/bin
SCRIPT=$ROOT/script

jobname=wenjie1D

$SCRIPT/main ${jobname}.in

$BIN/run_surface -t surface -i ${jobname}.in
mv ${jobname}.out ${jobname}.surface.out
mv ${jobname}.log ${jobname}.surface.log

<< EOF

$BIN/run_prepareinit -t prepareinit -i ${jobname}.in
mv ${jobname}.out ${jobname}.prepareinit.out
mv ${jobname}.log ${jobname}.prepareinit.log

mpirun -n 4 $BIN/run_preparedat -t preparedat -i ${jobname}.in
mv ${jobname}.out ${jobname}.preparedat.out
mv ${jobname}.log ${jobname}.preparedat.log

mpirun -n 4 $BIN/run_simulation -t simulation -i ${jobname}.in
mv ${jobname}.out ${jobname}.simulation.out
mv ${jobname}.log ${jobname}.simulation.log

EOF
