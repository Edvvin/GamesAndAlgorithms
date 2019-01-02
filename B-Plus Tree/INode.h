#pragma once
#include "Node.h"

struct Paketic;

class INode :public Node
{
	friend class BPlus;
	Node* sons[11];
protected:
	using Node::parent;
public:
	int remove(int key,int order);
	bool isINode() const override { return true; }
	bool isENode() const override { return false; }
	bool remove(int i) override;
	bool add(const BPlus& bp,Paketic& p);
	Node* getSon(int index) { return sons[index]; }
	~INode() override;
};

