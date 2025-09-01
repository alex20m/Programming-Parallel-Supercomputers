#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

// Example program that creates a vector data type on sender,
// the receiver receives contiguous data

int main(int argc, char** argv) {

/* Initialize the MPI execution environment */
MPI_Init(&argc,&argv);

int nprocs, world_rank;
MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
 int sender=0;
 int receiver=1;
 double* source;
 double* target;

// stride = the length of the data with a repeating pattern
// blocklen = the contiguous part of the data in this block
// count = How many such repeating patterns there are
// Now we have non-contiguous data for * and we wish to make
// it contiguous for **
// **--**--**--**--**-- source
// |   |
// stride
// | |
// blocklen
// |   |   |   |   |   | count
// ********** target
 int stride=4,blocklen=2,count=5; 
 source = (double*) malloc(stride*count*sizeof(double));
 target = (double*) malloc(count*blocklen*sizeof(double));
 int target_size=count*blocklen*sizeof(double);
 
 for (int i=0; i<stride*count; i++) {
   source[i] = i+.5;
 }
 
 MPI_Datatype newvectortype;
 if (world_rank==sender) {
   MPI_Type_vector(count,blocklen,stride,MPI_DOUBLE,&newvectortype);
   MPI_Type_commit(&newvectortype);
   MPI_Send(source,1,newvectortype,receiver,0,MPI_COMM_WORLD);
   MPI_Type_free(&newvectortype);
 } else if (world_rank==receiver) {
   MPI_Recv(target,target_size,MPI_DOUBLE,sender,0,MPI_COMM_WORLD,
	    MPI_STATUS_IGNORE);
 }

 if (world_rank==receiver) {
   for (int i=0; i<blocklen*count; i++)
     for (int j=0;j<blocklen;j++) 
     if (target[j]!=source[stride*(i+j)])
       printf("location %d %e s/b %e\n",i,target[i],source[stride*(i+j)]);
     }
 if (world_rank==0)
   printf("Finished\n"); 
 
 MPI_Finalize();
}
