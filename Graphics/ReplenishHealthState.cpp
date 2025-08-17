#include "ReplenishHealthState.h"


void ReplenishHealthState::OnEnter(NPC* p)
{
	p->setIsMoving(true);
}

void ReplenishHealthState::Transition(NPC* p)
{
	//Todo BFS to look in the nearby environment if a enempy is visibly, if so run away from the mothafucka




	//Todo If BFS didn't see enemeies, apply A star search with a traget assigned to the medic location
}

void ReplenishHealthState::OnExit(NPC* p)
{
	//Todo check health and ammo supplies, and alert the medic if help is needed
}
