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

    // Base case: only one process or trivial range
    if (size == 1 || end - start <= 1) {
        quicksort(pivot, start, end, data);
        return;
    }

    // Partition local array
    std::vector<float> less, greater;
    for (int i = start; i < end; i++) {
        if (data[i] < pivot) less.push_back(data[i]);
        else greater.push_back(data[i]);
    }

    // Determine color for splitting communicator (node 0 vs node 1)
    int color = rank / (size / 2); // 0: first half of ranks, 1: second half
    MPI_Comm new_comm;
    MPI_Comm_split(comm, color, rank, &new_comm);

    // Gather all elements for this node
    int local_count = (color == 0) ? less.size() : greater.size();
    std::vector<int> counts(size / 2);
    MPI_Allgather(&local_count, 1, MPI_INT, counts.data(), 1, MPI_INT, new_comm);

    std::vector<int> displs(size / 2, 0);
    for (int i = 1; i < size / 2; i++) displs[i] = displs[i - 1] + counts[i - 1];

    int total = displs[size / 2 - 1] + counts[size / 2 - 1];
    std::vector<float> gathered(total);

    std::vector<float>& local_data = (color == 0) ? less : greater;
    MPI_Allgatherv(local_data.data(), local_count, MPI_FLOAT,
                gathered.data(), counts.data(), displs.data(), MPI_FLOAT, new_comm);

    // Copy gathered back into data
    std::copy(gathered.begin(), gathered.end(), data + start);

    // Compute new pivot and recurse
    if (total > 0) {
        float new_pivot = gathered[total / 2];
        quicksort_distributed(new_pivot, start, start + total, data, new_comm);
    }

    MPI_Comm_free(&new_comm);

}