#!/bin/bash
#SBATCH --job-name=example
#SBATCH --account=project_<id>    # Project ID (for billing)
#SBATCH --partition=small-g 
#SBATCH --gpus-per-node=1         # Amount of GPUs (per node)
#SBATCH --nodes=1                 # Number of nodes
#SBATCH --ntasks=1                # Number of MPI tasks
#SBATCH --time=00:05:00

srun ./my_gpu_exe