#!/bin/bash -l
#SBATCH --job-name=examplejob 
#SBATCH --output=examplejob.o%j  # Name of stdout output file
#SBATCH --error=examplejob.e%j   # Name of stderr error file
#SBATCH --partition=small        
#SBATCH --nodes=2                # Total number of nodes 
#SBATCH --ntasks-per-node=16     # Number of mpi tasks per node
#SBATCH --cpus-per-task=8        # Number of cores (threads) per task
#SBATCH --time=00:10:00          # Run time (d-hh:mm:ss)
#SBATCH --account=project_<id>   # Project for billing

# Set the number of threads based on --cpus-per-task
export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK

# Launch MPI code 
srun ./your_application # Use srun instead of mpirun or mpiexec