#include "BPlus.h"
#include "ENode.h"
#include "INode.h"
#include "DataBase.h"
#include "Movie.h"
#include <Queue>
#include <iostream>




void BPlus::unvalKey(int key)
{
	ENode* e = findLeaf(key);
	int i = e->keyIndex(key);
	int temp = e->movies[i];
	int c;
	do {
		(*db)[temp].setValid(false);
		temp++;
		c = (*db)[temp].getYear() * 100 + (*db)[temp].getMonth();
	} while (c == key);
}

int BPlus::separator(INode* par,Node * l, Node * r)
{
	for (int i = 0; i <= par->size; i++)
	{
		if (par->sons[i] == (Node*)(l)) {
			return i;
		}
	}
	return -1;
}

void BPlus::collapseE(ENode * l, ENode * r, int key)
{

	for (int k = l->size, j = 0; j < r->size; j++, k++)
	{
		l->keys[k] = r->keys[j];
		l->movies[k] = r->movies[j];
		l->size++;
	}
	l->removeKey(key);
	update(l, l->keys[l->size - 1], l->keys[l->size - 1]);
	update(l, key, l->keys[l->size - 1]);
	l->next = r->next;
	if (r->next != nullptr) {
		r->next->prev = l;
	}
	INode* par = (INode*)(l->parent);
	int i = separator(par, l, r);
	par->remove(i);
	marx = par;
	delete r;
}

void BPlus::collapseI(INode * l, INode * r)
{
	INode* par = (INode*)(l->parent);
	int i = separator(par, l, r);
	int parkey = par->keys[i];
	l->keys[l->size++] = parkey;
	l->sons[l->size] = r->sons[0];
	for (int k = l->size, j = 0; j < r->size; j++, k++)
	{
		l->keys[k] = r->keys[j];
		l->sons[k+1] = r->sons[j+1];
		l->size++;
	}
	for (int k = 0; k <= r->size; k++)
	{
		r->sons[k]->parent = l;
	}
	par->remove(i);
	marx = par;
	delete r;
}


void BPlus::update(Node* t,int key, int rep)
{
	Node* temp = t->parent;
	while (temp != nullptr) {
		int i = temp->keyIndex(key);
		if (i >= 0) {
			temp->keys[i] = rep;
		}
		temp = temp->parent;
	}
}

void BPlus::lendRightE(ENode * l, ENode * r)
{
	int key = r->keys[0];
	int movie = r->movies[0];
	r->remove(0);
	l->movies[l->size] = movie;
	l->keys[l->size++] = key;
}

void BPlus::lendLeftE(ENode * l, ENode * r)
{
	//shifts r right
	for (int i = r->size; i > 0; i--)
	{
		r->keys[i] = r->keys[i - 1];
		r->movies[i] = r->movies[i - 1];
	}

	r->keys[0] = l->keys[l->size-1];
	r->movies[0] = l->movies[l->size - 1];
	r->size++;
	l->remove(l->size - 1);
}

void BPlus::lendRightI(INode * l, INode * r)
{
	INode* par = (INode*)(l->parent);
	int i = separator(par, l, r);
	l->keys[l->size - 1] = par->keys[i];
	l->sons[l->size++] = r->sons[0];
	r->sons[0]->parent = l;
	par->keys[i] = r->keys[0];
	for (int j = 0; j < r->size; j++)
	{
		r->keys[j] = r->keys[j + 1];
		r->sons[j] = r->sons[j + 1];
	}
	r->size--;
}

void BPlus::lendLeftI(INode * l, INode * r)
{
	for (int j = r->size; j > 0; j--)
	{
		r->keys[j] = r->keys[j - 1];
		r->sons[j] = r->sons[j - 1];
	}
	INode* par = (INode*)(l->parent);
	int i = separator(par, l, r);
	r->keys[0] = par->keys[i];
	r->size++;
	r->sons[0] = l->sons[l->size];
	r->sons[0]->parent = r;
	par->keys[i] = l->keys[l->size - 1];
	l->size--;
}

