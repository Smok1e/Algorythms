#pragma once

//------------------------------

class Mouse 
{

public :

	enum class State
	{
		None,
		RightButtonPressed,
		LeftButtonPressed,
		WheelPressed
	};

	Mouse (Coord2D position_addition);
	Mouse ();

	void update ();

	void    setPositionAddition (Coord2D addition);
	Coord2D getPositionAddition ();

	Mouse::State getState ();
	Coord2D      getPosition   ();
	int          getWheel ();

	bool hasStateChanged    ();
	bool hasPositionChanged ();
	bool hasWheelMoved      ();

private :

	Coord2D position_addition_;

	Mouse::State state_;
	Mouse::State last_state_;

	Coord2D position_;
	Coord2D last_position_;

	int wheel_delta_;
	int last_wheel_delta_;

};

//------------------------------

Mouse::State GetMouseState ()
{
	int buttons = txMouseButtons ();
	switch (buttons)
	{
		case 1:
			return Mouse::State::RightButtonPressed;
		case 2:
			return Mouse::State::LeftButtonPressed;
		case 3:
			return Mouse::State::WheelPressed;
		default :
			return Mouse::State::None;
	}
}

//------------------------------

Mouse::Mouse (Coord2D position_addition) :
	position_addition_ (position_addition),

	state_      (Mouse::State::None),
	last_state_ (state_),

	position_      (txMousePos ()),
	last_position_ (position_),

	wheel_delta_      (GetMouseWheel ()),
	last_wheel_delta_ (wheel_delta_)
{}

Mouse::Mouse () :
	Mouse (Coord2D (0, 0))
{}

//------------------------------

void Mouse::update ()
{
	last_state_       = state_;
	last_position_    = position_;
	last_wheel_delta_ = wheel_delta_;
						
	state_       = GetMouseState ();
	position_    = txMousePos    ();
	wheel_delta_ = GetMouseWheel ();
}

//------------------------------

void Mouse::setPositionAddition (Coord2D addition)
{
	position_addition_ = addition;
}

Coord2D Mouse::getPositionAddition ()
{
	return position_addition_;
}

//------------------------------

Mouse::State Mouse::getState ()
{
	return state_;
}

Coord2D Mouse::getPosition ()
{
	return position_ + position_addition_;
}

int Mouse::getWheel ()
{
	return wheel_delta_;
}

//------------------------------

bool Mouse::hasStateChanged ()
{
	return last_state_ != state_;
}

bool Mouse::hasPositionChanged ()
{
	return last_position_.x != position_.x || last_position_.y != position_.y;
}

bool Mouse::hasWheelMoved ()
{
	return last_wheel_delta_ != wheel_delta_;
}

//------------------------------