#pragma once

//------------------------

template <size_t Size>
void SedgewickSort (protected_array <Size>& arr, int left, int right)
{
	int size = right-left + 1;

	if (size <= 1) 
		return;
	
	if (size == 2)
	{
		if (arr[left] > arr[right]) 
			std::swap (arr[left], arr[right]);

		return;
	}

	if (size == 3)
	{
		if (arr[left+2] < arr[left  ]) std::swap (arr[left+2], arr[left  ]);
		if (arr[left+2] < arr[left+1]) std::swap (arr[left+2], arr[left+1]);
		if (arr[left+1] < arr[left  ]) std::swap (arr[left+1], arr[left  ]);

		return;
	}
}

//------------------------