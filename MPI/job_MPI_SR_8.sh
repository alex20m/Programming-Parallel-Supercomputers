#!/bin/bash -l
#SBATCH --account=project_462001048
#SBATCH --partition=debug
#SBATCH --time=00:05:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=2
#SBATCH --output=MPI_SR_8.out

time srun SR8
