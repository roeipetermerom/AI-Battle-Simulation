#include "AttackState.h"
#include "ThinkingState.h"

using namespace std;

void AttackState::OnEnter(NPC* p)
{
	p->setIsMoving(true);
	p->setAttacking(true);
}

void AttackState::Transition(NPC* p)
{
	OnExit(p);

	//Todo BFS to look in the near by environment if a enempy is visibly, if so shoot the mothafucka
	p->setCurrentState(new ThinkingState());
	
	

	p->getCurrentState()->OnEnter(p);
	//Todo If BFS didn't see enemeies, apply A star search with a trarget provided by the BFS
}

void AttackState::OnExit(NPC* p)
{
	p->setIsMoving(false);
	p->setAttacking(false);
}
