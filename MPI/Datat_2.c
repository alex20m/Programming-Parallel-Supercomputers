#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "mpi.h"

int main() {

  MPI_Init(NULL,NULL);

  int rank;
  int size;
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  MPI_Comm_size(MPI_COMM_WORLD,&size);
  
  if (size<2) {
    printf("This program needs at least two processes\n");
    MPI_Finalize();
    return -1;
  }  
  int asize=2;
  struct dataa {
    int i;
    double *x;
  };
  struct dataa buffer;

    // Allocate the array dynamically
    buffer.x = malloc(asize * sizeof(double));
    if (!buffer.x) {
        fprintf(stderr, "Memory allocation failed\n");
        MPI_Finalize();
        return -1;
    }

    // Define and commit a new datatype
    int          count=2; //Two elements in the structure
    int          blocklength[count]; //Array to store the size of each block
    MPI_Aint     displacement[count]; //Array to store the displacements 
    MPI_Datatype datatypes[count]; //Array to store the types of data in each block
    MPI_Datatype mpi_struct_data; // New datatype to be created

    // Integer field
    blocklength[0] = 1;
    datatypes[0] = MPI_INT;
    displacement[0] = offsetof(struct dataa, i);

    // Array of doubles
    blocklength[1] = asize;
    datatypes[1] = MPI_DOUBLE;
    displacement[1] = offsetof(struct dataa, x);

  if (rank == 0) {
      buffer.i = 1234;
      buffer.x[0] = 10.0;
      buffer.x[1] = 20.0;
      printf("Sending %d %f %f\n", buffer.i, buffer.x[0], buffer.x[1]);

      MPI_Send(&buffer.i, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
      MPI_Send(buffer.x, asize, MPI_DOUBLE, 1, 1, MPI_COMM_WORLD);
  } else {
      MPI_Recv(&buffer.i, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Recv(buffer.x, asize, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf("Received %d %f %f\n", buffer.i, buffer.x[0], buffer.x[1]);
  }

  free(buffer.x);

  MPI_Finalize();
}
