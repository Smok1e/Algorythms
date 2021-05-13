#pragma once

//---------------------

bool EnableSedgewick = true;

//---------------------

#include "Sort/Controllable.h"
#include "Sort/ProtectedArray.h"
#include "Sort/CheckSort.h"
#include "Sort/FillRand.h"

#include "Sort/Algorythms/SedgewickSort.h"

#include "Sort/Algorythms/Bubble.h"
#include "Sort/Algorythms/Selection.h"
#include "Sort/Algorythms/Insertion.h"
#include "Sort/Algorythms/BinaryInsertion.h"
#include "Sort/Algorythms/QuickSort.h"
#include "Sort/Algorythms/Merge.h"

//---------------------

#define SIZE 1000
#define SCALE ((double) (SIZE*SIZE)/2)

Context* STHR_Context     = nullptr;
bool     STHR_Processing  = false;
bool     STHR_Abort       = false;
bool     STHR_Check       = true;
double   STHR_Progress    = 0;
int      STHR_CurrentSize = 0;

//---------------------

void DrawSortGraph   (Context* result);
void SortTest        ();
void DrawProgressBar (Context* context, int width, int height, Color background, Color left, Color right, double progress);
void SortStableTest  (Context* context);

//---------------------

void DrawSortGraph (Context* result)
{
	if (STHR_Processing) 
	{
		printf ("Sort is already running\n");
		return;
	}

	STHR_Context = result;

	std::thread thr (SortTest);
	thr.detach ();

	while (!STHR_Processing) Sleep (10);
}

//---------------------