ENode * BPlus::findLeaf(int key)
{
	if (root == nullptr)
		return nullptr;
	Node* temp = root;
	while (temp->isINode()) {
		if (temp == nullptr) {
			cout << "There exists a nullptr in an internal node" << endl;
			exit(1);
		}
		if (key <= temp->keys[0]) {
			temp = ((INode*)temp)->sons[0];
		}
		else {
			int i;
			int last = temp->size;
			for (i = 1; i < last; i++)
			{
				if (temp->keys[i-1] < key && key <= temp->keys[i]) {
					temp = ((INode*)temp)->sons[i];
					break;
				}
			}
			if(i == last)
				temp = ((INode*)temp)->sons[last];
		}
	}
	return (ENode*)temp;
}

const ENode * BPlus::findLeaf(int key) const
{
	return (const_cast<BPlus*>(this))->findLeaf(key);
}

bool BPlus::rangeFromPeriod(int time1, int time2, int & low, int & high) const
{
	if (time1 < 100000 || time1>999912) {
		return false;
	}
	if (time2 < 100000 || time2>999912) {
		return false;
	}
	if (time2 < time1) {
		return false;
	}
	const ENode* lowLeaf = findLeaf(time1);
	Movie m1,m2;
	bool help = false;
	m1.set("", time1 / 100, time1 % 100, 1, "", 0, .0);
	m2.set("", time2 / 100, time2 % 100, 31, "", 0, .0);
	if (lowLeaf == nullptr) {
		low = 0;
	}
	else {
		int i;
		for (i = lowLeaf->size - 1; i >= 0; --i)
		{
			if (lowLeaf->keys[i] <= time1) {
				break;
			}
		}
		if (i < 0) {
			if (lowLeaf->prev == nullptr) {
				low = 0;
				help = true;
			}
			else {
				lowLeaf = lowLeaf->prev;
				low = lowLeaf->size - 1;
			}
		}
		if(!help)
			low = lowLeaf->movies[low];
	}
	while ((*db)[low] <= m1) {
		low++;
	}
	while (!((*db)[low].isValid())) {
		low++;
	}
	const ENode* highLeaf = findLeaf(time2);
	if (highLeaf == nullptr) {
		high = db->getSize()-1;
	}
	else {
		int i;
		for (i = 0; i < highLeaf->size; i++)
		{
			if (highLeaf->keys[i] > time2) {
				break;
			}
		}
		if (i == highLeaf->size) {
			if (highLeaf->next == nullptr) {
				high = db->getSize() - 1;
				while ((*db)[high] >= m2) {
					high--;
				}
				while (!((*db)[high].isValid())) {
					high--;
				}
				return true;
			}
			else {
				highLeaf = highLeaf->next;
				high = 0;
			}
		}
		high = highLeaf->movies[high];
	}
	while ((*db)[high] >= m2) {
		high--;
	}
	while (!((*db)[high].isValid())) {
		high--;
	}
	return true;
}

BPlus::BPlus(DataBase* db, int order):db(db)
{
	if (order < 3 || order > 10) {
		this->order = 3;
	}
	else {
		this->order = order;
	}
	root = new ENode();
}

void BPlus::levelOrder() const
{
	Node* temp;
	queue<Node*> Q;
	Q.push(root);
	Q.push(nullptr);
	while (Q.size()>1) {
		temp = Q.front();
		Q.pop();
		if (temp == nullptr) {
			Q.push(nullptr);
			cout << endl;
		}
		else {
			for (int i = 0; i < temp->size; i++)
			{
				cout << temp->keys[i] << " ";
			}
			cout << "    ";
			if (temp->isINode()) {
				INode* x = (INode*)temp;
				for (int i = 0; i <= temp->size; i++)
				{
					Q.push(x->sons[i]);
				}
			}
		}
	}
}

bool BPlus::add(int key)
{
	if (root == nullptr)
		root = new ENode();
	ENode* e = findLeaf(key);
	if (e->isInNode(key))
		return false;
	Paketic p;
	bool t = e->add(key, *this,p);
	if (!t)
		return false;
	INode* temp = (INode*)(e->parent);
	while (p.key >= 0) {
		if (temp == nullptr) {
			INode* x = new INode();
			root->parent = x;
			root = root->parent;
			x->add(*this,p);
			if (mark != nullptr) {
				mark->parent = x;
				mark = nullptr;
			}
			return true;
		}
		temp->add(*this,p);
		temp = (INode*)temp->parent;
	}
	return true;
}

