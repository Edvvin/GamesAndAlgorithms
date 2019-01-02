#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include "Movie.h"

class Movie;

using namespace std;

class DataBase
{
	ifstream f;
	Movie* movies;
	int size = 0;
	void add(Movie m);
public:
	int getSize() const { return size; }
	DataBase(string file,int maxsize);
	DataBase(const DataBase&) = delete;
	DataBase(DataBase&&) = delete;
	int findMovie(int key) const;
	DataBase& operator=(const DataBase&) = delete;
	void remove(int index) { movies[index].setValid(false); }
	friend ostream& operator<<(ostream& o, const DataBase& db);
	Movie& operator[](int index) { return movies[index]; }
	~DataBase();
};

