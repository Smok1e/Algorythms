#pragma once

template <size_t Size>
void SortSelection (protected_array <Size>& arr, size_t nMaxCount = Size);

//------------------------

template <size_t Size>
void SortSelection (protected_array <Size>& arr, size_t nMaxCount /*= Size*/)
{
	bool running = true;

	for (int n = 0; n < Size && n < nMaxCount; n++)
	{
		running = false;

		int selected = n;
		for (int i = n; i < Size && i < nMaxCount; i++)
			if (arr[i] < arr[selected]) selected = i, running = true;

		if (running) std::swap (arr[n], arr[selected]);
	}
}

//------------------------