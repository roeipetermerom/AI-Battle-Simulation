#include "RequestingAmmoState.h"
#include "ThinkingState.h"

void RequestingAmmoState::OnEnter(NPC* p)
{
	p->setRequestingAmmo(true);
}

void RequestingAmmoState::Transition(NPC* p)
{
	OnExit(p);
	if (p->getAmmo() > 0)
	{
		p->setCurrentState(new ThinkingState());
	}
	p->getCurrentState()->OnEnter(p);
}

void RequestingAmmoState::OnExit(NPC* p)
{
	p->setRequestingAmmo(false);
}
