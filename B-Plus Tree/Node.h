#pragma once
#include <iostream>
using namespace std;


class Node
{
protected:
	friend class BPlus;
	friend class INode;
	int keys[10],size = 0;
	Node* parent = nullptr;
	bool isInNode(int key) const;
public:
	virtual bool isINode() const = 0;
	virtual bool isENode() const = 0;
	virtual bool remove(int i) = 0;
	bool removeKey(int key);
	int keyIndex(int key);
	Node* leftBrother();
	Node* rightBrother();
	virtual ~Node() = 0;
	friend ostream& operator<<(ostream& o, const Node& n);
};

struct Paketic {
	int key;
	Node *left, *right;
};