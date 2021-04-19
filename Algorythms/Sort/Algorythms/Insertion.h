#pragma once

//------------------------

template <size_t Size>
void SortInsertion (protected_array <Size>& arr, size_t nMaxCount = Size);

template <size_t Size>
int FindInsertion (protected_array <Size>& arr, size_t src, size_t nMaxCount = Size);

template <size_t Size>
void Insert (protected_array <Size>& arr, size_t src, size_t dst);

//------------------------

template <size_t Size>
void SortInsertion (protected_array <Size>& arr, size_t nMaxCount /*= Size*/)
{
	for (int sorted = 1; sorted < Size && sorted < nMaxCount; sorted++)
	{
		int i = FindInsertion (arr, sorted, sorted);
		Insert (arr, sorted, i);
	}
}

//------------------------

template <size_t Size>
int FindInsertion (protected_array <Size>& arr, size_t src, size_t nMaxCount /*= Size*/)
{
	for (int i = 0; i < nMaxCount && i < Size; i++)
		if (arr[i] > arr[src]) return i;
	return nMaxCount;
}

//------------------------

template <size_t Size>
void Insert (protected_array <Size>& arr, size_t src, size_t dst)
{
	if (src == dst) return;

	int value = arr[src];
	for (int i = src; i > dst && i > 0; i--)
		arr[i] = arr[i-1];

	arr[dst] = value;
}

//------------------------