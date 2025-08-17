#include "RequestingHealthState.h"
#include "ThinkingState.h"

void RequestingHealthState::OnEnter(NPC* p)
{
	p->setRequestingHealth(true);
}
void RequestingHealthState::Transition(NPC* p)
{
	OnExit(p);
	if (p->getHealth()>5)
	{
		p->setCurrentState(new ThinkingState());
	}
	p->getCurrentState()->OnEnter(p);

}
void RequestingHealthState::OnExit(NPC* p)
{
	p->setRequestingHealth(false);
}


