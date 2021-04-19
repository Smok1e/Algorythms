#pragma once

//------------------------

template <size_t Size>
int CheckSort (protected_array <Size>& arr, size_t nMaxCount = Size);

template <size_t Size>
bool TestSort (protected_array <Size>& arr, size_t nMaxCount = Size);

//------------------------

template <size_t Size>
int CheckSort (protected_array <Size>& arr, size_t nMaxCount /*= Size*/)
{
	for (size_t i = 1; i < Size && i < nMaxCount; i++)
		if (arr[i] < arr[i-1]) return i;
	return 0;
}

//------------------------

template <size_t Size>
bool TestSort (protected_array <Size>& arr, size_t nMaxCount /*= Size*/)
{
	int failed = CheckSort (arr, nMaxCount);

	if (failed)
	{
		printf ("Array check failed at [index = %d], size = %zu, maxcount = %zu:\n", failed, Size, nMaxCount);
		for (size_t i = 0; i < Size && i < nMaxCount; i++)
		{
			if (i == failed)
				printf ("[%3zu]: %d <= FAILED\n", i, arr[i].operator int ());
			else
				printf ("[%3zu]: %d\n",           i, arr[i].operator int ());
		}
	}

	else
		printf ("Ok\n");

	return !failed;
}

//------------------------