bool BPlus::remove(int key)
{
	ENode* e = findLeaf(key);
	if (e == nullptr)
		return false;
	int i;
	for (i = 0; i < e->size; i++)
	{
		if (e->keys[i] == key)
			break;
	}
	if (i == e->size)
		return false;
	if (e->parent == nullptr) { //e is root
		for (int j = i; j < e->size - 1; j++)
		{
			e->movies[j] = e->movies[j + 1];
			e->keys[j] = e->keys[j + 1];
		}
		e->size--;
		return true;
	}
	if (e->size <= minsizeE()) {
		ENode* bro = (ENode*)(e->rightBrother());
		if (bro != nullptr && bro->size > minsizeE()) {
			update(e, e->keys[e->size - 1], bro->keys[0]);
			e->removeKey(key);
			lendRightE(e, bro);
			return true;
		}
		bro = (ENode*)(e->leftBrother());
		if (bro != nullptr && bro->size > minsizeE()) {
			update(e, bro->keys[bro->size - 1], bro->keys[bro->size - 2]);
			if (e->keys[e->size - 1] == key) {
				if (e->size == 1) {
					update(e, key, bro->keys[bro->size - 1]);
				}
				else {
					update(e, key, e->keys[e->size - 2]);
				}
			}
			e->removeKey(key);
			lendLeftE(bro, e);
			return true;
		}
		bool colpsd = false;
		bro = (ENode*)(e->rightBrother());
		if (bro != nullptr) {
			collapseE(e, bro, key);
			colpsd = true;
		}

		bro = (ENode*)(e->leftBrother());
		if (bro != nullptr && !colpsd) {
			collapseE(bro, e, key);
		}

		while (marx!=nullptr) {
			//root case
			if (marx->parent == nullptr) {
				if (marx->size == 0) {
					root = marx->sons[0];
					root->parent = nullptr;
					delete marx;
				}
				marx = nullptr;
				return true;
			}
			if (marx->size <= minsizeI()) {
				INode* bro = (INode*)(marx->rightBrother());
				if (bro != nullptr && bro->size > minsizeI()) {
					lendRightI(marx, bro);
					marx = nullptr;
					return true;
				}
				bro = (INode*)(marx->leftBrother());
				if (bro != nullptr && bro->size > minsizeI()) {
					lendLeftI(bro, marx);
					marx = nullptr;
					return true;
				}
				bro = (INode*)(marx->rightBrother());
				if (bro != nullptr) {
					collapseI(marx, bro);
				}
				else {
					bro = (INode*)(marx->leftBrother());
					collapseI(bro, marx);
				}
			}
			else {
				marx = nullptr;
			}
		}
	}
	else {
		if (e->keys[e->size - 1] == key) {
			update(e, key, e->keys[e->size - 2]);
		}
		e->removeKey(key);
	}
	return true;
}

bool BPlus::find(int key) const
{
	if (root == nullptr)
		return false;
	const ENode* e = findLeaf(key);
	return e->isInNode(key);
}

void BPlus::allFromPeriod(int time1, int time2) const
{
	int low = 0, high = 0;
	if (!rangeFromPeriod(time1, time2, low, high)) { cout << "Invalid Period" << endl; return; }
	for (int i = low; i <= high; i++)
	{
		if(((*db)[i]).isValid())
			cout << (*db)[i] << endl;
	}
}

int BPlus::numInYear(int year) const
{
	if (year < 1000 || year>9999)
		return -1;

	int low = 0, high = 0;
	int time1 = year * 100 + 1, time2 = year*100 + 12;
	if (!rangeFromPeriod(time1, time2, low, high)) { return -1; }
	int j = 0;
	for (int i = low; i <= high; i++)
	{
		if (((*db)[i]).isValid())
			j++;
	}
	return j;
}



BPlus::~BPlus()
{
	Node* temp;
	queue<Node*> Q;
	Q.push(root);
	while (!Q.empty()) {
		temp = Q.front();
		Q.pop();
		if (temp->isINode()) {
			INode* x = (INode*)temp;
			for (int i = 0; i <= temp->size; i++)
			{
				Q.push(x->sons[i]);
			}
		}
		delete temp;
	}
}
