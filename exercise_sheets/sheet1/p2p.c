#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

double blocking(char *buffer, int size, int reps, int rank) {
    double total = 0.0;
    for (int r = 0; r < reps; r++) {
        MPI_Barrier(MPI_COMM_WORLD);
        if (rank == 0) {
            double start = MPI_Wtime();
            MPI_Send(buffer, size, MPI_BYTE, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(buffer, size, MPI_BYTE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            double end = MPI_Wtime();
            total += (end - start) / 2.0;
        } else {
            MPI_Recv(buffer, size, MPI_BYTE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(buffer, size, MPI_BYTE, 0, 0, MPI_COMM_WORLD);
        }
    }
    return (rank == 0) ? total / reps : 0.0;
}

double syncing(char *buffer, int size, int reps, int rank) {
    double total = 0.0;
    for (int r = 0; r < reps; r++) {
        MPI_Barrier(MPI_COMM_WORLD);
        if (rank == 0) {
            double start = MPI_Wtime();
            MPI_Ssend(buffer, size, MPI_BYTE, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(buffer, size, MPI_BYTE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            double end = MPI_Wtime();
            total += (end - start) / 2.0;
        } else {
            MPI_Recv(buffer, size, MPI_BYTE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Ssend(buffer, size, MPI_BYTE, 0, 0, MPI_COMM_WORLD);
        }
    }
    return (rank == 0) ? total / reps : 0.0;
}

/* Non-blocking send/recv */
double nonblocking(char *buffer, int size, int reps, int rank) {
    double total = 0.0;
    for (int r = 0; r < reps; r++) {
        MPI_Barrier(MPI_COMM_WORLD);
        if (rank == 0) {
            MPI_Request requests[2];
            double start = MPI_Wtime();
            MPI_Isend(buffer, size, MPI_BYTE, 1, 0, MPI_COMM_WORLD, &requests[0]);
            MPI_Irecv(buffer, size, MPI_BYTE, 1, 0, MPI_COMM_WORLD, &requests[1]);
            MPI_Waitall(2, requests, MPI_STATUSES_IGNORE);
            double end = MPI_Wtime();
            total += (end - start) / 2.0;
        } else {
            MPI_Request requests[2];
            MPI_Irecv(buffer, size, MPI_BYTE, 0, 0, MPI_COMM_WORLD, &requests[0]);
            MPI_Isend(buffer, size, MPI_BYTE, 0, 0, MPI_COMM_WORLD, &requests[1]);
            MPI_Waitall(2, requests, MPI_STATUSES_IGNORE);
        }
    }
    return (rank == 0) ? total / reps : 0.0;
}

int main(int argc, char **argv) {
    MPI_Init(NULL, NULL);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int reps = 1000;

    if (rank == 0)
        printf("# Size(bytes)  Blocking(μs)  Synchronous(μs)  Nonblocking(μs)\n");

    for (int p = 0; p <= 20; p++) {
        int size = 1 << p;
        char *buffer = malloc(size);

        double t_block = blocking(buffer, size, reps, rank);
        double t_sync  = syncing(buffer, size, reps, rank);
        double t_nb    = nonblocking(buffer, size, reps, rank);

        if (rank == 0)
            printf("%8d  %12.3f  %15.3f  %14.3f\n",
                   size, t_block * 1e6, t_sync * 1e6, t_nb * 1e6);
        free(buffer);
    }

    MPI_Finalize();
    return 0;
}