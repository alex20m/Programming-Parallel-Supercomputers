#!/bin/bash -l
#### Job submission script example to a CPU queue using hybrid programming model
#### (Remember for Slurm #SBATCH, one # is active, two or
#### more ## is commented out)
#SBATCH --account courses
#SBATCH --partition courses
#### Standard parameters
#SBATCH --time=00:05:00
#SBATCH --mem-per-cpu=4000
#### Number of nodes
#SBATCH --nodes=1
#### Number of MPI processes per node
#SBATCH --ntasks-per-node=2
#### Number of openMP threads per MPI process
#SBATCH --cpus-per-task=4
##SBATCH --output=prog.out

export OMP_PROC_BIND=TRUE
export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK
module purge   # unload all current modules
module load gcc openmpi

mpicc -fopenmp -o prog prog.c

time srun prog
