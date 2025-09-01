#!/bin/bash -l
#### For multi-GPU jobs, in --gpus-per-node N, choose N>1. Single GPU, N==1.
#SBATCH --account courses
#SBATCH --partition courses
#### Standard parameters
#SBATCH --gpus-per-node=2 
#SBATCH --time=00:05:00
#SBATCH --nodes=1             #Single node, for more nodes, increase
#SBATCH --ntasks-per-node=2   #Same as number of GPUs
##SBATCH --output=GPUcode.out

module load gcc openmpi cuda cmake

nvcc -o GPUcode GPUcode.cu -lmpi

time srun GPUcode
