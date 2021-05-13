#pragma once

//------------------------

template <size_t Size>
void SortBubble (protected_array <Size>& arr, size_t nMaxCount = Size);

//------------------------

template <size_t Size>
void SortBubble (protected_array <Size>& arr, size_t nMaxCount /*= Size*/)
{
	bool running = true;

	for (int n = 0; n < Size-1 && n < nMaxCount-1 && running; n++)
	{
		running = false;							   

		for (int i = 0; i < Size-n-1 && i < nMaxCount-n-1; i++) 
			if (arr[i] > arr[i+1]) std::swap (arr[i], arr[i+1]), running = true;
	}
}

//------------------------