#!/bin/bash -l
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --gpus=1
#SBATCH --time=00:05:00
##SBATCH --output=BA.out
#SBATCH --account=project_462001048
#SBATCH --partition=dev-g

time srun BA
