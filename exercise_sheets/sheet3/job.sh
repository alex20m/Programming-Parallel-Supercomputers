#!/bin/bash -l
#### Job submission script example to a CPU queue using hybrid programming
#### model
#### (Remember for Slurm #SBATCH, one # is active, two or
#### more ## is commented out)
####
#### If you have permanent Triton account (not made just for this
#### course), you can comment this out (comment = two `##`) and
#### maybe you will have shorter queue time.
#SBATCH --account=project_462001048
#SBATCH --partition=debug
#### General resource parameters:
#SBATCH --time=00:10:00
#### Number of nodes, number of MPI processes is nodes x ntasks
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=4
#SBATCH --cpus-per-task=1
####Specify output file, otherwise slurm-<jobid>.out generated
##SBATCH --output=DE.out
####Special resource allocation, do not use unless instructed

export EBU_USER_PREFIX=/projappl/project_462001048/EB/
export SCOREP_EXPERIMENT_DIRECTORY=scorep_experiment_${SLURM_JOBID}

ml LUMI/24.03
ml partition/C
ml Score-P/9.0-cpeGNU-24.03
ml Scalasca/2.6.2-cpeGNU-24.03

export SCOREP_TOTAL_MEMORY=100M
export SCAN_TRACE_FILESYS=${PWD}${SCAN_TRACE_FILESYS:+:${SCAN_TRACE_FILESYS}}


rm -f HEAT_RESTART.dat
export SRUN_CPUS_PER_TASK=$SLURM_CPUS_PER_TASK
time scalasca -analyze srun heat_mpi 4000 4000 1000

