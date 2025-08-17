#pragma once
#include "Bullet.h"

const int NUM_BULLETS = 10;

class Grenade
{
private:
	double row, col;
	Bullet* bullets[NUM_BULLETS];
public:
	Grenade(double r, double c);
	void explode();
	void expand(int maze[MSZ][MSZ]);
	void show();
	void SimulateExplosion(int maze[MSZ][MSZ], double sm[MSZ][MSZ]);
	void hitScanGrenade(int maze[MSZ][MSZ],int* arr);
	void setKillGrenade();
	bool dmgGrenade(int maze[MSZ][MSZ]);
};