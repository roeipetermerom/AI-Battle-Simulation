#include "replenishingTeamateState.h"
#include "MedicNPC.h"
#include "replenishingSelfState.h"

void replenishingTeamateState::OnEnter(NPC* p)
{
	((MedicNPC*)p)->setReplenishingTeamate(true);
}

void replenishingTeamateState::Transition(NPC* p)
{
	OnExit(p);

	if (((MedicNPC*)p)->getammoSupply() == 0 || ((MedicNPC*)p)->gethealthSupply() == 0)
	{
		p->setCurrentState(new replenishingSelfState());
	}

	p->getCurrentState()->OnEnter(p);
}

void replenishingTeamateState::OnExit(NPC* p)
{
	((MedicNPC*)p)->setReplenishingTeamate(false);
}