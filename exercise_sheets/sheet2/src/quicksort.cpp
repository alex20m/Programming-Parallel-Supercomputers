#include <iostream>
#include <cstdlib>
#include <vector>

void quicksort(float pivot, int start, int end, float* &data)
{
/**
	Exercise 1: Your code here
	Input:
		pivot: a pivot value based on which to split the array in to less and greater elems
		start: starting index of the range to be sorted
		end: exclusive ending index of the range to be sorted
		data: array of floats to sort in range start till end
	Return:
		upon return the array range should be sorted
	Task: 	
		to sort the array using the idea of quicksort in a stable manner
		a sort is stable if it maintains the relative order of elements with equal values
**/
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
