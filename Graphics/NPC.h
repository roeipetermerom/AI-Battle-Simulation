#pragma once

#include "State.h"
#include "AmmoSupply.h"
#include "HealthSupply.h"
#include "definitions.h"
#include "CompareCells.h";
#include "Cell.h";
#include <queue>;
#include "Bullet.h";
#include "Grenade.h";


using namespace std;
const double SPEED = 0.001;

class State;
class NPC
{
private:
	bool shotFired = false;
	bool grenadeThrown = false;
	bool isMoving;
	bool attacking, retreating,requestingAmmo,requestingHealth;
	int ammo;
	int grenadeAmmo;
	int team;
	NPC* enemyTeam[3];
	Bullet* npcBullet;
	Grenade* npcGrenade;
	NPC* medicNpc;
protected:
	int health;
	State* pCurrentState;
	double targetX, targetY;
	int x, y;
	double security_map[MSZ][MSZ];
	double red, green, blue;

public:
	NPC(double xx, double yy,int team);
	void show();
	double calculateDistance(double x1, double y1, double x2, double y2);
	virtual void DoSomething(int maze[MSZ][MSZ], double sm[MSZ][MSZ]);
	void setIsMoving(bool value) { isMoving = value; }
	void setTarget(double x, double y) { targetX = x;  targetY = y; }
	void setCurrentState(State* ps) { pCurrentState = ps; }
	State* getCurrentState() { return pCurrentState; }
	void setColor(double r, double g, double b) { red = r; green = g; blue = b; }
    void setEnemyTeam(NPC* enemyteam[3]) { 
        for (int i = 0; i < 3; ++i) {
            enemyTeam[i] = enemyteam[i];
        }
    }
	void setX(double xx) { x = xx; }
	void setY(double yy) { y = yy; }
	double getX() { return x; }
	double getY() { return y; }
	int getHealth() { return health; }
	int getAmmo() { return ammo; }
	void setSecurityMap(double sm[MSZ][MSZ]);

	void AddNeighbor(int maze[MSZ][MSZ], double sm[MSZ][MSZ],int r, int c, Cell* pCurrent,
		std::priority_queue<Cell*, std::vector<Cell*>, CompareCells>& pq,
		std::vector<Cell>& grays, std::vector<Cell>& black);
	void AstarSearch(int maze[MSZ][MSZ], double sm[MSZ][MSZ]);
	void moveTowardsTarget(Cell* pn);
	void findTargetBFS(int maze[MSZ][MSZ]);
	bool CheckNeighbor(int row, int col, Cell* pCurrent, int maze[MSZ][MSZ], queue <Cell*> *grays);
	void setAttacking(bool attack) { attacking = attack; }
	void setRetreating(bool retreat) { retreating = retreat; }
	void setRequestingAmmo(bool ammo) { requestingAmmo = ammo; }
	void setRequestingHealth(bool health) { requestingHealth = health; }
	double calculateAngle(double x1, double y1, double x2, double y2);
	void dmgEnemy();
	void setHealth(int h) { health = h; }
	bool getshotFired() { return shotFired; }
	Bullet* getBullet() { return npcBullet; }
	double getTargetX() { return targetX; }
	double getTargetY() { return targetY; }
	void setShotFired(bool value) { shotFired = value; }
	void setAmmo(int a) { ammo = a; }
	void setMedic(NPC* m) { medicNpc = m; }
	int getTeam() { return team; }
	bool getRequestingAmmo() { return requestingAmmo; }
	bool getRequestingHealth() { return requestingHealth; }
	bool getGrenadeThrown() { return grenadeThrown; }
	void setGrenadeThrown(bool value) { grenadeThrown = value; }
	Grenade* getGrenade() { return npcGrenade; }
	void dmgEnemy(int index);
};


