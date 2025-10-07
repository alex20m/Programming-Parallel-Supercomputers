#include <mpi.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <cstring> 

// Sequential quicksort
void quicksort(float pivot, int start, int end, float* &data)
{
	if (end - start <= 1)
		return;

	std::vector<float> less;
	std::vector<float> equal;
	std::vector<float> greater;

	for (int i = start; i < end; i++) {
		if (data[i] < pivot)
			less.push_back(data[i]);
		else if (data[i] == pivot)
			equal.push_back(data[i]);
		else
			greater.push_back(data[i]);
	}

	int idx = start;
	for (float v : less)     data[idx++] = v;
	for (float v : equal)    data[idx++] = v;
	for (float v : greater)  data[idx++] = v;

	if (!less.empty())
        quicksort(less[0], start, start + less.size(), data);
    if (!greater.empty())
        quicksort(greater[0], end - greater.size(), end, data);
}

// Distributed quicksort
void quicksort_distributed(float pivot, int start, int end, float* &data, MPI_Comm comm) {
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    // Base case: only one process or small chunk -> sequential sort
    if (size == 1 || end - start <= 1) {
        quicksort(pivot, start, end, data);
        return;
    }
    const int n = end - start;

    // --- Partition local data into < pivot and >= pivot ---
    std::vector<float> less, greater;
    for (int i = start; i < end; i++)
        (data[i] < pivot ? less : greater).push_back(data[i]);

    // --- Compute how many elements in each group ---
    int local_less = less.size();
    int local_greater = greater.size();
    int total_less, total_greater;

    int mid = start + total_less;
    if (total_less > 0) std::memcpy(&data[start], less.data(), total_less * sizeof(float));
    if (total_greater > 0) std::memcpy(&data[mid], greater.data(), greater * sizeof(float));

    if (total_less == 0 || total_greater == 0) {
        if (rank == 0){
            quicksort(pivot, start, end, data);
        }
        else {
            // --- Broadcast pivot to all processes in communicator ---
            MPI_Bcast(&data[start], n, MPI_FLOAT, comm);
        }
    }

    // --- Split communicator: ranks < half handle "less", others "greater" ---
    int color = (rank < size / 2) ? 0 : 1;
    MPI_Comm new_comm;
    MPI_Comm_split(comm, color, rank, &new_comm);

    // --- Recursive distributed call ---
    if (color == 0 && total_less > 0) {
        float new_pivot = data[start];
        quicksort_distributed(new_pivot, start, mid, data, new_comm);
    } else if (color == 1 && total_greater > 0) {
        float new_pivot = data[mid];
        quicksort_distributed(new_pivot, mid, end, data, new_comm);
    }

    MPI_Comm_free(&new_comm);
}