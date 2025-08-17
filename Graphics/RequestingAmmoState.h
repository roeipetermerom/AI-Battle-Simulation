#pragma once
#include "State.h"
class RequestingAmmoState :
    public State
{
public:
	void OnEnter(NPC* p) override;
	void Transition(NPC* p) override;
	void OnExit(NPC* p) override;
};

