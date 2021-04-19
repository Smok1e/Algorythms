#pragma once

//------------------------

template <size_t Size>
void QuickSort (protected_array <Size>& arr, size_t nMaxCount = Size);

template <size_t Size>
void ReplaceWrongElements (protected_array <Size>& arr, int left, int right);

//------------------------

template <size_t Size> 
void QuickSort (protected_array <Size>& arr, size_t nMaxCount /*= Size*/)
{
	ReplaceWrongElements (arr, 0, nMaxCount-1);
}

//------------------------

template <size_t Size>
void ReplaceWrongElements (protected_array <Size>& arr, int left, int right)
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

	int lft = left;
	int rgt = right;

	int mid = left + (right-left) / 2;
	controllable tmp = arr[mid];

	while (true)
	{
		while (arr[lft] < tmp) lft++;
		while (arr[rgt] > tmp) rgt--;

		if (lft < rgt)
			std::swap (arr[lft++], arr[rgt--]);
		else break;
	}

	ReplaceWrongElements (arr, left,  lft-1);
	ReplaceWrongElements (arr, rgt+1, right);
}

//------------------------
