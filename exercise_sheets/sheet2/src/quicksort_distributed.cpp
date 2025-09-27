#include <mpi.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cstdlib>

// Sequential quicksort
void quicksort(float pivot, int start, int end, float* &data) {
    if (end - start <= 1) return;
    std::vector<float> less, equal, greater;
    for (int i = start; i < end; i++)
        if (data[i] < pivot) less.push_back(data[i]);
        else if (data[i] == pivot) equal.push_back(data[i]);
        else greater.push_back(data[i]);
    int idx = start;
    for (float v : less) data[idx++] = v;
    for (float v : equal) data[idx++] = v;
    for (float v : greater) data[idx++] = v;
    if (!less.empty()) quicksort(less[0], start, start + less.size(), data);
    if (!greater.empty()) quicksort(greater[0], end - greater.size(), end, data);
}

// Distributed quicksort
void quicksort_distributed(float pivot, int start, int end, float* &data, MPI_Comm comm) {
    int rank, size; MPI_Comm_rank(comm, &rank); MPI_Comm_size(comm, &size);
    if (size == 1 || end - start <= 1) { quicksort(pivot, start, end, data); return; }

    std::vector<float> less, greater;
    for (int i = start; i < end; i++)
        (data[i] < pivot ? less : greater).push_back(data[i]);

    int half = size/2, color = rank < half ? 0 : 1;
    MPI_Comm new_comm; MPI_Comm_split(comm, color, rank, &new_comm);

    std::vector<float>& local = color==0 ? less : greater;
    int local_count = local.size(), new_size; MPI_Comm_size(new_comm, &new_size);
    std::vector<int> counts(new_size), displs(new_size,0);
    MPI_Allgather(&local_count, 1, MPI_INT, counts.data(), 1, MPI_INT, new_comm);
    for (int i=1;i<new_size;i++) displs[i]=displs[i-1]+counts[i-1];

    int total = displs[new_size-1]+counts[new_size-1];
    std::vector<float> gathered(total);
    MPI_Allgatherv(local.data(), local_count, MPI_FLOAT, gathered.data(), counts.data(), displs.data(), MPI_FLOAT, new_comm);
    std::copy(gathered.begin(), gathered.end(), data+start);

    if (!gathered.empty()) quicksort_distributed(gathered[total/2], start, start+total, data, new_comm);
    MPI_Comm_free(&new_comm);
}
