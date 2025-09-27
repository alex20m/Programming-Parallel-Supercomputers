#include <mpi.h>
#include <vector>

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

    int mid = start + less.size();

    // Split communicator for distributed recursion
    int color = (rank < size / 2) ? 0 : 1;
    MPI_Comm new_comm;
    MPI_Comm_split(comm, color, rank, &new_comm);

    int part_start = (color == 0) ? start : mid; // Less
    int part_end   = (color == 0) ? mid : end; // Equal and greater

    // Pick new pivot
    if (part_end > part_start) {
        float new_pivot = data[part_start + (part_end - part_start)/2];
        quicksort_distributed(new_pivot, part_start, part_end, data, new_comm);
    }

    MPI_Comm_free(&new_comm);
}