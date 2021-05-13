#pragma once

//------------------------

template <size_t Size>
void FillRand (protected_array <Size>& arr, size_t nMaxCount = Size, int min = 0, int max = 1000);

//------------------------

template <size_t Size>
void FillRand (protected_array <Size>& arr, size_t nMaxCount /*= Size*/, int min /*= 0*/, int max /*= 1000*/)
{
	for (size_t i = 0; i < Size && i < nMaxCount; i++)
		arr[i] = controllable (min + rand () % (max - min), i);
}

//------------------------