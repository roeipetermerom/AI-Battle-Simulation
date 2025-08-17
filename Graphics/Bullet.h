#pragma once
#include "definitions.h"
class Bullet
{
private:
	double x, y;
	double dir_angle;
	double dirX, dirY;
	double speed;
	bool isMoving;
	bool killShot = false;
public:
	Bullet(double xx, double yy, double angle);
	void move(int maze[MSZ][MSZ]);
	void show();
	void setIsMoving(bool value) { isMoving = value; }
	void SimulateExplosion(int maze[MSZ][MSZ], double sm[MSZ][MSZ]);
	bool hitScan(int maze[MSZ][MSZ], double targetX, double targetY);
	bool dmgShot(int maze[MSZ][MSZ], double targetX, double targetY);
	bool getIsMoving() { return isMoving; }
	void setKillShot(bool value) { killShot = value; }
	int getX() { return x; }
	int getY() { return y; }
};
