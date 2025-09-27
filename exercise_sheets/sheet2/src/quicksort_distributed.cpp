#include <mpi.h>
#include <vector>
#include <iostream>
#include <cstdlib>

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

void quicksort_distributed(float pivot, int start, int end, float* &data, MPI_Comm comm)
{
/**
        Exercise 2: Your code here
        Input:
                pivot: a pivot value based on which to split the array in to less and greater elems
                start: starting index of the range to be sorted
                end: exclusive ending index of the range to be sorted
                data: array of floats to sort in range start till end
		      the array is the same for each MPI process
		comm: the communicator of the MPI processes
        Return:
                upon return each MPI process should have their array sorted
        Task:   
                To sort the array using the idea of quicksort in a stable manner using each MPI process.
                A sort is stable if it maintains the relative order of elements with equal values.

		Truly split the work among the process i.e. don't have each process independently sort the array.
		You should stick to the idea of quicksort also for the splitting of the array between processes.
		As an example you should not split the array into N equal sized parts (where N is the number of processors),
		sort them and then combine then (this would be following mergesort). Instead the splitting should be based on the
		pivot values
	
	       	 	

	Hints:
		why is the communicator given to you?
		maybe it could be a good idea to use MPI_Comm_split?
		could you reuse your quicksort implementation for the base case of a single process?

**/
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    // Base case: single process, just use local quicksort
    if (size == 1 || end - start <= 1) {
        quicksort(pivot, start, end, data);
        return;
    }

    // Partition local array
    std::vector<float> less, equal, greater;
    for (int i = start; i < end; i++) {
        if (data[i] < pivot) less.push_back(data[i]);
        else if (data[i] == pivot) equal.push_back(data[i]);
        else greater.push_back(data[i]);
    }

    // Count elements in each group globally
    int local_less = less.size();
    int local_greater = greater.size();
    int total_less, total_greater;
    MPI_Allreduce(&local_less, &total_less, 1, MPI_INT, MPI_SUM, comm);
    MPI_Allreduce(&local_greater, &total_greater, 1, MPI_INT, MPI_SUM, comm);

    // Decide which processes handle less and greater halves
    int half = size / 2;
    int color = (rank < half) ? 0 : 1; // 0: less, 1: greater
    MPI_Comm new_comm;
    MPI_Comm_split(comm, color, rank, &new_comm);

    // Gather data for this sub-communicator
    std::vector<float> local_data = (color == 0) ? less : greater;
    int local_count = local_data.size();
    std::vector<int> counts(size), displs(size);
    MPI_Allgather(&local_count, 1, MPI_INT, counts.data(), 1, MPI_INT, comm);

    displs[0] = 0;
    for (int i = 1; i < size; i++) displs[i] = displs[i - 1] + counts[i];

    std::vector<float> gathered(displs[size - 1] + counts[size - 1]);
    MPI_Allgatherv(local_data.data(), local_count, MPI_FLOAT,
                gathered.data(), counts.data(), displs.data(), MPI_FLOAT,
                comm);

    // Copy back gathered data
    std::copy(gathered.begin(), gathered.end(), data + start);

    // Compute new pivot for next recursion
    float new_pivot = gathered[gathered.size() / 2];

    // Recursive call on sub-communicator
    quicksort_distributed(new_pivot, start, start + gathered.size(), data, new_comm);

    MPI_Comm_free(&new_comm);

}