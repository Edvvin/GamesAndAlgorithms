#include "Movie.h"

void Movie::set(string name, int year, int month, int day, string lang, int runtime, double revenue)
{
	this->name = name;
	this->year = year;
	this->month = month;
	this->day = day;
	this->lang = lang;
	this->runtime = runtime;
	this->revenue = revenue;
}

void Movie::swap(Movie & m1, Movie & m2)
{
	string temp;
	temp = m2.name;
	m2.name = m1.name;
	m1.name = temp;
	int tempp = m2.year;
	m2.year = m1.year;
	m1.year = tempp;
	tempp = m2.month;
	m2.month = m1.month;
	m1.month = tempp;
	tempp = m2.day;
	m2.day = m1.day;
	m1.day = tempp;
	tempp = m2.runtime;
	m2.runtime = m1.runtime;
	m1.runtime = tempp;
	double temppp = m2.revenue;
	m2.revenue = m1.revenue;
	m1.revenue = temppp;
	temp = m2.lang;
	m2.lang = m1.lang;
	m1.lang = temp;
}

bool Movie::operator>=(const Movie & m) const
{
	if (this->getYear() > m.getYear()) {
		return true;
	}
	else if (this->getYear() == m.getYear()) {
		if (this->getMonth() > m.getMonth()) {
			return true;
		}
		else if (this->getMonth() == m.getMonth()) {
			if (this->getDay() >= m.getDay()) {
				return true;
			}
		}
	}
	return false;
}

bool Movie::operator<=(const Movie & m) const
{
	return m >= *this;
}

bool Movie::operator<(const Movie & m) const
{
	return !(*this>=m);
}

bool Movie::operator>(const Movie & m) const
{
	return !(*this<=m);
}

ostream & operator<<(ostream & o, const Movie & m)
{
	o << m.lang << ", " << '"' << m.name << "\",";
	o << "(" << m.year << "-" << m.month << "-" << m.day << "), ";
	o << m.runtime;
	o << m.revenue;
	o << "  " << (m.valid ? " + " : " - ");
	return o;
}
