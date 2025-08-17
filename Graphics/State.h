#pragma once
#include "NPC.h"

class NPC;
// pure virtual class (interface)

class State
{
public:
	virtual void OnEnter(NPC* p) = 0;
	virtual void Transition(NPC* p) = 0;
	virtual void OnExit(NPC* p) = 0;
};

