#!/bin/bash
#SBATCH --gpus-per-node=4
#SBATCH -t 00:00:59
#SBATCH --ntasks=1
#SBATCH --nodes=1
#SBATCH --account=project_462001048
#SBATCH --partition=dev-g

srun ../build/reduce-multi reduce-multi.result
