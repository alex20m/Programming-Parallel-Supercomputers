#!/bin/bash -l
#### Job submission script example.
#### (Remember for Slurm #SBATCH, one # is active, two or
#### more ## is commented out)
#SBATCH --account=project_462001048
#SBATCH --partition=dev-g
#### Standard parameters
#SBATCH --time=00:05:00
#### For a small MPI job:
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
##SBATCH --output=prog.out

srun ../build/quicksort-serial


