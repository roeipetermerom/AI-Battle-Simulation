#include "ThinkingState.h"
#include "AttackState.h"
#include "RequestingAmmoState.h"
#include "RequestingHealthState.h"

void ThinkingState::OnEnter(NPC* p)
{

}

void ThinkingState::Transition(NPC* p)
{
	OnExit(p);

	if (p->getHealth() > 5&& p->getAmmo()>0)
	{
		p->setCurrentState(new AttackState());
	}
	else if (p->getHealth() <= 5 && p->getAmmo() > 0)
	{
		p->setCurrentState(new RequestingHealthState());
	}
	else if (p->getAmmo() <= 0) {
		p->setCurrentState(new RequestingAmmoState());
	}

	p->getCurrentState()->OnEnter(p);
}

void ThinkingState::OnExit(NPC* p)
{
	p->setAttacking(false);
	p->setRetreating(false);
	p->setRequestingAmmo(false);
	p->setRequestingHealth(false);
}
