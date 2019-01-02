#pragma once
#include <string>
#include <iostream>
using namespace std;

class Movie
{
	string name = "";
	int year = 0, month = 0, day = 0, runtime = 0;
	string lang = "";
	double revenue = 0.0;
	bool valid = true;
public:
	void set(string name,int year,int month,int day,string lang, int runtime, double revenue);
	void setValid(bool v) { valid = v; }
	static void swap(Movie& m1, Movie& m2);

	string getName() const { return name; }
	int getYear() const { return year; }
	int getMonth() const { return month; }
	int getDay() const { return day; }
	int getRuntime() const { return runtime; }
	double getRevenue() const { return revenue; }
	bool isValid() const { return valid; }

	bool operator>=(const Movie& m) const;
	bool operator<=(const Movie& m) const;
	bool operator<(const Movie& m) const;
	bool operator>(const Movie& m) const;
	friend ostream& operator<<(ostream& o, const Movie& m);
};

