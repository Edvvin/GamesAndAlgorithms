#include "Node.h"
#include "INode.h"
#include <iostream>

using namespace std;

bool Node::removeKey(int key)
{
	for (int i = 0; i < size; i++)
	{
		if (keys[i] == key)
			return remove(i);
	}
	return false;
}

int Node::keyIndex(int key)
{
	for (int i = 0; i <= size; i++)
	{
		if (keys[i] == key)
			return i;
	}
	return -1;
}

Node * Node::leftBrother()
{
	Node* temp = this->parent;
	if (temp == nullptr)
		return nullptr;
	if(temp->keys[0] >= keys[size-1])
		return nullptr;
	for (int i = 0; i < temp->size; i++)
	{
		if (temp->keys[i] >= keys[size - 1])
			return ((INode*)temp)->getSon(i-1);
	}
	return ((INode*)temp)->getSon(temp->size-1);
}

Node * Node::rightBrother()
{
	Node* temp = this->parent;
	if (temp == nullptr)
		return nullptr;
	if (temp->keys[temp->size -1 ] < keys[0])
		return nullptr;
	for (int i = temp->size-1; i >= 0; i--)
	{
		if (temp->keys[i] < keys[0])
			return ((INode*)temp)->getSon(i+2);
	}
	return ((INode*)temp)->getSon(1);
}

Node::~Node()
{
}

bool Node::isInNode(int key) const
{
	for (int i = 0; i < this->size; i++)
	{
		if (this->keys[i] == key)
			return true;
	}
	return false;
}


ostream& operator<<(ostream& o, const Node& n) {
	for (int i = 0; i < n.size; i++)
	{
		cout << n.keys[i] << " ";
	}
	return o;
}