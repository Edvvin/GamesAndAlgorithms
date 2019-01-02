#pragma once
#include <cmath>
using namespace std;

class Node;
class INode;
class ENode;
class DataBase;

class BPlus
{
	DataBase* db;
	Node* root;
	int size = 0,order;
	Node* mark = nullptr;
	int i_mark = 0;
	INode* marx = nullptr;

	int minsizeE() const { return order/2 - 1 ; }
	int minsizeI() const { return (int)ceil(order / 2.0) - 1; }


	int separator(INode* par, Node* l, Node* r);
	void collapseE(ENode* l, ENode* r,int key);
	void collapseI(INode * l, INode * r);
	void update(Node* t,int key, int rep);

	void lendRightE(ENode* l, ENode* r);
	void lendLeftE(ENode* l, ENode* r);
	void lendRightI(INode* l, INode* r);
	void lendLeftI(INode* l, INode* r);


	ENode * findLeaf(int key);
	const ENode * findLeaf(int key) const;
	bool rangeFromPeriod(int time1, int time2, int& low, int& high) const;
public:
	void unvalKey(int key);
	BPlus(DataBase* db,int order);
	int getOrder() const { return order; }
	void levelOrder() const;
	bool add(int key);
	bool remove(int key);
	bool find(int key) const;
	const DataBase* getDataBase() const { return db; }
	void allFromPeriod(int ,int) const;
	int numInYear(int year) const;
	void markNullParent(Node* mark) const { (const_cast<BPlus*> (this))->mark = mark; }
	~BPlus();
};

