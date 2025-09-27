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
    
    std::vector<float> less;
    std::vector<float> greater;

    for (int i = start; i < end; i++) {
        if (data[i] < pivot)
            less.push_back(data[i]);
        else
            greater.push_back(data[i]);
    }

    int less_count = less.size();
    int greater_count = greater.size();
    int total_less = 0;
    MPI_Allreduce(&less_count, &total_less, 1, MPI_INT, MPI_SUM, comm);

    // Split processes based on pivot
    int color = rank < size / 2 ? 0 : 1;
    MPI_Comm new_comm;
    MPI_Comm_split(comm, color, rank, &new_comm);

    // Send data to right group
    int local_count = (color == 0) ? less.size() : greater.size();

    // Get counts in new_comm
    int new_rank, new_size;
    MPI_Comm_rank(new_comm, &new_rank);
    MPI_Comm_size(new_comm, &new_size);
    std::vector<int> counts(new_size);
    MPI_Allgather(&local_count, 1, MPI_INT, counts.data(), 1, MPI_INT, new_comm);

    // Get displacement and total size
    std::vector<int> displs(new_size, 0);
    int total = counts[0];
    for (int i = 1; i < new_size; i++) {
        displs[i] = displs[i - 1] + counts[i - 1];
        total += counts[i];
    }

    std::vector<float> gathered(total);
    MPI_Allgatherv((color == 0 ? less.data() : greater.data()), local_count, MPI_FLOAT,
                   gathered.data(), counts.data(), displs.data(), MPI_FLOAT, new_comm);

    // Copy data back
    for (int i = 0; i < total; i++) {
        data[start + i] = gathered[i];
    }

    // Recursive call with new communicator
    if (!gathered.empty()) {
        float new_pivot = gathered[0];
        quicksort_distributed(new_pivot, start, start + total, data, new_comm);
    }

    MPI_Comm_free(&new_comm);
}