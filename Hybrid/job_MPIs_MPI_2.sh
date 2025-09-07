#!/bin/bash -l
#SBATCH --account=project_462001048
#SBATCH --partition=debug
#SBATCH --time=00:05:00
#### Number of nodes, number of MPI processes is nodes x ntasks
#SBATCH --nodes=2
#### Number of MPI processes per node
#SBATCH --ntasks-per-node=2
##SBATCH --output=prog.out

time srun MPIs_MPI_2
