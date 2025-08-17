#pragma once
class AmmoSupply
{
private:
	int x, y;
	double red, green, blue;
	int team;
public:
	AmmoSupply(double yy, double xx, int t);
	void show();
	int getX() { return x; }
	int getY() { return y; }
};

