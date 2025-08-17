#include "MedicNPC.h"
#include "glut.h"
#include "replenishingTeamateState.h"

const int MAXAMMO = 5;
const int MAXHEALTH = 10;

MedicNPC::MedicNPC(double xx, double yy, AmmoSupply* ammo, HealthSupply* health, int t, NPC* m1,NPC* m2,AmmoSupply* as, HealthSupply* hs)
    : NPC(xx, yy, t)
{
    if (t)
        setColor(1, 0.5, 0.5);
    else
        setColor(0.68, 0.85, 0.9);
         
    ammoSupply = 10;
    healthSupply = 10;
    teamates[0] = m1;
    teamates[1] = m2;
    ammoStorage = ammo;
    healthStorage = health;
	needAmmo = false;
	needHealth = false;
    pCurrentState = new replenishingTeamateState();
	pCurrentState->OnEnter(this);
}

void MedicNPC::DoSomething(int maze[MSZ][MSZ], double sm[MSZ][MSZ])
{
	if (health>0) {
		if (replenishingSelf) {
			if (ammoSupply == 0) {
				targetX = ammoStorage->getX();
				targetY = ammoStorage->getY();
				needAmmo = true;
			}
			else if (healthSupply == 0) {
				targetX = healthStorage->getX();
				targetY = healthStorage->getY();
				needHealth = true;
			}

			if (targetX == x && targetY == y && needAmmo)
			{
				ammoSupply = 100;
				needAmmo = false;
			}
			else if (targetX == x && targetY == y && needHealth)
			{
				healthSupply = 100;
				needHealth = false;
			}
			else {
				AstarSearch(maze, sm);
			}
		}
		else if (replenishingTeamate) {
			bool targetFound = false;
			NPC* tempTarget = nullptr;
			int distance = MSZ;
			int teamateIndex = -1;
			for (int i = 0; i < 2 && !targetFound; i++) {
				int tempDistance = 0;
				if (teamates[i]->getHealth() > 0) {
					tempDistance = calculateDistance(x,y,teamates[i]->getX(), teamates[i]->getY());
					if (tempDistance < distance) {
						distance = tempDistance;
						teamateIndex = i;
					}
				}
				if (teamates[i]->getHealth() > 0 && (teamates[i]->getRequestingAmmo() || teamates[i]->getRequestingHealth())) {
					targetX = teamates[i]->getX();
					targetY = teamates[i]->getY();
					tempTarget = teamates[i];
					targetFound = true;
				}
			}
			if (targetFound) {
				if (tempTarget->getX() == x && tempTarget->getY() == y) {
					int currentTargetAmmo = tempTarget->getAmmo();
					int currentTargetHealth = tempTarget->getHealth();
					int maxAmmoNeeded = MAXAMMO - currentTargetAmmo;
					int maxHealthNeeded = MAXHEALTH - currentTargetHealth;

					if (maxAmmoNeeded > 0) {
						if (ammoSupply >= maxAmmoNeeded) {
							tempTarget->setAmmo(MAXAMMO);
							ammoSupply -= maxAmmoNeeded;
						}
						else {
							tempTarget->setAmmo(currentTargetAmmo + ammoSupply);
							ammoSupply = 0;
						}
					}
					else if (maxHealthNeeded > 0) {
						if (healthSupply >= maxHealthNeeded) {
							tempTarget->setHealth(MAXHEALTH);
							healthSupply -= maxHealthNeeded;
						}
						else {
							tempTarget->setHealth(currentTargetHealth + healthSupply);
							healthSupply = 0;
						}
					}
				}
				else {
					AstarSearch(maze, sm);
				}
			}
			else if(teamateIndex != -1 && distance>10) {
				targetX = teamates[teamateIndex]->getX();
				targetY = teamates[teamateIndex]->getY();
				AstarSearch(maze, sm);
			}


		}

		pCurrentState->Transition(this);
	}
	
}


