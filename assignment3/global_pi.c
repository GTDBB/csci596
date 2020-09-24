#include "mpi.h"
#include <stdio.h>
#define NBIN 1000000000

int nprocs;  /* Number of processes */
int myid;    /* My rank */

double global_sum(double partial) {
  /* Write your hypercube algorithm here */
  MPI_Status status;
  int bitValue,partner;
	double mydone, hisdone;

  mydone = partial;

  for(bitValue=1; bitValue<nprocs; bitValue*=2){
    partner = myid ^ bitValue;
    //send mydone to partner;
    MPI_Send(&mydone,1,MPI_DOUBLE,partner,bitValue,MPI_COMM_WORLD);
    //receive hisdone from partner;
    MPI_Recv(&hisdone,1,MPI_DOUBLE,partner,bitValue,MPI_COMM_WORLD, &status);
    mydone += hisdone;
  }
  return mydone;
}

int main(int argc, char *argv[]) {
  double partial;
  double cpu1, cpu2;
  long long i;
  double step, x, sum=0.0, pi;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

  cpu1 = MPI_Wtime();

  step = 1.0 / NBIN;

  for (i = myid; i < NBIN; i += nprocs) {
    x = (i+0.5)*step;
    sum += 4.0/(1.0+x*x);
  }
  
  partial = sum * step;
  pi = global_sum(partial);
  cpu2 = MPI_Wtime();

  if (myid == 0) {
    printf("PI = %le\n",pi);
    printf("Nprocs & Execution time (s) = %d %le\n",nprocs,cpu2-cpu1);
  }

  MPI_Finalize();
  return 0;
}
