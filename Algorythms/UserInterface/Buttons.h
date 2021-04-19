#pragma once

//------------------------------

class AbstractButton 
{

public :

	AbstractButton (Coord2D position, Coord2D size, bool * parameter);
	AbstractButton ();

	virtual void    setPosition (Coord2D position);
	virtual Coord2D getPosition ();

	virtual void    setSize (Coord2D size);
	virtual Coord2D getSize ();

	virtual void  setParameterPointer (bool * parameter);
	virtual bool* getParameterPointer ();

	virtual bool onMouseTest  (Coord2D mouse);
	virtual bool onMouseClick ();
	virtual void onTimeTick   ();

	virtual void onDraw (Context * context);

protected :

	Coord2D position_;
	Coord2D size_;

	bool * parameter_;

};

//------------------------------

class AnimatedGradientButton : public AbstractButton
{

public :

	AnimatedGradientButton (Coord2D position, Coord2D size, Color color1, Color color2, bool * parameter);
	AnimatedGradientButton ();

	virtual void  setColor1 (Color color);
	virtual Color getColor1 ();

	virtual void  setColor2 (Color color);
	virtual Color getColor2 ();

	static void   setAnimationSpeed (double speed);
	static double getAnimationSpeed ();

	virtual bool onMouseTest (Coord2D mouse);
	virtual void onTimeTick ();

	virtual void onDraw (Context * context);

protected :

	bool mousehover_;

	Color color1_;
	Color color2_;

	double t_;

	static double delta_t;

};

double AnimatedGradientButton::delta_t = 1;

//------------------------------

class ExitButton : public AnimatedGradientButton
{

public :

	ExitButton (Coord2D coord, Coord2D size, bool * parameter, Color background, Color hover);
	ExitButton (Coord2D coord, Coord2D size, bool * parameter);
	ExitButton ();

	virtual void onDraw (Context * context);

protected :

};

//------------------------------

class HideButton : public AnimatedGradientButton
{

public :

	HideButton (Coord2D coord, Coord2D size, bool * parameter, Color background, Color hover);
	HideButton (Coord2D coord, Coord2D size, bool * parameter);
	HideButton ();

	virtual void onDraw (Context * context);

protected :

};

//------------------------------

class AnimatedTextButton : public AnimatedGradientButton
{

public :

	AnimatedTextButton (Coord2D position, Coord2D size, const char * text, int fontsize, Color background, Color hover, Color textcolor, Color hovertextcolor, bool * parameter);
	AnimatedTextButton ();

	virtual void onDraw (Context * context);

protected :

	Color textcolor_;
	Color hovertextcolor_;

	const char * text_;

	int fontsize_;

};

//------------------------------

class AnimatedCheckButton : public AnimatedTextButton
{

public :

	AnimatedCheckButton (Coord2D position, Coord2D size, const char * text, int fontsize, Color background, Color hover, Color textcolor, Color hovertextcolor, bool * parameter);
	AnimatedCheckButton ();

	virtual void onTimeTick ();

	virtual double getT ();

	virtual void onDraw (Context * context);

protected :

	double t1_;

};

//------------------------------

AbstractButton::AbstractButton (Coord2D position, Coord2D size, bool * parameter) :
	position_  (position),
	size_      (size),
	parameter_ (parameter)
{}

AbstractButton::AbstractButton () :
	AbstractButton (Coord2D (0, 0), Coord2D (0, 0), nullptr)
{}

//------------------------------

void AbstractButton::setPosition (Coord2D position)
{
	position_ = position;
}

Coord2D AbstractButton::getPosition ()
{
	return position_;
}

//------------------------------

void AbstractButton::setSize (Coord2D size)
{
	size_ = size;
}

Coord2D AbstractButton::getSize ()
{
	return size_;
}

//------------------------------

void AbstractButton::setParameterPointer (bool * parameter)
{
	parameter_ = parameter;
}

bool* AbstractButton::getParameterPointer ()
{
	return parameter_;
}

//------------------------------

