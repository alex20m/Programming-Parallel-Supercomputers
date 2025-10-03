#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

double pp_blocking(char *buf, int size, int reps, int rank) {
    double total = 0.0;
    for (int r = 0; r < reps; r++) {
        MPI_Barrier(MPI_COMM_WORLD);
        double start, end;
        if (rank == 0) {
            start = MPI_Wtime();
            MPI_Send(buf, size, MPI_BYTE, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(buf, size, MPI_BYTE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            end = MPI_Wtime();
            total += (end - start) / 2.0;
        } else if (rank == 1) {
            MPI_Recv(buf, size, MPI_BYTE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(buf, size, MPI_BYTE, 0, 0, MPI_COMM_WORLD);
        }
    }
    return (rank == 0) ? total/reps : 0.0;

}

double pp_sync(char *buf, int size, int reps, int rank) {
   
    double total = 0.0;
    for (int r = 0; r < reps; r++) {
    double start, end;
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        start = MPI_Wtime();
        MPI_Ssend(buf, size, MPI_BYTE, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(buf, size, MPI_BYTE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        end = MPI_Wtime();
        total += (end - start) / 2.0;
    } else if (rank == 1) {
        MPI_Recv(buf, size, MPI_BYTE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Ssend(buf, size, MPI_BYTE, 0, 0, MPI_COMM_WORLD);
    }
}
    return (rank == 0) ? total/reps : 0.0;

}
double pp_nonblocking(char *buf, int size, int reps, int rank) {
    
    double total = 0.0;
    for (int r = 0; r < reps; r++) {
    double start, end;
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        start = MPI_Wtime();
        MPI_Request reqs[2];
        MPI_Isend(buf, size, MPI_BYTE, 1, 0, MPI_COMM_WORLD, &reqs[0]);
        MPI_Irecv(buf, size, MPI_BYTE, 1, 0, MPI_COMM_WORLD, &reqs[1]);
        MPI_Waitall(2, reqs, MPI_STATUSES_IGNORE);
        end = MPI_Wtime();
        total += (end - start) / 2.0;
    } else if (rank == 1) {
        MPI_Request reqs[2];
        MPI_Irecv(buf, size, MPI_BYTE, 0, 0, MPI_COMM_WORLD, &reqs[0]);
        MPI_Isend(buf, size, MPI_BYTE, 0, 0, MPI_COMM_WORLD, &reqs[1]);
        MPI_Waitall(2, reqs, MPI_STATUSES_IGNORE);
    }
}
    return (rank == 0) ? total/reps : 0.0;

}

int main(int argc, char** argv) {

  /* Initialize the MPI execution environment */
  MPI_Init(NULL, NULL);

  /* Get the rank of the process */
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  for (int p = 0; p <= 20; p++) {  
    int size = 1 << p;             
    char *buf = malloc(size);
    int reps = 1000;
    double t_block = pp_blocking(buf, size, reps, world_rank);
    double t_sync  = pp_sync(buf, size, reps, world_rank);
    double t_nb    = pp_nonblocking(buf, size, reps, world_rank);

if (world_rank == 0)
            printf("%d %.3f %.3f %.3f\n", size, t_block*1e6, t_sync*1e6, t_nb*1e6);
    free(buf);
}
  
  MPI_Finalize();
}

