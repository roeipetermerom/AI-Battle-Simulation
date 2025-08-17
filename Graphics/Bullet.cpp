#include "Bullet.h"
#include <math.h>
#include "glut.h"

// angle is in radians
Bullet::Bullet(double xx, double yy, double angle)
{
	x = xx;
	y = yy;
	dir_angle = angle;
	dirX = cos(angle);
	dirY = sin(angle);
	speed = 1;
	isMoving = false;
}

void Bullet::move(int maze[MSZ][MSZ])
{
	if (isMoving)
	{
		x += speed * dirX;
		y += speed * dirY;
		if (maze[(int)y][(int)x] == WALL)
			isMoving = false;
	}
}

void Bullet::show()
{
	if (killShot && isMoving) {
		glColor3d(1, 0, 0);
		glBegin(GL_POLYGON);
		glVertex2d(x - 0.2, y);
		glVertex2d(x, y + 0.2);
		glVertex2d(x + 0.2, y);
		glVertex2d(x, y - 0.2);
		glEnd();
	}
	
}

void Bullet::SimulateExplosion(int maze[MSZ][MSZ], double sm[MSZ][MSZ])
{
	isMoving = true;
	while (isMoving)
	{
		sm[(int)y][(int)x] += SECURITY_FACTOR;
		move(maze);
	}
}

bool Bullet::hitScan(int maze[MSZ][MSZ], double targetX , double targetY){
	isMoving = true;
	while (isMoving) {
		move(maze);
		if (maze[(int)y][(int)x] == WALL) {
			isMoving = false;
			return false;
		}
		if ((int)x == (int)targetX && (int)y == (int)targetY) {
			isMoving = false;
			return true;
		}
	}
}



bool Bullet::dmgShot(int maze[MSZ][MSZ], double targetX, double targetY) {


	
	if (maze[(int)y][(int)x] == WALL) {
		return false;
	}
	if (isMoving==true && maze[(int)y][(int)x] == TARGETONE || maze[(int)y][(int)x] == TARGETTWO || maze[(int)y][(int)x] == TARGETTHREE) {
		isMoving = false;
		return true;
	}

	move(maze);
	return false;
}
