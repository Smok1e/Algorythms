#pragma once

//-----------------------

template <size_t Size>
void MergeSort (protected_array <Size>& arr, size_t nMaxCount = Size);

template <size_t Size>
void _Merge (protected_array <Size>& arr, int begin, int mid, int end, controllable* tmp_memory);

template <size_t Size>
void _MergeSort (protected_array <Size>& arr, int begin, int end, controllable* tmp_memory);

//-----------------------

template <size_t Size>
void MergeSort (protected_array <Size>& arr, size_t nMaxCount /*= Size*/)
{
	controllable* tmp_memory = new controllable[nMaxCount];

	_MergeSort (arr, 0, nMaxCount-1, tmp_memory);

	delete[] (tmp_memory);
}

//-----------------------

template <size_t Size>
void _Merge (protected_array <Size>& arr, int begin, int mid, int end, controllable* tmp_memory)
{
	const size_t l_size = mid-begin+1;
	const size_t r_size = end-mid;

	const size_t tmp_size = l_size + r_size;

	const size_t l_begin = 0;
	const size_t r_begin = l_size;

	for (size_t i = 0; i < tmp_size; i++) tmp_memory[i] = arr[begin+i];
	
	size_t l_index = 0;
	size_t r_index = 0;

	size_t arr_index = begin;

	while (l_index < l_size && r_index < r_size)
		if   (tmp_memory[l_index] <= tmp_memory[r_index]) arr[arr_index++] = tmp_memory[l_index++];
		else                                              arr[arr_index++] = tmp_memory[r_index++];

	while (l_index < l_size) arr[arr_index++] = tmp_memory[l_index++];
	while (r_index < r_size) arr[arr_index++] = tmp_memory[r_index++];
}

//-----------------------

template <size_t Size>
void _MergeSort (protected_array <Size>& arr, int left, int right, controllable* tmp_memory)
{
	size_t size = right-left + 1;

	if (size == 1) 
		return;

	else if (size <= 3)
	{
		SedgewickSort (arr, left, right);
		return;
	}

	int mid = left + (right-left) / 2;

	_MergeSort (arr, left,  mid,   tmp_memory);
	_MergeSort (arr, mid+1, right, tmp_memory);

	_Merge (arr, left, mid, right, tmp_memory);
}

//-----------------------