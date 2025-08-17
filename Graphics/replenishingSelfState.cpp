#include "replenishingSelfState.h"
#include "MedicNPC.h"
#include "replenishingTeamateState.h"

void replenishingSelfState::OnEnter(NPC* p)
{
	((MedicNPC*)p)->setReplenishingSelf(true);
}

void replenishingSelfState::Transition(NPC* p)
{
	OnExit(p);

	if (((MedicNPC*)p)->getammoSupply() > 0 && ((MedicNPC*)p)->gethealthSupply() > 0)
	{
		p->setCurrentState(new replenishingTeamateState());
	}

	p->getCurrentState()->OnEnter(p);

}

void replenishingSelfState::OnExit(NPC* p)
{
	((MedicNPC*)p)->setReplenishingSelf(false);
}
