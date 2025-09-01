#include <mpi.h>
#include <stdio.h>
#if defined(_OPENMP)
#include <omp.h>
#endif

MPI_Comm THREADPRIVATE_COMM = MPI_COMM_NULL;
#pragma omp threadprivate(THREADPRIVATE_COMM)
int main(int argc, char** argv) {

  // Program using highest level of thread support; Communicating from
  // thread to another thread inside omp parallel region.

  const int n_cores = omp_get_max_threads();
  int prov,req=MPI_THREAD_MULTIPLE;
  /* Initialize the MPI execution environment */
  MPI_Init_thread(NULL, NULL,req,&prov);

  if( prov < req ) {
     printf("Error: MPI implementation does not support the threading level requested.\n");
     MPI_Abort(MPI_COMM_WORLD,0); }

  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  int next_id, prev_id;
  MPI_Status status;

  next_id = world_rank+1;
  prev_id = world_rank-1;

  // Plenty of private variables need to be declared
  #pragma omp parallel num_threads(n_cores)
  {
	  //MPI_Comm_dup(MPI_COMM_WORLD,&THREADPRIVATE_COMM);
	  for(int i = 0; i < n_cores; ++i)
	  {
	  	if(omp_get_thread_num() == i)
	  		MPI_Comm_dup(MPI_COMM_WORLD,&THREADPRIVATE_COMM);
#pragma omp barrier
	  }
  }
#pragma omp parallel num_threads(n_cores)
  {
  const int idth = omp_get_thread_num();
  const int send_value = world_rank*idth;
  int recv_value;
  MPI_Allreduce(&send_value,&recv_value,1,MPI_INT,MPI_SUM,THREADPRIVATE_COMM);
  printf("Hi, this is rank %d, thread %d! Result is: %d\n",world_rank,idth,recv_value);
  }

  MPI_Finalize();
}

