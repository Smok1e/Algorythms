#pragma once

//------------------------------

class ButtonManager
{

public :

	 ButtonManager ();
	~ButtonManager ();

	void            addButton    (AbstractButton* button);
	AbstractButton* getButton    (size_t index);
	int             findButton   (AbstractButton* button);
	bool            removeButton (size_t index);
	bool            removeButton (AbstractButton* button);

	size_t getButtonsCount ();

	bool processButtons (Mouse   * mouse  );
	void drawButtons    (Context * context);
	void onTimeTick     ();

private :

	std::vector <AbstractButton*> buttons_;

};

//------------------------------

ButtonManager::ButtonManager () :
	buttons_ ()
{}

ButtonManager::~ButtonManager ()
{
	size_t count = buttons_.size ();
	for (size_t i = 0; i < count; i++)
		delete (buttons_[i]);
}

//------------------------------

void ButtonManager::addButton (AbstractButton* button)
{
	buttons_.push_back (button);
}

AbstractButton* ButtonManager::getButton (size_t index)
{
	if (buttons_.size () < index) return nullptr;
	return buttons_[index];
}

int ButtonManager::findButton (AbstractButton* button)
{
	size_t count = buttons_.size ();
	for (size_t i = 0; i < count; i++)
		if (buttons_[i] == button) return i;
	return -1;
}

bool ButtonManager::removeButton (size_t index)
{
	if (buttons_.size () < index) return false;
	delete (buttons_[index]);
	buttons_.erase (buttons_.begin () + index);
	return true;
}

bool ButtonManager::removeButton (AbstractButton* button)
{
	int index = findButton (button);
	if (index < 0) return false;
	return removeButton (static_cast <size_t> (index));
}

//------------------------------

size_t ButtonManager::getButtonsCount ()
{
	return buttons_.size ();
}

//------------------------------

bool ButtonManager::processButtons (Mouse * mouse)
{
	Coord2D position = mouse -> getPosition ();
		
	size_t count = buttons_.size ();
	for (size_t i = 0; i < count; i++)
	{
		AbstractButton* button = buttons_[i];
		if (button -> onMouseTest (position) && mouse -> hasStateChanged () && mouse -> getState () == Mouse::State::RightButtonPressed 
				                                && GetForegroundWindow () == txWindow ())
			if (button -> onMouseClick ()) return true;
	}

	return false;
}

void ButtonManager::drawButtons (Context * context)
{

	size_t count = buttons_.size ();
	for (size_t i = 0; i < count; i++)
		buttons_[i] -> onDraw (context);
}

void ButtonManager::onTimeTick ()
{
	size_t count = buttons_.size ();
	for (size_t i = 0; i < count; i++)
		buttons_[i] -> onTimeTick ();
}

//------------------------------