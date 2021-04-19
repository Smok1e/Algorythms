#pragma once

//------------------------

class controllable
{

public :

	controllable ();
	controllable (int value);
	controllable (const controllable& that);

	controllable& operator = (const controllable& that);

	bool operator < (const controllable& that);
	bool operator > (const controllable& that);

	operator int ();

	static int getCompareCount    ();
	static int getAssignmentCount ();

	static void setCompareCount    (int count);
	static void setAssignmentCount (int count);

	static void resetCompareCount    ();
	static void resetAssignmentCount ();

private :

	int value_;

	static int compare_count_;
	static int assignment_count_;

};

//------------------------

int controllable::compare_count_    = 0;
int controllable::assignment_count_ = 0;

//------------------------

controllable::controllable () :
	value_ (0)
{}

controllable::controllable (int value) :
	value_ (value)
{}

controllable::controllable (const controllable& that) :
	value_ (that.value_)
{ assignment_count_ ++; }

//------------------------

controllable& controllable::operator = (const controllable& that)
{
	assignment_count_ ++;
	if (&that == this) return *this;
	value_ = that.value_;
	return *this;
}

//------------------------

bool controllable::operator < (const controllable& that)
{
	compare_count_++;
	return value_ < that.value_;
}

bool controllable::operator > (const controllable& that)
{
	compare_count_++;
	return value_ > that.value_;
}

//------------------------

controllable::operator int ()
{
	return value_;
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

//------------------------

void controllable::setCompareCount (int count)
{
	compare_count_ = count;
}

void controllable::setAssignmentCount (int count)
{
	assignment_count_ = count;
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

//------------------------