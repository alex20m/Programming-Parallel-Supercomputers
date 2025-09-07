#!/bin/bash -l
#### Job submission script example to a CPU queue.
#### (Remember for Slurm #SBATCH, one # is active, two or
#### more ## is commented out)
#SBATCH --account=project_462001048
#SBATCH --partition=debug
#SBATCH --nodes=2
#SBATCG --ntasks-per-node=1
#SBATCH --time=00:05:00
##SBATCH --output=prog.out #You can optionally name output, otherwise slurm.jobid

srun ./synchro.x
