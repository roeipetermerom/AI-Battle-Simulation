#include "AmmoSupply.h"
#include "glut.h"
#include "definitions.h"


AmmoSupply::AmmoSupply(double yy, double xx, int t) {
	x = xx;
	y = yy;
	team = t;
	red = 0.58;
	green = 0.29;
	blue = 0.0;
}


void AmmoSupply::show()
{
	glColor3d(red, green, blue);
	glBegin(GL_POLYGON);
	glVertex2d(x, y - DRAW_BUFFER);
	glVertex2d(x - DRAW_BUFFER, y + DRAW_BUFFER);
	glVertex2d(x + DRAW_BUFFER, y + DRAW_BUFFER);
	glEnd();
}

