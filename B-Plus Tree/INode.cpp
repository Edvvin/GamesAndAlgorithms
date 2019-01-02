#include "INode.h"
#include "BPlus.h"
#include "ENode.h"
class BPlus;


int INode::remove(int key, int order)
{
	int i;
	for (i = 0; i < size; i++)
	{
		if (keys[i] == key)
			break;
	}
	if (parent == nullptr) {
		//if (size == 1) {
		//	Node* left = sons[0];
		//	Node* right = sons[1];
		//	for (int k = left->size, j = 0; j < right->size; j++, k++)
		//	{
		//		left->keys[k] = right->keys[j];
		//		if (left->isENode()) {
		//			((ENode*)left)->movies[k] = ((ENode*)right)->movies[j];
		//		}
		//		else {
		//			((INode*)left)->sons[k+1] = ((INode*)right)->sons[j];
		//		}
		//		left->size++;
		//	}
		//}
		for (int j = i; j < size - 1; j++)
		{
			sons[j+1] = sons[j + 2];
			keys[j] = keys[j + 1];
		}
		size--;
		return -1;
	}
	if (size < order / 2) {
		INode* bro = (INode*)(rightBrother());
		if (bro != nullptr && bro->size >= order / 2) {
			int j;
			for (j = 0; j < parent->size; j++)
			{
				if (keys[0] < parent->keys[j] && parent->keys[j] < bro->keys[0])
					break;
			}
			for (int k = i; k < size - 1; k++)
			{
				sons[k + 1] = sons[k + 2];
				keys[k] = keys[k + 1];
			}
			size--;
			keys[size++] = parent->keys[j];
			sons[size] = bro->sons[0];
			bro->sons[0]->parent = this;
			parent->keys[j] = bro->keys[0];
			for (int k = 0; k < bro->size-1; k++)
			{
				bro->keys[k] = bro->keys[k + 1];
				bro->sons[k] = bro->sons[k + 1];
			}
			bro->size--;
			return -1;
		}
		bro = (INode*)(leftBrother());
		if (bro != nullptr && bro->size >= order / 2) {
			int j;
			for (j = 0; j < parent->size; j++)
			{
				if (keys[0] > parent->keys[j] && parent->keys[j] > bro->keys[0])
					break;
			}
			for (int k = i; k > 0; k--)
			{
				sons[k+1] = sons[k];
				keys[k] = keys[k - 1];
			}
			sons[1] = sons[0];
			keys[0] = parent->keys[j];
			sons[0] = bro->sons[bro->size-1];
			bro->sons[bro->size - 1]->parent = this;
			parent->keys[j] = bro->keys[0];
			for (int k = 1; k < bro->size; k++)
			{
				bro->keys[k-1] = bro->keys[k];
				bro->sons[k-1] = bro->sons[k];
			}
			bro->size--;
			return -1;
		}
		//Ubi boze
	}
	else {
		for (int j = i; j < size - 1; j++)
		{
			sons[j + 1] = sons[j + 2];
			keys[j] = keys[j + 1];
		}
		size--;
		return -1;
	}
	return -1;
}

bool INode::remove(int i)
{
	if (i > size || i<0 || size == 0) {
		return false;
	}
	for (int j = i; j < size; j++)
	{
		keys[j] = keys[j + 1];
		sons[j + 1] = sons[j + 2];
	}
	size--;
	return true;
}


bool INode::add( const BPlus& bp, Paketic& p)
{
	int i = 0, key = p.key;
	Node* son = p.right;
	if (size == 0)
		sons[0] = p.left;
	while (keys[i] <= p.key && i < size) {
		i++;
	}
	while (i <= size) {
		int temp;
		temp = key;
		key = keys[i];
		this->keys[i] = temp;
		Node* tempp = son;
		son = sons[i+1];
		sons[i+1] = tempp;
		i++;
	}
	size++;
	if (size == bp.getOrder()) {
		p.key = keys[size / 2];
		p.left = this;
		INode* n = new INode();
		int j = 0;
		for (int i = size / 2 + 1; i < size; j++, i++)
		{
			n->keys[j] = keys[i];
			n->sons[j] = sons[i];
			n->size++;
		}
		n->sons[j] = sons[size];
		n->parent = parent;
		if (parent == nullptr)bp.markNullParent(n);
		size = size / 2;
		p.right = n;
		for (int i = 0; i <= n->size; i++)
		{
			n->sons[i]->parent = n;
		}
	}
	else {
		p.key = -1;
	}
	return true;
}

INode::~INode()
{
}
