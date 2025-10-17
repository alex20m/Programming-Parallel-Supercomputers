#!/bin/bash -l
#SBATCH --account=project_462001048
#SBATCH --partition=debug
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=4
#SBATCH --time=00:05:00
##BATCH --output=output_prog.out #You can optionally name output, otherwise slurm.jobid

srun ./adevc_program 100 100 2 1 10