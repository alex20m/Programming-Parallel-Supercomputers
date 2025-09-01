#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main() {

  MPI_Init(NULL,NULL);

  int rank;
  int size;
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  MPI_Comm_size(MPI_COMM_WORLD,&size);
  
  if (size<2) {
    printf("This program needs at least two processes\n");
    return -1;
  }  
  int asize=2;
  struct dataa {
    int i;
    double x[asize];
  };
  struct dataa buffer;

    // Define and commit a new datatype
    int          count=2; //Two elements in the structure
    int          blocklength[count]; //Array to store the size of each block
    MPI_Aint     displacement[count]; //Array to store the displacements 
    MPI_Datatype datatypes[count]; //Array to store the types of data in each block
    MPI_Datatype mpi_struct_data; // New datatype to be created

    // One integer
    blocklength[0] = 1 ; datatypes[0]=MPI_INT;
    displacement[0] = (size_t)&(buffer.i)-(size_t)&buffer;
  
    // Array of floats
    blocklength[1] = asize ; datatypes[1]=MPI_DOUBLE;
    displacement[1] = (size_t)&(buffer.x)-(size_t)&buffer;

    MPI_Type_create_struct(count,blocklength,displacement,datatypes,&mpi_struct_data);
    MPI_Type_commit(&mpi_struct_data);

    if (rank==0) {
	}

    if (rank==0) {
      buffer.i=1234;
      buffer.x[0]=10.0;
      buffer.x[1]=20.0;
      printf("Sending %d %f %f\n: ",buffer.i,buffer.x[0],buffer.x[1]);
      MPI_Send(&buffer,1,mpi_struct_data,1,0,MPI_COMM_WORLD);
    } else {
      MPI_Recv(&buffer,1,mpi_struct_data,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
      printf("Received %d %f %f\n: ",buffer.i,buffer.x[0],buffer.x[1]);
    }
    MPI_Type_free(&mpi_struct_data);

    MPI_Finalize();
}
