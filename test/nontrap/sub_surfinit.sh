#!/bin/bash
#PBS -N L('w')D
#PBS -l walltime=310:00:00
#PBS -l mem=10GB
#PBS -l nodes=1:ppn=1
cd $PBS_O_WORKDIR
SCRATCHDIR=/scratch/Eplistical/${PBS_JOBID}
mkdir -p $SCRATCHDIR
if [ -f ".pbs.log" ]; then
	rm .pbs.log
fi

BIN=/data/home/Eplistical/code/ScatterProgram/install/bin

jobname=nontrap
infile=${jobname}.in

jobtype=surface
base=${jobname}.${jobtype}
CMD=`python3 $BIN/scatter_gen_cmd.py --infile ${infile} --outdir ${SCRATCHDIR} --base=${base} --jobtype ${jobtype}`
echo $CWD
eval $CMD
mv $SCRATCHDIR/* .

jobtype=prepareinit
base=${jobname}.${jobtype}
CMD=`python3 $BIN/scatter_gen_cmd.py --infile ${infile} --outdir ${SCRATCHDIR} --base=${base} --jobtype ${jobtype}`
echo $CWD
eval $CMD
mv $SCRATCHDIR/* .

python3 $BIN/checksurf.py ${infile} --save img/init.png
python3 $BIN/checkinit.py ${infile} --save img/surf.png
