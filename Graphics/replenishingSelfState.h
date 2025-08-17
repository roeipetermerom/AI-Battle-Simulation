#pragma once
#include "State.h"
class replenishingSelfState :
    public State
{
public:
	void OnEnter(NPC* p);
	void Transition(NPC* p);
	void OnExit(NPC* p);
};