void SortTest ()
{
	STHR_Processing = true;
	STHR_Abort      = false;

	protected_array <SIZE> arr = {};

	const int count = 150;

	const int size_x = STHR_Context -> getSizeX (); //1000;
	const int size_y = STHR_Context -> getSizeY (); //500;

	const int rnd_min = 0;
	const int rnd_max = 1000;

	const Color background   (11, 21, 31);
	const Color foreground = Color::White;
	
	const Color b_foreground  = Color::DarkCyan;
	const Color s_foreground  = Color::DarkPink;
	const Color i_foreground  = Color::Green;
	const Color bi_foreground = Color::Yellow;
	const Color q_foreground  = Color::Magneta;
	const Color m_foreground  = Color::Orange;

	const Color processing_background             = Color (16, 16, 16);
	const Color processing_progressbar_left       = Color::DarkPink;
	const Color processing_progressbar_right      = Color::DarkCyan;
	const Color processing_progressbar_background = Color::Black;

	const int r = 2;

	STHR_Context -> resize (size_x, size_y);

	Context context (size_x, size_y);
	context.clear   (background);

	context.setColor     (background, 1);
	context.setFillColor (background);
	RoundRect (context, 0, 0, size_x, size_y, 10, 10);

	double x_scale = (double) size_x / 3.0 / count;

	printf ("Sort started: array size = %d, points count = %zu\n", SIZE, count);

	LARGE_INTEGER start = {};
	QueryPerformanceCounter (&start);

	LARGE_INTEGER freq = {};
	QueryPerformanceFrequency (&freq);

	bool aborted = false;

	for (int i = 1; i <= count; i++)
	{
		int current_size = SIZE / count * i;
		STHR_CurrentSize = current_size;

		#define _TEST(func, compares_result, assignments_result, allocations_result) \
			FillRand (arr, current_size, rnd_min, rnd_max);				             \
			controllable::resetCompareCount    ();					                 \
			controllable::resetAssignmentCount ();					                 \
			controllable::resetAllocationCount ();									 \
			func (arr, current_size);									             \
			int compares_result    = controllable::getCompareCount    ();            \
			int assignments_result = controllable::getAssignmentCount ();            \
			int allocations_result = controllable::getAllocationCount ();			 \
			if (STHR_Check)												             \
			{																         \
				if (CheckSort (arr, current_size)) 							         \
				{ 															         \
					printf ("function '%s' check failed:\n", #func);		         \
					TestSort (arr, current_size);							         \
					break; 													         \
				}															         \
			}

		#define _DRAWPOINT(color, x, y)               \
			context.setColor     (color);             \
			context.setFillColor (color);             \
			txRectangle (x-r, y-r, x+r, y+r, context);

		#define _TEST_AND_DRAW(func, color, compares_scale, assignments_scale, allocations_scale)              \
		{																									   \
			_TEST (func, compares, assignments, allocations);	    										   \
			_DRAWPOINT (color,              i*x_scale, size_y - size_y/SCALE * compares   *compares_scale   ); \
			_DRAWPOINT (color, size_x/3   + i*x_scale, size_y - size_y/SCALE * assignments*assignments_scale); \
			_DRAWPOINT (color, size_x/3*2 +	i*x_scale, size_y - size_y/SCALE * allocations*allocations_scale); \
		}

		_TEST_AND_DRAW (SortBubble,          b_foreground,  1,  1,  100);
		_TEST_AND_DRAW (SortSelection,       s_foreground,  1,  10, 100);
		_TEST_AND_DRAW (SortInsertion,       i_foreground,  1,  1,  100);
		_TEST_AND_DRAW (SortBinaryInsertion, bi_foreground, 10, 1,  100);
		_TEST_AND_DRAW (QuickSort,           q_foreground,  10, 10, 100);
		_TEST_AND_DRAW (MergeSort,           m_foreground,  10, 10, 100);

		STHR_Progress = (double) 100.0/count * i;

		if (STHR_Abort) 
		{ 
			STHR_Abort = false;
			aborted = true;
			break; 
		}

		txSetProgress (STHR_Progress, TBPF_NORMAL);

		#undef _TEST
		#undef _DRAWPOINT
		#undef _TEST_AND_DRAW
	}

	LARGE_INTEGER stop = {};
	QueryPerformanceCounter (&stop);
	unsigned long long elapsed = 1000.0 * (double) (stop.QuadPart - start.QuadPart) / (double) freq.QuadPart;

	printf ("Sort elapsed %llu msec%s\n", elapsed, (aborted) ? " [ABORTED]" : "");

	int y = 0;
	#define _TEXTOUT(color, x, text)     \
		context.setColor     (color);    \
		context.setFillColor (color);    \
		txTextOut (x, y, text, context); \
		y+=25;
	
	context.setFont ("consolas", 25, 11, FW_BOLD);

	y = 0;
	_TEXTOUT (foreground,                6, "Compares              ");
	_TEXTOUT (b_foreground,             19, "BUBBLE                ");
	_TEXTOUT (s_foreground,             19, "SELECTION             ");
	_TEXTOUT (i_foreground,             19, "INSERTION             ");
	_TEXTOUT (bi_foreground,            19, "BINARY INSERTION [x10]");
	_TEXTOUT (q_foreground,             19, "QUICKSORT        [x10]");
	_TEXTOUT (m_foreground,             19, "MERGE            [x10]");

	y = 0;
	_TEXTOUT (foreground,    size_x/3 +  6, "Assignments           ");
	_TEXTOUT (b_foreground,  size_x/3 + 19, "BUBBLE                ");
	_TEXTOUT (s_foreground,  size_x/3 + 19, "SELECTION        [x10]");
	_TEXTOUT (i_foreground,  size_x/3 + 19, "INSERTION             ");
	_TEXTOUT (bi_foreground, size_x/3 + 19, "BINARY INSERTION      ");
	_TEXTOUT (q_foreground,  size_x/3 + 19, "QUICKSORT        [x10]");
	_TEXTOUT (m_foreground,  size_x/3 + 19, "MERGE            [x10]");

	y = 0;
	_TEXTOUT (foreground,    size_x/3*2 +  6, "Allocations            ");
	_TEXTOUT (b_foreground,  size_x/3*2 + 19, "BUBBLE           [x100]");
	_TEXTOUT (s_foreground,  size_x/3*2 + 19, "SELECTION        [x100]");
	_TEXTOUT (i_foreground,  size_x/3*2 + 19, "INSERTION        [x100]");
	_TEXTOUT (bi_foreground, size_x/3*2 + 19, "BINARY INSERTION [x100]");
	_TEXTOUT (q_foreground,  size_x/3*2 + 19, "QUICKSORT        [x100]");
	_TEXTOUT (m_foreground,  size_x/3*2 + 19, "MERGE            [x100]");

	context.setColor     (foreground, 2);
	context.setFillColor (foreground);

	txLine (size_x/3,   0, size_x/3,   size_y, context);
	txLine (size_x/3*2, 0, size_x/3*2, size_y, context);

	txLine (10, 25,  10, 164, context);
		txLine (10, 39,  16, 39,  context);
		txLine (10, 64,  16, 64,  context);
		txLine (10, 89,  16, 89,  context);
		txLine (10, 114, 16, 114, context);
		txLine (10, 139, 16, 139, context);
		txLine (10, 164, 16, 164, context);

	txLine (size_x/3 + 10, 25,  size_x/3 + 10, 164, context);
		txLine (size_x/3 + 10, 39,  size_x/3 + 16, 39,  context);
		txLine (size_x/3 + 10, 64,  size_x/3 + 16, 64,  context);
		txLine (size_x/3 + 10, 89,  size_x/3 + 16, 89,  context);
		txLine (size_x/3 + 10, 114, size_x/3 + 16, 114, context);
		txLine (size_x/3 + 10, 139, size_x/3 + 16, 139, context);
		txLine (size_x/3 + 10, 164, size_x/3 + 16, 164, context);

	txLine (size_x/3*2 + 10, 25,  size_x/3*2 + 10, 164, context);
		txLine (size_x/3*2 + 10, 39,  size_x/3*2 + 16, 39,  context);
		txLine (size_x/3*2 + 10, 64,  size_x/3*2 + 16, 64,  context);
		txLine (size_x/3*2 + 10, 89,  size_x/3*2 + 16, 89,  context);
		txLine (size_x/3*2 + 10, 114, size_x/3*2 + 16, 114, context);
		txLine (size_x/3*2 + 10, 139, size_x/3*2 + 16, 139, context);
		txLine (size_x/3*2 + 10, 164, size_x/3*2 + 16, 164, context);

	Context rect (size_x, size_y);
	rect.clear (Color::Black);

	rect.setColor     (Color::White);
	rect.setFillColor (Color::White);
	RoundRect (rect, 1, 1, size_x-1, size_y-1, 10, 10);

	for (int x = 0; x < size_x; x++)
		for (int y = 0; y < size_y; y++)
			if (rect.getPixel (x, y) != Color::White) context.setPixel (x, y, background);

	context.setColor     (Color::White, 2);
	context.setFillColor (Color::Transparent);
	RoundRect (context, 1, 1, size_x-1, size_y-1, 10, 10);

	txSetProgress (0, TBPF_NOPROGRESS);

	STHR_Processing = false;
	txSleep (10);

	context.render (*STHR_Context);

	#undef _TEXTOUT
}

//---------------------

void DrawProgressBar (Context* context, int width, int height, Color background, Color left, Color right, double progress)
{
	int size_x = context -> getSizeX ();
	int size_y = context -> getSizeY ();

	int src_x = size_x/2 - width /2;
	int src_y = size_y/2 - height/2+15;

	int current_width = (double) width / 100 * progress;

	context -> setColor     (background);
	context -> setFillColor (background);
	txRectangle (src_x, src_y, src_x + width, src_y + height, *context);

	for (int x = 0; x < current_width; x++)
	{
		double t = 1.0 / width * x;
		Color color = Color::Interpolate (left, right, t);

		context -> setColor     (color);
		context -> setFillColor (color);
		txRectangle (src_x+x, src_y, src_x+x + 1, src_y + height, *context);
	}
}

//---------------------

void SortStableTest (Context* context)
{
	int fsize_x = 25;
	int fsize_y = 50;

	const Color background (1, 41, 61);
	const Color foreground = Color::White;
				 
	const size_t arr_size = 20;

	int size_x = fsize_x*arr_size*2;
	int size_y = fsize_y*4;

	int rnd_min = 10;
	int rnd_max = 20;

	context -> resize (size_x, size_y);

	protected_array <arr_size> arr = {};
	FillRand (arr, arr_size, rnd_min, rnd_max);

	context -> clear (background);
	context -> setColor (foreground, 2);

	for (size_t i = 1; i < arr_size; i++)
	{
		int x = (double) size_x/arr_size * i;
		txLine (x, 0, x, size_y, *context);
	}

	txLine (0, size_y/4,        size_x, size_y/4,        *context);
	txLine (0, size_y/2,        size_x, size_y/2,        *context);
	txLine (0, size_y-size_y/4, size_x, size_y-size_y/4, *context);

	context -> setFont ("consolas", fsize_y/2, fsize_x/2, FW_BOLD);

	for (size_t i = 0; i < arr_size; i++)
	{
		int x = (double) size_x/arr_size * i;
		Color color = Color::HSV ((double) ((int) arr[i]-rnd_min) / (rnd_max-rnd_min) * 255, 255, 255);

		char str[MAX_PATH] = "";
		int len = sprintf_s (str, "%d", arr[i].operator int ());

		context -> setColor     (color);
		context -> setFillColor (color);
		txTextOut (x + len*fsize_x/4, fsize_y/4, str, *context);

		len = sprintf_s (str, "%d", arr[i].getPosition ());

		context -> setColor     (foreground);
		context -> setFillColor (foreground);
		txTextOut (x + fsize_x-fsize_x/2*len/2, size_y/2-size_y/4+fsize_y/4, str, *context);
	}

	MergeSort (arr);

	for (size_t i = 0; i < arr_size; i++)
	{
		int x = (double) size_x/arr_size * i;
		Color color = Color::HSV ((double) ((int) arr[i]-rnd_min) / (rnd_max-rnd_min) * 255, 255, 255);

		char str[MAX_PATH] = "";
		int len = sprintf_s (str, "%d", arr[i].operator int ());

		int y = size_y/2;

		context -> setColor     (color);
		context -> setFillColor (color);
		txTextOut (x + len*fsize_x/4, y+fsize_y/4, str, *context);

		len = sprintf_s (str, "%d", arr[i].getPosition ());

		context -> setColor     (foreground);
		context -> setFillColor (foreground);
		txTextOut (x + fsize_x-fsize_x/2*len/2, y+size_y/2-size_y/4+fsize_y/4, str, *context);
	}
}

//---------------------