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
    
    if (size == 1) {
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
            quicksort_distributed(less[0], start, start + less.size(), data, comm);
        if (!greater.empty())
            quicksort_distributed(greater[0], end - greater.size(), end, data, comm);
        return;
    }
    
    // Split local data according to pivot
    std::vector<float> less, equal, greater;
    for (int i = start; i < end; i++) {
        if (data[i] < pivot) less.push_back(data[i]);
        else if (data[i] == pivot) equal.push_back(data[i]);
        else greater.push_back(data[i]);
    }

    int rank_group = rank < size / 2 ? 0 : 1;  // split ranks into two groups
    MPI_Comm new_comm;
    MPI_Comm_split(comm, rank_group, rank, &new_comm);

    // Gather sizes of partitions across the new group
    int local_size = (rank_group == 0 ? less.size() : greater.size());
    std::vector<int> sizes;
    int new_rank, new_size;
    MPI_Comm_rank(new_comm, &new_rank);
    MPI_Comm_size(new_comm, &new_size);

    sizes.resize(new_size);
    MPI_Allgather(&local_size, 1, MPI_INT, sizes.data(), 1, MPI_INT, new_comm);

    // Compute displacement for gather
    std::vector<int> displs(new_size, 0);
    int total = sizes[0];
    for (int i = 1; i < new_size; i++) {
        displs[i] = displs[i - 1] + sizes[i - 1];
        total += sizes[i];
    }

    // Gather all elements of this partition
    std::vector<float> gathered(total);
    MPI_Allgatherv(
        rank_group == 0 ? less.data() : greater.data(), local_size, MPI_FLOAT,
        gathered.data(), sizes.data(), displs.data(), MPI_FLOAT,
        new_comm
    );

    // Copy gathered data back to the correct segment
    for (int i = 0; i < total; i++) {
        data[start + i] = gathered[i];
    }

    // Choose new pivot (first element in the gathered partition)
    float new_pivot = gathered.empty() ? pivot : gathered[0];

    // Recursive call for the new communicator
    quicksort_distributed(new_pivot, start, start + total, data, new_comm);

    MPI_Comm_free(&new_comm);
}