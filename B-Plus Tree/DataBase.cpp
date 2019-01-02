#include "DataBase.h"

void DataBase::add(Movie m)
{
	int i = 0;
	while (movies[i] <= m && i<size) {
		i++;
	}
	while (i <= size) {
		swap(movies[i++], m);
	}
	size++;
}

DataBase::DataBase(string file, int maxsize):size(0) {
	movies = new Movie[maxsize];
	if (movies == nullptr) {
		cout << "Bad alloc";
		exit(1);
	}
	f.open(file);
	if (!f.is_open()) {
		cout << "File not loaded";
		exit(1);
	}
	try {
		string temp;
		getline(f, temp);
		string name = "";
		Movie M;
		char t;
		string lng;
		int year, month, day, runtime;
		double rev;
		while (!f.eof()&& size<maxsize) {
			name = "";
			lng = "";
			f >> t;
			lng += t;
			f >> t;
			lng += t;
			f >> t;
			f >> t;
			if (t == '"') {
				while (1) {
					f >> t;
					if (t == '"')
						break;
					name += t;
				};
				f >> t;
			}
			else {
				name += t;
				while (1) {
					f >> t;
					if (t == ',')
						break;
					name += t;
				}
			}
			f >> year;
			f >> t;
			f >> month;
			f >> t;
			f >> day;
			f >> t;
			f >> rev;
			f >> t;
			f >> runtime;
			getline(f, temp);
			M.set(name, year, month, day, lng, runtime, rev);
			M.setValid(true);
			add(M);
		}
	}
	catch (...) {
		cout << "Bad data";
		exit(1);
	}
}

int DataBase::findMovie(int key) const
{
	if (key < 100001)
		return -1;
	int mm = key % 100;
	int yy = key / 100;
	Movie m;
	m.set("", yy, mm, 0, "", 0, 0.0);
	int high = size - 1, low = 0;
	int mid,min;
	while (high - low > 1) {
		mid = (high + low) / 2;
		if (m >= movies[mid]) {
			low = mid;
		}
		else {
			high = mid;
		}
	}
	if (movies[low].getYear() == yy && movies[low].getMonth() == mm)
		min = low;
	else if (movies[high].getYear() == yy && movies[high].getMonth() == mm)
		min = high;
	else
		return -1;
	while (min < size && movies[min].getYear() == yy && movies[min].getMonth() == mm) {
		if (movies[min].isValid()) {
			return min;
		}
		min++;
	}
	return -1;
}

DataBase::~DataBase()
{
	delete [] movies;
}

ostream & operator<<(ostream & o, const DataBase & db)
{
	for (int i = 0; i < db.size; i++)
	{
		o << i << " " << db.movies[i] << endl;
	}
	return o;
}
