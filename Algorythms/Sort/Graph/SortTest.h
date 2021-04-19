#pragma once

//---------------------

#include "Sort/Controllable.h"
#include "Sort/ProtectedArray.h"
#include "Sort/CheckSort.h"
#include "Sort/FillRand.h"

#include "Sort/Algorythms/Bubble.h"
#include "Sort/Algorythms/Selection.h"
#include "Sort/Algorythms/Insertion.h"
#include "Sort/Algorythms/BinaryInsertion.h"
#include "Sort/Algorythms/QuickSort.h"

//---------------------

#define SIZE 2000
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

//---------------------

void DrawSortGraph (Context* result)
{
	if (STHR_Processing) return;

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

	const int size_x = 1800;
	const int size_y = 800;

	const Color background (1, 41, 61);
	const Color foreground = Color::White;
	
	const Color b_foreground  = Color::DarkCyan;
	const Color s_foreground  = Color::DarkPink;
	const Color i_foreground  = Color::Green;
	const Color bi_foreground = Color::Yellow;
	const Color q_foreground  = Color::Magneta;

	const Color processing_background             = Color (16, 16, 16);
	const Color processing_progressbar_left       = Color::DarkPink;
	const Color processing_progressbar_right      = Color::DarkCyan;
	const Color processing_progressbar_background = Color::Black;

	const int progressbar_width  = 800;
	const int progressbar_height =  20;

	const int r = 5;

	STHR_Context -> resize (size_x, size_y);

	Context context            (size_x, size_y);
	context.clear            (background);

	double x_scale = size_x / 2.0 / count;

	for (int i = 0; i < count; i++)
	{
		int current_size = SIZE / count * i;
		STHR_CurrentSize = current_size;

		#define _TEST(func, compares_result, assignments_result)               \
			FillRand (arr, current_size);								       \
			controllable::resetCompareCount    ();						       \
			controllable::resetAssignmentCount ();						       \
			func (arr, current_size);									       \
			int compares_result    = controllable::getCompareCount    ();      \
			int assignments_result = controllable::getAssignmentCount ();      \
			if (STHR_Check)												       \
			{																   \
				if (CheckSort (arr, current_size)) 							   \
				{ 															   \
					printf ("function '%s' check failed:\n", #func);		   \
					TestSort (arr, current_size);							   \
					break; 													   \
				}															   \
			}

		_TEST (SortBubble,          b_compares,  b_assignments );
		_TEST (SortSelection,       s_compares,  s_assignments );
		_TEST (SortInsertion,       i_compares,  i_assignments );
		_TEST (SortBinaryInsertion, bi_compares, bi_assignments);
		_TEST (QuickSort,           q_compares,  q_assignments );

		#define _DRAWPOINT(color, x, y)             \
			context.setColor     (color);           \
			context.setFillColor (color);           \
			txEllipse (x-r, y-r, x+r, y+r, context);

		_DRAWPOINT (b_foreground,             i*x_scale, size_y - size_y/SCALE*b_compares    );
		_DRAWPOINT (s_foreground,             i*x_scale, size_y - size_y/SCALE*s_compares    );
		_DRAWPOINT (i_foreground,             i*x_scale, size_y - size_y/SCALE*i_compares    );
		_DRAWPOINT (bi_foreground,            i*x_scale, size_y - size_y/SCALE*bi_compares*10);
		_DRAWPOINT (q_foreground,             i*x_scale, size_y - size_y/SCALE*q_compares *10);

		_DRAWPOINT (b_foreground,  size_x/2 + i*x_scale, size_y - size_y/SCALE*b_assignments   );
		_DRAWPOINT (s_foreground,  size_x/2 + i*x_scale, size_y - size_y/SCALE*s_assignments*10);
		_DRAWPOINT (i_foreground,  size_x/2 + i*x_scale, size_y - size_y/SCALE*i_assignments   );
		_DRAWPOINT (bi_foreground, size_x/2 + i*x_scale, size_y - size_y/SCALE*bi_assignments  );
		_DRAWPOINT (q_foreground,  size_x/2 + i*x_scale, size_y - size_y/SCALE*q_assignments*10);

		STHR_Progress = (double) 100.0/count * i;

		if (STHR_Abort) 
		{ 
			STHR_Abort = false;
			break; 
		}

		#undef _TEST
		#undef _DRAWPOINT
	}

	int y = 0;
	#define _TEXTOUT(color, x, text)     \
		context.setColor     (color);    \
		context.setFillColor (color);    \
		txTextOut (x, y, text, context); \
		y+=25;
	
	context.setFont ("consolas", 25, 11, FW_BOLD);

	y = 0;
	_TEXTOUT (foreground,                6, "Compares"              );
	_TEXTOUT (b_foreground,             19, "Bubble"                );
	_TEXTOUT (s_foreground,             19, "Selection"             );
	_TEXTOUT (i_foreground,             19, "Insertion"             );
	_TEXTOUT (bi_foreground,            19, "Binary insertion (x10)");
	_TEXTOUT (q_foreground,             19, "Quicksort (x10)"       );

	y = 0;
	_TEXTOUT (foreground,    size_x/2 +  6, "Assignments"     );
	_TEXTOUT (b_foreground,  size_x/2 + 19, "Bubble"          );
	_TEXTOUT (s_foreground,  size_x/2 + 19, "Selection"       );
	_TEXTOUT (i_foreground,  size_x/2 + 19, "Insertion"       );
	_TEXTOUT (bi_foreground, size_x/2 + 19, "Binary insertion");
	_TEXTOUT (q_foreground,  size_x/2 + 19, "Quicksort (x10)" );

	context.setColor     (foreground, 2);
	context.setFillColor (foreground);
	txLine (size_x/2, 0, size_x/2, size_y, context);

	txLine (10, 25,  10, 139, context);
	txLine (10, 39,  16, 39,  context);
	txLine (10, 64,  16, 64,  context);
	txLine (10, 89,  16, 89,  context);
	txLine (10, 114, 16, 114, context);
	txLine (10, 139, 16, 139, context);

	txLine (size_x/2 + 10, 25,  size_x/2 + 10, 139, context);
	txLine (size_x/2 + 10, 39,  size_x/2 + 16, 39,  context);
	txLine (size_x/2 + 10, 64,  size_x/2 + 16, 64,  context);
	txLine (size_x/2 + 10, 89,  size_x/2 + 16, 89,  context);
	txLine (size_x/2 + 10, 114, size_x/2 + 16, 114, context);
	txLine (size_x/2 + 10, 139, size_x/2 + 16, 139, context);

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
		Color color = InterpolateColor (left, right, t);

		context -> setColor     (color);
		context -> setFillColor (color);
		txRectangle (src_x+x, src_y, src_x+x + 1, src_y + height, *context);
	}
}

//---------------------