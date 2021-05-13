#pragma once

//------------------------

template <size_t Size>
void QuickSort (protected_array <Size>& arr, size_t nMaxCount = Size);

template <size_t Size>
void AntiQuickSort (protected_array <Size> & arr, size_t nMaxCount = Size);

template <size_t Size>
void _QuickSort (protected_array <Size>& arr, int left, int right);

template <typename Type>
void qSwap (Type& a, Type& b);

//------------------------

template <size_t Size> 
void QuickSort (protected_array <Size>& arr, size_t nMaxCount /*= Size*/)
{
	_QuickSort (arr, 0, nMaxCount-1);
}

//------------------------

template <size_t Size>
void AntiQuickSort (protected_array <Size> & arr, size_t nMaxCount /*= Size*/)
{
	int last = 0;
	for (size_t i = 0; i < nMaxCount && i < Size; i++)
		arr[i] = (last += rand () % 10);

	for (size_t i = 1; i < nMaxCount && i < Size; i++)
		std::swap (arr[i], arr[i / 2]);
}

//------------------------

template <size_t Size>
void _QuickSort (protected_array <Size>& arr, int left, int right)
{
	size_t size = right-left+1;
	
	if (size == 1)
		return;

	else if (size <= 3)
	{
		SedgewickSort (arr, left, right);
		return;
	}

	int lft = left;
	int rgt = right;

	int mid = left + rand () % (right-left);
	controllable tmp = arr[mid];

	while (true)
	{
		while (arr[lft] < tmp) lft++;
		while (arr[rgt] > tmp) rgt--;

		if (lft < rgt)
			qSwap (arr[lft++], arr[rgt--]);

		else break;
	}

	_QuickSort (arr, left,  lft-1);
	_QuickSort (arr, rgt+1, right);
}										  

//------------------------

template <typename Type>
void qSwap (Type& a, Type& b)
{
	if (a != b) std::swap (a, b);
}

//------------------------