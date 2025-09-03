#!/bin/bash -l
#SBATCH --job-name=examplejob
#SBATCH --output=examplejob.o%j  # Name of stdout output file
#SBATCH --error=examplejob.e%j   # Name of stderr error file
#SBATCH --partition=small
#SBATCH --nodes=2                # Number of nodes
#SBATCH --ntasks=256             # Total number of mpi tasks
#SBATCH --mem=0                  # Allocate all the memory on each node
#SBATCH --time=00:10:00          # Run time (d-hh:mm:ss)
#SBATCH --account=project_<id>   # Project for billing

# Launch MPI code 
srun ./your_application # Use srun instead of mpirun or mpiexec