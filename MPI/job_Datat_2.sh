#!/bin/bash -l
#### Job submission script example to a CPU queue.
#### (Remember for Slurm #SBATCH, one # is active, two or
#### more ## is commented out)
#SBATCH --account=courses
#SBATCH --partition=courses-cpu
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=2
#SBATCH --mem-per-cpu=4000
#SBATCH --time=00:05:00
#SBATCH --output=prog.out #You can optionally name output, otherwise slurm.jobid

module load gcc openmpi

mpicc -o Datat_2 Datat_2.c

time srun Datat_2
