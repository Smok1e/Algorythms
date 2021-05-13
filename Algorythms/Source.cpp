#include <TXLib.h>
#include <TXU.h>
#include "C:\Lib\FloatingWindow\FloatingWindow.h"

using namespace txu;

//-----------------------

#define TRACE(format, ...) 														 \
	printf ("[TRACE]: in function '%s' [at line #%d]: ", __FUNCSIG__, __LINE__); \
	printf (format, __VA_ARGS__);												 \
	printf ("\n");

//-----------------------

//#define FULLSCREEN

#include "UserInterface/Mouse.h"
#include "UserInterface/Buttons.h"
#include "UserInterface/ButtonManager.h"
#include "UserInterface/Interface.h"

#include "Sort/Graph/SortTest.h"

//-----------------------

void OnImageFloat (AppManager* mgr, Coord2D* pos, Coord2D mpos, Coord2D size, bool allow);

//-----------------------

int main ()
{
	const int window_indent_x = 500;
	const int window_indent_y = 200;

	int wsize_x = GetSystemMetrics (SM_CXSCREEN);
	int wsize_y = GetSystemMetrics (SM_CYSCREEN) - 30;

#ifndef FULLSCREEN
	wsize_x -= window_indent_x;
	wsize_y -= window_indent_y;
#endif

	int menu_width = 150;

	ShowWindow (GetConsoleWindow (), SW_HIDE);

	while (IsWindowVisible (GetConsoleWindow ())) txSleep (0);

	Context screen (GetSystemMetrics (SM_CXSCREEN), GetSystemMetrics (SM_CYSCREEN));
	txBitBlt (screen, 0, 0, 0, 0, GetDC (nullptr));

	AppManager manager (wsize_x, wsize_y,"Sort test");
	manager.init ();

	Context context (wsize_x - menu_width, wsize_y);

	int y     = -29;
	int fsize =  11;

	Color button_background        (32, 32, 32);
	Color button_active_background (64, 64, 64);
	Color button_text_color        (Color::White);

#define NEXTCOORD Coord2D (2, y += 30 + 1), Coord2D (menu_width - 4, 30 - 2)

	bool onreset = false;
	manager.buttons.addButton (new AnimatedTextButton (NEXTCOORD, "RESET", AnimatedTextButton::align::left, fsize, 
		                       button_background, button_active_background, button_text_color, button_text_color, &onreset));

	bool onresort = false;
	manager.buttons.addButton (new AnimatedTextButton (NEXTCOORD, "RESORT", AnimatedTextButton::align::left, fsize, 
		                       button_background, button_active_background, button_text_color, button_text_color, &onresort));

	bool onstop = false;
	manager.buttons.addButton (new AnimatedTextButton (NEXTCOORD, "STOP", AnimatedTextButton::align::left, fsize, 
		                       button_background, button_active_background, button_text_color, button_text_color, &onstop));

	bool onconsole = false;
	manager.buttons.addButton (new AnimatedCheckButton (NEXTCOORD, "CONSOLE", AnimatedTextButton::align::left, fsize,
	                           button_background, button_active_background, button_text_color, button_text_color, &onconsole));
	bool last_onconsole = !onconsole;

	manager.buttons.addButton (new AnimatedCheckButton (NEXTCOORD, "CHECK", AnimatedTextButton::align::left, fsize,
		                       button_background, button_active_background, button_text_color, button_text_color, &STHR_Check));

	bool consoleoutput = false;
	manager.buttons.addButton (new AnimatedCheckButton (NEXTCOORD, "OUTPUT", AnimatedTextButton::align::left, fsize, 
		                       button_background, button_active_background, button_text_color, button_text_color, &consoleoutput));
	bool last_consoleoutput = !consoleoutput;

	bool togglebackground = true;
	manager.buttons.addButton (new AnimatedCheckButton (NEXTCOORD, "BACKGROUND", AnimatedTextButton::align::left, fsize,
		                       button_background, button_active_background, button_text_color, button_text_color, &togglebackground));

	char fps_str[MAX_PATH] = "";
	manager.buttons.addButton (new AnimatedTextButton (Coord2D (2, wsize_y - 30), Coord2D (menu_width - 4, 30 - 2), fps_str, AnimatedTextButton::align::center, fsize,
		                       button_background, button_background, button_text_color, button_text_color, nullptr));

#undef NEXTCOORD

	Context image (wsize_x - menu_width*2, wsize_y-300);
	DrawSortGraph (&image);

	Coord2D img_pos ((wsize_x-menu_width) / 2 - image.getSizeX () / 2, wsize_y / 2 - image.getSizeY () / 2);

	const Color background (11, 21, 31);

	RECT window = {};

	txBegin ();
	while (!manager.closed ())
	{
		GetWindowRect (txWindow (), &window);

		manager.context.clear (Color (24, 24, 24));
		context.clear (background);

		if (!togglebackground)
			txBitBlt (context, 0, 0, 0, 0, screen, window.left + menu_width, window.top + 30),
			context.setColor     (Color (24, 24, 24), 10),
			context.setFillColor (Color::Transparent),
			txRectangle (-2, -2, context.getSizeX (), context.getSizeY (), context);

		Coord2D mouse = manager.mouse.getPosition ();
		mouse.x -= menu_width;

		if (STHR_Processing)
		{
			const int fsizey = 30;

			image.clear (background);

			image.setColor     (Color (16, 16, 16));
			image.setFillColor (Color (16, 16, 16));
			RoundRect (image, 1, 1, image.getSizeX ()-1, image.getSizeY ()-1, 10, 10);

			DrawProgressBar (&image, 800, 5, Color::Black, Color::DarkPink, Color::DarkCyan, STHR_Progress);

			image.setColor     (Color::White);
			image.setFillColor (Color::White);
			image.setFont ("consolas", fsizey, fsizey / 2, FW_BOLD);
			
			char text[MAX_PATH]	= "";
			sprintf_s (text, "Testing sort on %d of %d elements [%.1lf%%]", STHR_CurrentSize, SIZE, STHR_Progress);

			int x = image.getSizeX ()/2 - strlen (text)*fsizey/2 / 2;
			int y = image.getSizeY ()/2 - fsizey / 2 - fsizey/2;

			txTextOut (x, y, text, image);
		}

		bool allow_moving = !manager.update  ();

		image.render (context, txCoord (img_pos));
		OnImageFloat (&manager, &img_pos, mouse, Coord2D (image.getSizeX (), image.getSizeY ()), allow_moving);

		if (onreset)
			img_pos = Coord2D ((wsize_x-menu_width) / 2 - image.getSizeX () / 2, wsize_y / 2 - image.getSizeY () / 2), onreset = false;

		if (onresort)
			DrawSortGraph (&image), onresort = false;

		if (onstop)
			STHR_Abort = true, onstop = false;

		if (onconsole != last_onconsole)
		{
			ShowWindow (GetConsoleWindow (), onconsole? SW_SHOW: SW_HIDE);
			last_onconsole = onconsole;
		}

		if (consoleoutput != last_consoleoutput)
		{
			SetConsoleOutputEnabled (consoleoutput);
			last_consoleoutput = consoleoutput;
		}

		int fps = txGetFPS ();
		sprintf_s (fps_str, "%3d fps", fps);
		if (fps > 0) AnimatedGradientButton::setAnimationSpeed (400.0 / fps);

		context.render (manager.context, menu_width);
		manager.display ();

		txSleep (0);
	}

	long long start = GetTickCount ();

	STHR_Abort = true;
	while (STHR_Processing) 
	{
		Sleep (10);

		if (GetTickCount () - start > 1000)
		{
			MessageBoxA (txWindow (), "Processing timed out", "Error", MB_ICONERROR | MB_OK);
			return 0;
		}
	}
}

//-----------------------

void OnImageFloat (AppManager* mgr, Coord2D* pos, Coord2D mpos, Coord2D size, bool allow)
{
	static bool    moving        = false;
	static Coord2D startmousepos = {};
	static Coord2D startimagepos = {};

	if (mgr -> mouse.hasStateChanged () && allow)
	{
		bool intersect = mpos.x >= pos -> x && mpos.x < pos -> x + size.x && mpos.y >= pos -> y && mpos.y < pos -> y + size.y;

		if (mgr -> mouse.getState () == Mouse::State::RightButtonPressed && intersect && GetForegroundWindow () == txWindow ())
		{
			startmousepos = mpos;
			startimagepos = *pos;
			moving = true;
		}

		else
			moving = false;
	}

	if (moving)
	{
		Coord2D diff = mpos - startmousepos;
		*pos = startimagepos + diff;
	}
}

//-----------------------