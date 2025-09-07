#!/bin/bash -l
#SBATCH --time=00:05:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
##SBATCH --output=vecAdd_CPU.out
#SBATCH --account=project_462001048
#SBATCH --partition=debug

time srun vecAdd_CPU
