#!/bin/bash -l
#### Job submission script example to a CPU queue.
#### (Remember for Slurm #SBATCH, one # is active, two or
#### more ## is commented out)
####
#SBATCH --account=courses
#SBATCH --partition=courses
#### Small MPI job
#SBATCH --ntasks=4
#SBATCH --mem-per-cpu=4000
#SBATCH --time=00:05:00
#### For a large MPI job:
##SBATCH --exclusive       #Allocate whole node
##SBATCH --nodes=2         #Use two nodes
###SBATCH --ntasks=4      #Total amount of tasks will be ntasks mapped to the requested number of nodes
##SBATCH --ntasks-per-node=4 #Total amount of tasks will be nodes*ntasks-per-node.
####End of large MPI job.
##SBATCH --output=prog.out #You can optionally name output, otherwise slurm.jobid

module purge   # unload all current modules
#Load MPI module
module load openmpi

mpicc -o prog prog.c

time srun prog
