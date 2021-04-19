#include <TXProject.h>
#include <thread>

//-----------------------

#define TRACE(format, ...) 														 \
	printf ("[TRACE]: in function '%s' [at line #%d]: ", __FUNCSIG__, __LINE__); \
	printf (format, __VA_ARGS__);												 \
	printf ("\n");

//-----------------------

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

	int wsize_x = GetSystemMetrics (SM_CXSCREEN) - 100;
	int wsize_y = GetSystemMetrics (SM_CYSCREEN) - 100;

	int menu_width = 150;

	AppManager manager (wsize_x, wsize_y, "Sort test");
	manager.init ();

	Context context (wsize_x - menu_width, wsize_y);

	AnimatedGradientButton::setAnimationSpeed (1.75);

	int y = -30;

#define NEXTCOORD Coord2D (0, y += 30), Coord2D (menu_width, 30)

	bool onreset = false;
	manager.buttons.addButton (new AnimatedTextButton (NEXTCOORD, "RESET", 13, 
		                       Color (24, 24, 24), Color (64, 64, 64), Color::White, Color::White, &onreset));

	bool onresort = false;
	manager.buttons.addButton (new AnimatedTextButton (NEXTCOORD, "RESORT", 13, 
		                       Color (24, 24, 24), Color (64, 64, 64), Color::White, Color::White, &onresort));

	bool onstop = false;
	manager.buttons.addButton (new AnimatedTextButton (NEXTCOORD, "STOP", 13, Color (24, 24, 24), Color (64, 64, 64), Color::White, Color::White, &onstop));

	bool onconsole = false;
	manager.buttons.addButton (new AnimatedCheckButton (NEXTCOORD, "CONSOLE", 13,
	                           Color (24, 24, 24), Color (64, 64, 64), Color::White, Color::White, &onconsole));
	bool last_onconsole = !onconsole;

	manager.buttons.addButton (new AnimatedCheckButton (NEXTCOORD, "CHECK", 13,
		                       Color (24, 24, 24), Color (64, 64, 64), Color::White, Color::White, &STHR_Check));

#undef NEXTCOORD

	Context image;
	DrawSortGraph (&image);

	Coord2D img_pos ((wsize_x-menu_width) / 2 - image.getSizeX () / 2, wsize_y / 2 - image.getSizeY () / 2);

	txBegin ();
	while (!manager.closed ())
	{
		manager.context.clear (Color (24, 24, 24));
		        context.clear (Color (11, 21, 31));

		Coord2D mouse = manager.mouse.getPosition ();
		mouse.x -= menu_width;

		if (STHR_Processing)
		{
			const int fsizey = 30;

			image.clear (Color (16, 16, 16));
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

		context.render (manager.context, menu_width);
		manager.display ();

		txSleep (0);
	}

	STHR_Abort = true;
	while (STHR_Processing) Sleep (10);
}

//-----------------------

void OnImageFloat (AppManager* mgr, Coord2D* pos, Coord2D mpos, Coord2D size, bool allow)
{
	static bool    moving        = false;
	static Coord2D startmousepos = {};
	static Coord2D startimagepos = {};

	if (mgr -> mouse.hasStateChanged () && allow)
	{
		if (mgr -> mouse.getState () == Mouse::State::RightButtonPressed && Intersect (mpos, *pos, size) && GetForegroundWindow () == txWindow ())
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