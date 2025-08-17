#pragma once
#include "NPC.h"

class AmmoSupply;
class HealthSupply;
class MedicNPC : public NPC
{

private:
	bool needAmmo, needHealth;
    int ammoSupply;
    int healthSupply;
    NPC* teamates[2];
    AmmoSupply* ammoStorage;
    HealthSupply* healthStorage;
	bool replenishingSelf, replenishingTeamate;
public:
    MedicNPC(double xx, double yy, AmmoSupply* ammo, HealthSupply* health, int t, NPC* m1, NPC* m2, AmmoSupply* as, HealthSupply* hs);
	void DoSomething(int maze[MSZ][MSZ], double sm[MSZ][MSZ]) override;
	void setReplenishingSelf(bool value) { replenishingSelf = value; }
	void setReplenishingTeamate(bool value) { replenishingTeamate = value; }
	int getammoSupply() { return ammoSupply; }
	int gethealthSupply() { return healthSupply; }
};