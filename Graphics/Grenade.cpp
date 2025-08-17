#include "Grenade.h"

Grenade::Grenade(double r, double c)
{
	int i;
	row = r;
	col = c;
	double alpha = 2*PI/NUM_BULLETS; // bullet separation angle
	for (i = 0;i < NUM_BULLETS;i++)
	{
		bullets[i] = new Bullet(c, r, i * alpha);
	}
}

void Grenade::explode()
{
	int i;

	for (i = 0;i < NUM_BULLETS;i++)
	{
		bullets[i]->setIsMoving(true);
	}


}
void Grenade::setKillGrenade()
{
	int i;

	for (i = 0; i < NUM_BULLETS; i++)
	{
		bullets[i]->setKillShot(true);
	}


}

void Grenade::expand(int maze[MSZ][MSZ])
{
	int i;

	for (i = 0;i < NUM_BULLETS;i++)
	{
		bullets[i]->move(maze);
	}

}

void Grenade::hitScanGrenade(int maze[MSZ][MSZ],int* arr)
{
	bool bulletIsMoving = true;
	explode();
	while(bulletIsMoving){
		bulletIsMoving = false;
		for (int i = 0; i < NUM_BULLETS; i++)
		{
			if (bullets[i]->dmgShot(maze, 52, 7) == true) {
				int target = maze[(int)bullets[i]->getY()][(int)bullets[i]->getX()];
				switch (target) {
				case TARGETONE:
					arr[i] = 0;
					break;
				case TARGETTWO:
					arr[i] = 1;
					break;
				case TARGETTHREE:
					arr[i] = 2;
					break;
				}
			}
		}
		for (int i = 0; i < NUM_BULLETS; i++) {

			if (bullets[i]->getIsMoving() == true)
				bulletIsMoving = true;
		}
	}
	
	
}



void Grenade::show()
{
	int i;

	for (i = 0;i < NUM_BULLETS;i++)
	{
		bullets[i]->show();
	}

}

void Grenade::SimulateExplosion(int maze[MSZ][MSZ], double sm[MSZ][MSZ])
{
	int i;

	for (i = 0;i < NUM_BULLETS;i++)
	{
		bullets[i]->SimulateExplosion(maze,sm);
	}

}

bool Grenade::dmgGrenade(int maze[MSZ][MSZ])
{
	bool bulletIsMoving = false;
	int i;
	for (i = 0; i < NUM_BULLETS; i++)
	{
		bullets[i]->dmgShot(maze, 52, 7);
		if (bullets[i]->getIsMoving() == true)
			bulletIsMoving = true;
	}

	return bulletIsMoving;
}