bool AbstractButton::onMouseTest (Coord2D mouse)
{
	return mouse.x >= position_.x && mouse.y >= position_.y && mouse.x < position_.x + size_.x && mouse.y < position_.y + size_.y;
}

bool AbstractButton::onMouseClick ()
{
	*parameter_ = !*parameter_;
	return true;
}

void AbstractButton::onTimeTick ()
{

}

//------------------------------

void AbstractButton::onDraw (Context * context)
{

	context -> setColor     (Color::White, 1);
	context -> setFillColor (Color::Red     );
	txRectangle (txCoord (position_), txCoord (position_ + size_), *context);

	const char * text = "Abstract Button";
	int len = strlen (text);

	double fontsize_x = size_.x / (len+2);
	double fontsize_y = fontsize_x * 2;
	
	double textsize_x = fontsize_x * len;
	double textsize_y = fontsize_y;

	context -> setColor     (Color::White);
	context -> setFillColor (Color::White);
	context -> setFont ("consolas", fontsize_y, fontsize_x, FW_BOLD);
	txTextOut (position_.x + size_.x / 2 - textsize_x / 2, position_.y + size_.y / 2 - textsize_y / 2, text, *context);

}

//------------------------------

AnimatedGradientButton::AnimatedGradientButton (Coord2D position, Coord2D size, Color color1, Color color2, bool* parameter) :
	AbstractButton (position, size, parameter),
	color1_ (color1),
	color2_ (color2),
	t_ (0),
	mousehover_ (false)
{}

AnimatedGradientButton::AnimatedGradientButton () :
	AnimatedGradientButton (Coord2D (0, 0), Coord2D (0, 0), Color::White, Color::White, nullptr)
{}

//------------------------------

void AnimatedGradientButton::setColor1 (Color color)
{
	color1_ = color;
}

Color AnimatedGradientButton::getColor1 ()
{
	return color1_;
}

void AnimatedGradientButton::setColor2 (Color color)
{
	color2_ = color;
}

Color AnimatedGradientButton::getColor2 ()
{
	return color2_;
}

//------------------------------

void AnimatedGradientButton::setAnimationSpeed (double speed)
{
	delta_t = speed;
}

double AnimatedGradientButton::getAnimationSpeed ()
{
	return delta_t;
}

//------------------------------

bool AnimatedGradientButton::onMouseTest (Coord2D mouse)
{
	mousehover_ = AbstractButton::onMouseTest (mouse);
	return mousehover_;
}

void AnimatedGradientButton::onTimeTick ()
{
	t_ = Clump (t_ + delta_t * 0.025 * ((mousehover_ == 1) - (mousehover_ == 0)), 0, 1);
}

//------------------------------

void AnimatedGradientButton::onDraw (Context * context)
{
	Color color = InterpolateColor (color1_, color2_, sin (t_));

	context -> setColor     (color);
	context -> setFillColor (color);
	txRectangle (txCoord (position_), txCoord (position_ + size_), *context);
}

//------------------------------

ExitButton::ExitButton (Coord2D coord, Coord2D size, bool * parameter, Color background, Color hover) :
	AnimatedGradientButton (coord, size, background, hover, parameter)
{}

ExitButton::ExitButton (Coord2D coord, Coord2D size, bool * parameter) :
	ExitButton (coord, size, parameter, Color (16, 16, 16), Color::DarkPink)
{}

ExitButton::ExitButton () :
	ExitButton (Coord2D (0, 0), Coord2D (0, 0), nullptr)
{}

//------------------------------

void ExitButton::onDraw (Context * context)
{
	AnimatedGradientButton::onDraw (context);
	
	const int width  = 2;
	const int indent = width*3;

	context -> setColor     (Color::White, width);
	context -> setFillColor (Color::White);

	txLine (position_.x + indent, position_.y + indent, position_.x + size_.x - indent, position_.y + size_.y - indent, *context);
	txLine (position_.x + size_.x - indent, position_.y + indent, position_.x + indent, position_.y + size_.y - indent, *context);
}

//------------------------------

