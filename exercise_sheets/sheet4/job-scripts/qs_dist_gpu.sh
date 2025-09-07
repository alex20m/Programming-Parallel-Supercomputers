#!/bin/bash -l
#SBATCH --account=project_462001048
#SBATCH --partition=dev-g
#### Standard parameters
#SBATCH --time=00:05:00
#### For a small MPI job:
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=2
####End of large MPI job.
#SBATCH --gpus-per-node=2
##SBATCH --output=prog.out

time srun ../build/quicksort-distributed-gpu

