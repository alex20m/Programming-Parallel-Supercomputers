#!/bin/bash -l
#SBATCH --account=project_462001048
#SBATCH --partition=debug
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=2
##SBATCH --output=p2p.out

srun ./p2p_program