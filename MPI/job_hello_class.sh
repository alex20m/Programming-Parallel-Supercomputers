#!/bin/bash -l
#SBATCH --account=project_462001048
#SBATCH --partition=debug
#SBATCH --nodes=2
###What is the difference if you use here --ntasks=8?
#SBATCH --ntasks-per-node=8
#SBATCH --time=00:05:00
##SBATCH --output=prog.out #You can optionally name output, otherwise slurm.jobid

time srun hello_class
