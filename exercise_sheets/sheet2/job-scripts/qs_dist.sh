#!/bin/bash -l
#SBATCH --account=project_462001048
#SBATCH --partition=dev-g
#### Standard parameters
#SBATCH --time=00:05:00
#### For a large MPI job:
#SBATCH --nodes=2         #Use two nodes
#SBATCH --ntasks-per-node=4  #Altogether eight tasks, evenly distributed over nodes
####End of large MPI job.
##SBATCH --output=prog.out

srun ../build/quicksort-distributed