HideButton::HideButton (Coord2D coord, Coord2D size, bool * parameter, Color background, Color hover) :
	AnimatedGradientButton (coord, size, background, hover, parameter)
{}

HideButton::HideButton (Coord2D coord, Coord2D size, bool * parameter) :
	HideButton (coord, size, parameter, Color (16, 16, 16), Color::DarkCyan)
{}

HideButton::HideButton () :
	HideButton (Coord2D (0, 0), Coord2D (0, 0), nullptr)
{}

//------------------------------

void HideButton::onDraw (Context * context)
{
	AnimatedGradientButton::onDraw (context);
	
	const int width  = 2;
	const int indent = width*3;

	context -> setColor     (Color::White, width);
	context -> setFillColor (Color::White);

	txRectangle (position_.x + indent, position_.y + size_.y / 2, position_.x + size_.x - indent, position_.y + size_.y / 2 + 1, *context);

}

//------------------------------

AnimatedTextButton::AnimatedTextButton (Coord2D position, Coord2D size, const char * text, int fontsize, Color background, Color hover, Color textcolor, Color hovertextcolor, bool * parameter) :
	AnimatedGradientButton (position, size, background, hover, parameter),
	
	textcolor_      (textcolor),
	hovertextcolor_ (hovertextcolor),

	text_ (text),

	fontsize_ (fontsize)
{}

//------------------------------

AnimatedTextButton::AnimatedTextButton () :
	AnimatedTextButton (Coord2D (0, 0), Coord2D (0, 0), "", 0, 0x0, 0x0, 0x0, 0x0, nullptr)
{}

//------------------------------

void AnimatedTextButton::onDraw (Context * context)
{
	AnimatedGradientButton::onDraw (context);

	int len = strlen (text_);

	double fontsize_x = fontsize_;
	double fontsize_y = fontsize_x * 2.2;
	
	double textsize_x = fontsize_x * len;
	double textsize_y = fontsize_y;

	Color color = InterpolateColor (textcolor_, hovertextcolor_, t_);

	context -> setColor     (color);
	context -> setFillColor (color);
	context -> setFont ("consolas", fontsize_y, fontsize_x, FW_BOLD);
	txTextOut (position_.x + 10, position_.y + size_.y / 2 - textsize_y / 2, text_, *context);
}

//------------------------------

AnimatedCheckButton::AnimatedCheckButton (Coord2D position, Coord2D size, const char * text, int fontsize, Color background, Color hover, Color textcolor, Color hovertextcolor, bool * parameter) :
	AnimatedTextButton (position, size, text, fontsize, background, hover, textcolor, hovertextcolor, parameter),
	t1_ (*parameter)
{}

//------------------------------

AnimatedCheckButton::AnimatedCheckButton () :
	AnimatedCheckButton (Coord2D (0, 0), Coord2D (0, 0), "", 0, 0x0, 0x0, 0x0, 0x0, nullptr)
{}

//------------------------------

void AnimatedCheckButton::onTimeTick ()
{
	AnimatedGradientButton::onTimeTick ();
	t1_ = Clump (t1_ + AnimatedGradientButton::delta_t * 0.03 * ((*parameter_ == 1) - (*parameter_ == 0)), 0, 1);
}

//------------------------------

double AnimatedCheckButton::getT ()
{
	return t1_;
}

//------------------------------

void AnimatedCheckButton::onDraw (Context * context)
{
	AnimatedTextButton::onDraw (context);

	//Color color = InterpolateColor (Color::DarkPink, Color::DarkCyan, t1_);
	//context -> setColor     (color);
	//context -> setFillColor (color);

	const int r = 3;
	Coord2D pos = position_ + Coord2D (size_.x - r*4, size_.y/2);

	context -> setColor     (Color::DarkPink);
	context -> setFillColor (Color::DarkPink);
	txRectangle (txCoord (pos - r), txCoord (pos + r), *context);

	context -> setColor     (Color::DarkCyan);
	context -> setFillColor (Color::DarkCyan);
	txRectangle (pos.x-r, pos.y-r, pos.x-r + t1_*r*2, pos.y+r, *context);
}

//------------------------------