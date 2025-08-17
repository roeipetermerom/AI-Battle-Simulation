#pragma once
#include "State.h"
class ReplenishHealthState :
    public State
{
public:
	void OnEnter(NPC* p);
	void Transition(NPC* p);
	void OnExit(NPC* p);
};

