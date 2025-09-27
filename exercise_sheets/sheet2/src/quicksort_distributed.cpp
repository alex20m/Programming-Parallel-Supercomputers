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

    if (end - start <= 1 || size == 1) {
        std::stable_sort(data + start, data + end);
        return;
    }

    // Local partition
    std::vector<float> less, equal, greater;
    for (int i = start; i < end; i++) {
        if (data[i] < pivot) less.push_back(data[i]);
        else if (data[i] == pivot) equal.push_back(data[i]);
        else greater.push_back(data[i]);
    }

    // --- Simple exchange between lower and upper halves ---
    int half = size / 2;
    int color = (rank < half) ? 0 : 1;
    int partner = (color == 0) ? rank + half : rank - half;

    std::vector<float> send_buf = (color == 0) ? greater : less;
    int send_count = send_buf.size();
    int recv_count = 0;

    // Exchange sizes first
    MPI_Sendrecv(&send_count, 1, MPI_INT, partner, 0,
                 &recv_count, 1, MPI_INT, partner, 0,
                 comm, MPI_STATUS_IGNORE);

    std::vector<float> recv_buf(recv_count);

    // Exchange elements
    if (send_count > 0)
        MPI_Send(send_buf.data(), send_count, MPI_FLOAT, partner, 1, comm);
    if (recv_count > 0)
        MPI_Recv(recv_buf.data(), recv_count, MPI_FLOAT, partner, 1, comm, MPI_STATUS_IGNORE);

    // Keep the correct elements
    std::vector<float> new_data = (color == 0) ? less : greater;
    new_data.insert(new_data.end(), recv_buf.begin(), recv_buf.end());

    // Copy back to original array
    for (int i = 0; i < new_data.size(); i++)
        data[start + i] = new_data[i];

    // Split communicator for recursion
    MPI_Comm new_comm;
    MPI_Comm_split(comm, color, rank, &new_comm);

    // Recursive quicksort on the local slice
    int part_start = start;
    int part_end = start + new_data.size();
    if (part_end > part_start) {
        float new_pivot = data[part_start + (part_end - part_start)/2];
        quicksort_distributed(new_pivot, part_start, part_end, data, new_comm);
    }

    MPI_Comm_free(&new_comm);
}