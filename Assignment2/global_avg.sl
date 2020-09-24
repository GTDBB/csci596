#!/bin/bash
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=4
#SBATCH --time=00:00:59
#SBATCH --output=global_avg.out
#SBATCH -A anakano_429

mpirun -n $SLURM_NTASKS ./global_avg
mpirun -n             4 ./global_avg