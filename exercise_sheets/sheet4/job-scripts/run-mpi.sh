#!/bin/bash
#SBATCH --gpus-per-node=2
#SBATCH -t 00:00:59

#SBATCH --ntasks-per-node=2
#SBATCH --nodes=2

#SBATCH --account=project_462001048
#SBATCH --partition=dev-g

srun ../build/reduce-mpi reduce-mpi.result
