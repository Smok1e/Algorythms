#pragma once

//------------------------

class controllable
{
public :
	controllable ();
	controllable (int value);
	controllable (int value, int position);
	controllable (const controllable& that);

	controllable& operator = (const controllable& that);

	bool operator <  (const controllable& that);
	bool operator <= (const controllable& that);

	bool operator >  (const controllable& that);
	bool operator >= (const controllable& that);

	void* operator new   (size_t size);
	void* operator new[] (size_t size);

	operator int ();

	void setPosition (int position);
	int  getPosition ();

	static int getCompareCount     ();
	static int getAssignmentCount  ();
	static int getAllocationCount ();

	static void setCompareCount     (int count);
	static void setAssignmentCount  (int count);
	static void setAllocationCount (int count);

	static void resetCompareCount     ();
	static void resetAssignmentCount  ();
	static void resetAllocationCount ();

private :
	int value_;
	int position_;

	static int compare_count_;
	static int assignment_count_;
	static int allocation_count_;
};

//------------------------

int controllable::compare_count_    = 0;
int controllable::assignment_count_ = 0;
int controllable::allocation_count_ = 0;

//------------------------

controllable::controllable () :
	value_    (0),
	position_ (0)
{}

controllable::controllable (int value) :
	value_    (value),
	position_ (0)
{}

controllable::controllable (int value, int position) :
	value_    (value),
	position_ (position)
{}

controllable::controllable (const controllable& that) :
	value_    (that.value_),
	position_ (that.position_)
{ assignment_count_ ++; }

//------------------------

controllable& controllable::operator = (const controllable& that)
{
	assignment_count_ ++;

	if (&that == this) return *this;

	value_    = that.value_;
	position_ = that.position_;

	return *this;
}

//------------------------

bool controllable::operator < (const controllable& that)
{
	compare_count_++;
	return value_ < that.value_;
}

bool controllable::operator <= (const controllable& that)
{
	compare_count_ ++;
	return value_ <= that.value_;
}

//------------------------

bool controllable::operator > (const controllable& that)
{
	compare_count_ ++;
	return value_ > that.value_;
}

bool controllable::operator >= (const controllable& that)
{
	compare_count_ ++;
	return value_ >= that.value_;
}

//------------------------

void* controllable::operator new (size_t size)
{
	allocation_count_ ++;
	return ::new controllable;
}

void* controllable::operator new[] (size_t size)
{
	size /= sizeof (controllable);

	allocation_count_ += size;
	return ::new controllable[size];
}

//------------------------

controllable::operator int ()
{
	return value_;
}

//------------------------

void controllable::setPosition (int position)
{
	position_ = position;
}

int controllable::getPosition ()
{
	return position_;
}

//------------------------

int controllable::getCompareCount ()
{
	return compare_count_;
}

int controllable::getAssignmentCount ()
{
	return assignment_count_;
}

int controllable::getAllocationCount ()
{
	return allocation_count_;
}

//------------------------

void controllable::setCompareCount (int count)
{
	compare_count_ = count;
}

void controllable::setAssignmentCount (int count)
{
	assignment_count_ = count;
}

void controllable::setAllocationCount (int count)
{
	allocation_count_ = count;
}

//------------------------

void controllable::resetCompareCount ()
{
	setCompareCount (0);
}

void controllable::resetAssignmentCount ()
{
	setAssignmentCount (0);
}

void controllable::resetAllocationCount ()
{
	setAllocationCount (0);
}

//------------------------