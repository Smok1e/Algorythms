#pragma once

//------------------------

template <size_t Size>
void SortBinaryInsertion (protected_array <Size>& arr, size_t nMaxCount = Size);

template <size_t Size>
int FindBinaryInsertion (protected_array <Size>& arr, size_t src, size_t nMaxCount = Size);

template <size_t Size>
void BinaryInsert (protected_array <Size>& arr, size_t src, size_t dst);

//------------------------

template <size_t Size>
void SortBinaryInsertion (protected_array <Size>& arr, size_t nMaxCount /*= Size*/)
{
	for (int sorted = 1; sorted < Size && sorted < nMaxCount; sorted++)
	{
		int i = FindBinaryInsertion (arr, sorted, sorted);
		BinaryInsert (arr, sorted, i);
	}
}

//------------------------

template <size_t Size>
int FindBinaryInsertion (protected_array <Size>& arr, size_t src, size_t nMaxCount /*= Size*/)
{
	int begin = -1;
	int end   = nMaxCount;

	while (end-begin > 1)
	{
	    int mid = begin + (end-begin) / 2;

		if (arr[mid] > arr[src]) { end   = mid; continue; }
		if (arr[mid] < arr[src]) { begin = mid; continue; }
		
		return mid+1;
	}

	return end;
}

//------------------------

template <size_t Size>
void BinaryInsert (protected_array <Size>& arr, size_t src, size_t dst)
{
	if (src == dst) return;

	int value = arr[src];
	for (int i = src; i > dst && i > 0; i--)
		arr[i] = arr[i-1];

	arr[dst] = value;
}

//------------------------