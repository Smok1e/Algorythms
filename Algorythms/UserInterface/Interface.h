#pragma once

//-----------------------

#define ICON_FILENAME "icon.ico"

//-----------------------

class AppManager

{

public:

	Context       context;
	Mouse         mouse;
	ButtonManager buttons;

	AppManager (int size_x, int size_y, const char * title);

	void init ();

	void display ();
	bool update  ();

	void         setTitle (const char * title);
	const char * getTitle ();

	void  setForegrounColor (Color color);
	Color getForegroundColor ();

	void setCaptionHeight (int height);
	int  getCaptionHeight ();

	bool closed ();

private:

	ButtonManager menu_buttons_;

	const char * title_;

	int size_x_;
	int size_y_;

	Context screen_;

	Color background_;
	Color title_background_;
	Color foreground_;

	int caption_height_;

	bool on_exit_;
	bool on_hide_;

	bool closed_;

	void onDrawTitle ();

};

//-----------------------

AppManager::AppManager (int size_x, int size_y, const char * title) :
	size_x_ (size_x),
	size_y_ (size_y),

	background_       (24, 24, 24),
	title_background_ (16, 16, 16),
	foreground_       (Color::White),

	context (size_x, size_y),
	screen_ (),

	caption_height_ (30),

	buttons       (),
	menu_buttons_ (),

	mouse (),

	title_ (title),

	on_exit_ (false),
	on_hide_ (false),

	closed_ (false)
{
	mouse.setPositionAddition (Coord2D (0, -caption_height_));
	screen_.resize (size_x_, size_y_ + caption_height_);
}

//-----------------------

void AppManager::init ()
{
	menu_buttons_.addButton (new ExitButton (Coord2D (size_x_ - caption_height_,   0), Coord2D (caption_height_, caption_height_), &on_exit_, title_background_, Color::DarkPink));
	menu_buttons_.addButton (new HideButton (Coord2D (size_x_ - caption_height_*2, 0), Coord2D (caption_height_, caption_height_), &on_hide_, title_background_, Color::DarkCyan));

	_txConsoleMode = SW_SHOW;
	_txWindowStyle = WS_NOFRAME;
	txDisableAutoPause ();
	txCreateWindow (size_x_, size_y_ + caption_height_);

	HICON icon = (HICON) LoadImageA (NULL, ICON_FILENAME, IMAGE_ICON, 32, 32, LR_LOADFROMFILE | LR_SHARED);

	SendMessage (txWindow (), WM_SETICON, ICON_SMALL, (LPARAM) icon);
	SendMessage (txWindow (), WM_SETICON, ICON_BIG,   (LPARAM) icon);
	SetActiveWindow (txWindow ());
}

//-----------------------

void AppManager::display ()
{
	screen_.clear (background_);
	onDrawTitle ();
	buttons.drawButtons (&context);
	context.render (screen_, 0, caption_height_);
	menu_buttons_.drawButtons (&screen_);
	screen_.render ();
}

//-----------------------

bool AppManager::update ()
{
	mouse.update ();
	
	Coord2D addition = mouse.getPositionAddition ();
	mouse.setPositionAddition (Coord2D (0, 0));
	bool res = menu_buttons_.processButtons (&mouse);
	mouse.setPositionAddition (addition);

	res = res || buttons.processButtons (&mouse);
	
	OnWindowFloat (mouse.getPosition ().y < 0 && !res);
	
	menu_buttons_.onTimeTick ();
	buttons.onTimeTick ();

	if (on_hide_)
		ShowWindow (txWindow (), SW_MINIMIZE), on_hide_ = false;

	if (on_exit_)
		closed_ = true;

	closed_ = closed_ || _txWindowClosed;

	return res;
}

//-----------------------

void AppManager::setTitle (const char * title)
{
	title_ = title;
}

const char * AppManager::getTitle ()
{
	return title_;
}

//-----------------------

void AppManager::setForegrounColor (Color color)
{
	foreground_ = color;
}

Color AppManager::getForegroundColor ()
{
	return foreground_;
}

//-----------------------

void AppManager::setCaptionHeight (int height)
{
	caption_height_ = height;
}

int AppManager::getCaptionHeight ()
{
	return caption_height_;
}

//-----------------------

bool AppManager::closed ()
{
	return closed_;
}

//-----------------------

void AppManager::onDrawTitle ()
{
	screen_.setColor     (title_background_);
	screen_.setFillColor (title_background_);

	txRectangle (0, 0, size_x_, caption_height_, screen_);

	double fontsize_y = caption_height_ - 2;
	double fontsize_x = fontsize_y / 2;

	screen_.setColor     (foreground_);
	screen_.setFillColor (foreground_);
	screen_.setFont ("calibri", fontsize_y, fontsize_x);

	txTextOut (5, caption_height_ / 2 - fontsize_y / 2, title_, screen_);
}

//-----------------------