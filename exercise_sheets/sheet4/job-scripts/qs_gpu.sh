#!/bin/bash -l
#SBATCH --account=project_462001048
#SBATCH --partition=dev-g
#### Standard parameters
#SBATCH --time=00:05:00
#SBATCH --nodes=1         #Use one node
#SBATCH --ntasks=1        #One task
#SBATCH --gpus-per-node=1
##SBATCH --output=prog.out


srun ../build/quicksort-gpu


