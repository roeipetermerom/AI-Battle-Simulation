#pragma once
class HealthSupply
{
private:
	int x, y;
	double red, green, blue;
	int team;
public:
	HealthSupply(double yy, double xx, int t);
	void show();
	int getX() { return x; }
	int getY() { return y; }
};

