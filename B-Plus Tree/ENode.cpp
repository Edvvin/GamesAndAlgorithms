#include "ENode.h"
#include "Movie.h"
#include "BPlus.h"
#include "DataBase.h"

bool ENode::add(int key, const BPlus& bp, Paketic& p)
{
	int x = (bp.getDataBase())->findMovie(key);
	if (x < 0)
		return false;
	int i = 0;
	while (this->keys[i] <= key && i < size) {
		i++;
	}
	while (i <= size) {
		int temp;
		temp = key;
		key = keys[i];
		this->keys[i] = temp;
		temp = x;
		x = movies[i];
		movies[i] = temp;
		i++;
	}
	size++;
	if (size == bp.getOrder()) {
		p.key = keys[(int)ceil(size / 2.0)-1];
		p.left = this;
		ENode* n = new ENode();
		n->next = this->next;
		this->next = n;
		n->prev = this;
		for (int i = (int)ceil(size/2.0), j = 0; i < size;j++, i++)
		{
			n->keys[j] = keys[i];
			n->movies[j] = movies[i];
			n->size++;
		}
		n->parent = parent;
		if (parent == nullptr)
			bp.markNullParent(n);
		size = ceil(size / 2.0);
		p.right = n;
	}
	else {
		p.key = -1;
	}
	return true;
}

bool ENode::remove(int i)
{
	if (i > size || i < 0 || size == 0) {
		return false;
	}
	for (int j = i; j < size; j++)
	{
		keys[j] = keys[j + 1];
		movies[j] = movies[j+1];
	}
	size--;
	return true;
}

ENode::~ENode()
{
}
