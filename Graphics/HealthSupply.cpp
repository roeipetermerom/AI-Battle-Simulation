#include "HealthSupply.h"
#include "glut.h"
#include "definitions.h"


HealthSupply::HealthSupply(double yy, double xx, int t) {
	x = xx;
	y = yy;
	team = t;
	green = 1;
	red = blue = 0;
}


void HealthSupply::show()
{
	glColor3d(red, green, blue);
	glBegin(GL_POLYGON);
	glVertex2d(x, y+ DRAW_BUFFER);
	glVertex2d(x- DRAW_BUFFER,y- DRAW_BUFFER);
	glVertex2d(x+ DRAW_BUFFER,y- DRAW_BUFFER);
	glEnd();
}