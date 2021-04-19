#pragma once

//------------------------

#define ERRMSG(caption, format, ...)                                           \
{																		       \
	char message[MAX_PATH] = "";                                               \
	sprintf_s (message, format, __VA_ARGS__);                                  \
	printf ("%s: %s\n", caption, message);                                     \
	MessageBoxA (GetConsoleWindow (), message, caption, MB_OK | MB_ICONERROR); \
	abort ();																   \
}

//------------------------

typedef long long canary_t;
const canary_t CanaryValue = 0xDEFEC8EDDEADF00D;

//------------------------

template <size_t Size>
class protected_array
{

public :

	protected_array ();
	protected_array (const std::initializer_list <int>& list);

	controllable& operator [] (int index);

	size_t size ();

	void print (size_t nMaxCount = Size);

private :

	canary_t     left_canary_;
	controllable data_[Size];
	canary_t     right_canary_;

	void check ();

};

//------------------------

template <size_t Size>
protected_array <Size>::protected_array () :
	left_canary_  (CanaryValue),
	data_         (),
	right_canary_ (CanaryValue)
{}

//------------------------

template <size_t Size>
protected_array <Size>::protected_array (const std::initializer_list <int>& list) :
	left_canary_  (CanaryValue),
	data_         (),
	right_canary_ (CanaryValue)
{
	size_t size = list.size ();
	for (size_t i = 0; i < size && i < Size; i++)
		data_[i] = *(list.begin () + i);
}

//------------------------

template <size_t Size>
controllable& protected_array <Size>::operator [] (int index)
{
	if (index < 0 || index >= Size)
		ERRMSG ("Wrong index", "Trying access to array by [index = %d], but [size = %zu], process will be aborted.", index, Size);

	check ();
	controllable& value = data_[index];
	check ();

	return value;
}

//------------------------

template <size_t Size>
size_t protected_array <Size>::size ()
{
	return Size;
}

//------------------------

template <size_t Size>
void protected_array <Size>::print (size_t nMaxCount /*= Size*/)
{
	printf ("[size = %zu, maxcount = %zu]:\n", Size, nMaxCount);

	for (size_t i = 0; i < Size && i < nMaxCount; i++)
		printf ("[%3zu]: %d\n", i, data_[i].operator int ());
}

//------------------------

template <size_t Size>
void protected_array <Size>::check ()
{
	if (left_canary_ != CanaryValue)
	{
		ERRMSG ("Canary corruption", "Stack around array was corrupted\n"
			                         "Left canary = 0x%016llX, process will be aborted.", left_canary_);
	}

	if (right_canary_ != CanaryValue)
	{
		ERRMSG ("Canary corruption", "Stack around array was corrupted\n"
			                         "Right canary = 0x%016llX, process will be aborted.", right_canary_);
	}	
}

//------------------------