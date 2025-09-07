#!/bin/bash -l
#SBATCH --nodes=2  #Number of nodes, maximally 2
#SBATCH --ntasks-per-node=4 #Number of MPI ranks
#SBATCH --gpus-per-node=4 #Number of GPUs
#SBATCH -t 00:00:59
#SBATCH --account=project_462001048
#SBATCH --partition=dev-g

time srun HC
