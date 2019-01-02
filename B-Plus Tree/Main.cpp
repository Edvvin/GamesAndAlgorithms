#include <iostream>
#include "DataBase.h"
#include "BPlus.h"
#include "Node.h"
#include "ENode.h"

using namespace std;


int main() {
	DataBase* db = new DataBase("hi.csv", 200);
	BPlus* B = nullptr;
	int op,x,y;
	while (1) {
		cout << "Izaberite opciju:" << endl;
		cout << "1) Ispis Baze" << endl;
		cout << "2) Brisanje iz baze" << endl;
		cout << "3) Stvori B+ stablo" << endl;
		cout << "4) Unisti B+ stablo" << endl;
		cout << "5) Nadji kljuc" << endl;
		cout << "6) Dodaj kljuc" << endl;
		cout << "7) Obrisi kljuc" << endl;
		cout << "8) Filmovi iz perioda" << endl;
		cout << "9) Broj filmova u godini" << endl;
		cout << "10) Ispisi B+" << endl;
		cout << "0) Exit" << endl;
		cin >> op;
		switch (op) {
		case 1:
			cout << (*db) << endl;
			break;
		case 2:
			cout << "Unesite indeks kljuca koji brisete: ";
			cin >> x;
			db->remove(x);
			break;
		case 3:
			if (B != nullptr) {
				delete B;
			}
			cout << "Unesite red stabla: ";
			cin >> x;
			B = new BPlus(db,x);
			break;
		case 4:
			delete B;
			B = nullptr;
			break;
		case 5:
			if (B == nullptr) {
				cout << "Stablo jos nije kreirano!"<<endl;
			}
			else {
				cout << "Unesite kljuc: ";
				cin >> x;
				cout << endl;
				if (B->find(x)) {
					cout << "Pronadjen" << endl;
				}
				else {
					cout << "Nije pronadjen" << endl;
				}
			}
			break;
		case 6:
			if (B == nullptr) {
				cout << "Stablo jos nije kreirano!" << endl;
			}
			else {
				cout << "Unesite kljuc: ";
				cin >> x;
				cout << endl;
				if (B->add(x)) {
					cout << "Kljuc je uspesno dodat" << endl;
				}
				else {
					cout << "Kljuc nije uspesno dodat" << endl;
				}
			}
			break;
		case 7:
			if (B == nullptr) {
				cout << "Stablo jos nije kreirano!" << endl;
			}
			else {
				cout << "Unesite kljuc: ";
				cin >> x;
				cout << endl;
				if (B->find(x)) {
					B->unvalKey(x);
				}
				if (B->remove(x)) {
					cout << "Kljuc je uspesno obrisan" << endl;
				}
				else {
					cout << "Kljuc nije uspesno obrisan" << endl;
				}
			}
			break;
		case 8:
			if (B == nullptr) {
				cout << "Stablo jos nije kreirano!" << endl;
			}
			else {
				cout << "Unesite kljuc pocetka perioda: ";
				cin >> x;
				cout << "Unesite kljuc kraja perioda: ";
				cin >> y;
				cout << endl;
				B->allFromPeriod(x, y);
			}
			break;
		case 9:
			if (B == nullptr) {
				cout << "Stablo jos nije kreirano!" << endl;
			}
			else {
				cout << "Unesite godinu: ";
				cin >> x;
				cout << endl;
				int xt = B->numInYear(x);
				if (xt < 0) {
					cout << "Nevalidna godina"<< endl;
				}
				else {
					cout << xt << endl;
				}
			}
			break;
		case 10:
			if (B == nullptr) {
				cout << "Stablo jos nije kreirano!" << endl;
			}
			else {
				B->levelOrder();
				cout << endl;
			}
			break;
		case 420:
			if (B == nullptr)
				delete B;
			B = new BPlus(db, 5);
			B->add(200411);
			B->add(201605);
			B->add(201510);
			B->add(200406);
			B->add(200504);
			B->add(201403);
			B->add(200512);
			B->add(200706);
			B->add(200708);
			B->add(200811);
			B->add(200905);
			B->remove(201403);
			B->remove(200708);
			B->remove(200706);
			B->remove(200512);
			B->remove(201510);
			B->remove(200504);
			B->levelOrder();
			B->remove(200905);
			break;
		case 0:
			delete B;
			delete db;
			return 0;
		default:
			cout << endl << "Nevalidna opcija!" << endl;
		}
	}
	int i;
	cin >> i;
	return 0;
}