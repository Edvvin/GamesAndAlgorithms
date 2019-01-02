#pragma once
#include "Node.h"

class Movie;

class ENode :public Node
{
	friend class INode;
	friend class BPlus;
	int movies[10];
	ENode* next = nullptr, *prev = nullptr;
	bool add(int key, const BPlus& bp, Paketic& p);
public:
	bool remove(int i) override;
	bool isINode() const override { return false; }
	bool isENode() const override { return true; }
	~ENode() override;
};

