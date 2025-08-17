#include "NPC.h"
#include "glut.h"
#include <math.h>
#include "definitions.h"
#include "AttackState.h"
#include "CompareCells.h"
#include <queue>
#include "Cell.h"
#include "Room.h"
#include <vector>
#include <iostream>
#include "MedicNPC.h"
#include <stdlib.h>
#include <cmath>
#include <print>

NPC::NPC(double xx, double yy, int t)
{
	x = xx;
	y = yy;
	isMoving = false;
	attacking = retreating = false;
	pCurrentState = new AttackState();
	pCurrentState->OnEnter(this);
	if (t) 
	{
			red = 1;
			green = 0;
			blue = 0;
	}
	else {
	
			red = 0;
			green = 0;
			blue = 1;	
		
	}
	health = 10;
	ammo = 5;
	grenadeAmmo = 3;
	team = t;
	npcBullet = nullptr;
	npcGrenade = nullptr;
	requestingHealth = false;
	requestingAmmo = false;

}

double NPC::calculateAngle(double x1, double y1, double x2, double y2) {
	double deltaY = y2 - y1;
	double deltaX = x2 - x1;

	// atan2 returns the angle in radians, convert to degrees
	double angleRad = atan2(deltaY, deltaX);
	double angleDeg = angleRad * (180.0 / PI);

	return angleDeg;
}

void NPC::show()
{
	if (health <= 0)
		glColor3d(0, 0, 0);
	else
		glColor3d(red, green, blue);
	
	glBegin(GL_POLYGON);
	glVertex2d(x - DRAW_BUFFER, y);
	glVertex2d(x, y + DRAW_BUFFER);
	glVertex2d(x + DRAW_BUFFER, y);
	glVertex2d(x, y - DRAW_BUFFER);
	glEnd();

	glColor3d(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2d(x - DRAW_BUFFER, y);
	glVertex2d(x, y + DRAW_BUFFER);
	glVertex2d(x + DRAW_BUFFER, y);
	glVertex2d(x, y - DRAW_BUFFER);
	glEnd();

}

double NPC::calculateDistance(double x1, double y1, double x2, double y2) {
	return std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
}

void generateGrenadePosition(int maze[MSZ][MSZ], int& grenadeX, int& grenadeY) {
	int originalX = grenadeX;
	int originalY = grenadeY;

	do {
		grenadeX = originalX;
		grenadeY = originalY;
		grenadeX += (rand() % 5) - 2;
		grenadeY += (rand() % 5) - 2;
	} while ((originalX == grenadeX && originalY == grenadeY) || maze[grenadeY][grenadeX] == WALL);
	
}

// execute the task
void NPC::DoSomething(int maze[MSZ][MSZ], double sm[MSZ][MSZ])
{
	if (health > 0) {
		if (attacking) {
			findTargetBFS(maze);
			double angle = calculateAngle(x, y, targetX, targetY);
			npcBullet = new Bullet(x, y, angle * PI / 180);
			bool los = npcBullet->hitScan(maze, targetX, targetY);

			bool throwGrenade = false;
			bool inRangeForGrenade = calculateDistance(x, y, targetX, targetY) < 15;
			if (ammo > 0 && grenadeAmmo>0) {
				throwGrenade = (rand() % 10 == 0  && inRangeForGrenade);
			}else if (grenadeAmmo > 0 && inRangeForGrenade) {
				throwGrenade = true;
			}

			if (los && (ammo>0 || grenadeAmmo >0)) {
				if (throwGrenade) { //throw granade
					grenadeThrown = true;
					grenadeAmmo--;

					int grenadeX = targetX;
					int grenadeY = targetY;
					generateGrenadePosition(maze, grenadeX, grenadeY);
					
					npcGrenade = new Grenade(grenadeY, grenadeX);
					int targetsHitArr[NUM_BULLETS] = { -1 };
					npcGrenade->hitScanGrenade(maze,targetsHitArr);
					for (int i = 0; i < NUM_BULLETS; i++) {
						if (targetsHitArr[i] != -1)
							dmgEnemy(targetsHitArr[i]);
					}
					npcGrenade = new Grenade(grenadeY, grenadeX);
					npcGrenade->explode();
					npcGrenade->setKillGrenade();
				}
				else { //shot a bullet
					shotFired = true;
					ammo--;
					dmgEnemy();
					npcBullet = new Bullet(x, y, angle * PI / 180);
					npcBullet->setKillShot(true);
					npcBullet->setIsMoving(true);
				}
			}
			else {
				AstarSearch(maze, sm);
			}

		}
		else if (requestingAmmo || requestingHealth) {
			if (x != medicNpc->getX() || y != medicNpc->getY()) {
				targetX = medicNpc->getX();
				targetY = medicNpc->getY();
				AstarSearch(maze, sm);
			}
		}
		
		pCurrentState->Transition(this);
	}	
}

void NPC::setSecurityMap(double sm[MSZ][MSZ]) {
    for (int i = 0; i < MSZ; ++i) {
        for (int j = 0; j < MSZ; ++j) {
            security_map[i][j] = sm[i][j];
        }
    }
}

void NPC::AddNeighbor(int maze[MSZ][MSZ], double sm[MSZ][MSZ], int r, int c, Cell* pCurrent,
	std::priority_queue<Cell*, std::vector<Cell*>, CompareCells>& pq,
	std::vector<Cell>& grays, std::vector<Cell>& black)// blacks shouldn't be changed
{
	double newg, cost;
	std::vector<Cell>::iterator itGray;
	std::vector<Cell>::iterator itBlack;



	cost = sm[r][c];
	newg = pCurrent->getG() + cost;
	Cell* pNeighbor = new Cell(r, c, pCurrent->getTargetRow(), pCurrent->getTargetCol(),
		newg, pCurrent);
	// check what to do with the neighbor Cell
	// 1. if the neighbor is black: do nothing
	// 2. if the neighbor is white: add it to PQ and to grays
	// 3. if it is gray:
	// 3.1 if F of neighbor is not below the neighbor copy that is stored in PQ then do nothing
	// 3.2 otherwise then we must update the PQ and grays
	itGray = find(grays.begin(), grays.end(), *pNeighbor);
	itBlack = find(black.begin(), black.end(), *pNeighbor);

	if (itBlack == black.end()) // then it is not black
	{
		if (itGray == grays.end()) // then it is not gray => it is white
		{
			// paint it gray
			pq.push(pNeighbor);
			grays.push_back(*pNeighbor);
		}
		else // it is gray
		{
			if (pNeighbor->getF() < itGray->getF()) // then we found a better path and we have to exchange it
			{
				grays.erase(itGray);
				grays.push_back(*pNeighbor);

				// and do the same with PQ
				vector<Cell*> tmp;
				while (!pq.empty() && !((*pq.top()) == (*pNeighbor)))
				{
					tmp.push_back(pq.top()); // save the top element in tmp
					pq.pop(); // remove top element from pq
				}
				if (pq.empty()) // ERROR!
				{
					cout << "PQ is empty\n";
					exit(2);
				}
				else // we have found the neighbor cell in PQ
				{
					pq.pop(); // remove old neighbor from pq
					pq.push(pNeighbor);
					// now restore pq
					while (!tmp.empty())
					{
						pq.push(tmp.back());
						tmp.pop_back();
					}
				}
			}
		}
	}


}

bool NPC::CheckNeighbor(int row, int col, Cell* pCurrent,int maze[MSZ][MSZ], queue <Cell*> *grays)
{
	if (maze[row][col] == TARGET)
	{
	
		targetX = col;
		targetY = row;
		return false;
	}
	else // maze[row][col] must be SPACE (WHITE)
	{
		Cell* pc = new Cell(row, col,0,0,0, pCurrent);
		maze[row][col] = GRAY;
		grays->push(pc);
		return true;
	}
}

void NPC::findTargetBFS(int maze[MSZ][MSZ])
{
	int rc[2];
	int maze2[MSZ][MSZ];
	queue <Cell*> grays;
	Cell* pCurrent;
	int row, col;
	bool go_on = true;
	bool targetFound = false;
	pCurrent = new Cell(y, x, 0, 0,0, nullptr);
	grays.push(pCurrent);

	for (int i = 0; i < MSZ; i++) {
		for (int j = 0; j < MSZ; j++)
		{
			if(maze[i][j] == TARGETONE || maze[i][j] == TARGETTWO || maze[i][j] == TARGETTHREE)
				maze2[i][j] = SPACE;
			else
				maze2[i][j] = maze[i][j];
		}
	}
		
	
	for (int k = 0; k < NUMSOLDIER; k++) {
			int enemyX = static_cast<int>(enemyTeam[k]->getX());
			int enemyY = static_cast<int>(enemyTeam[k]->getY());
			if (enemyTeam[k]->getHealth() > 0)
				maze2[enemyY][enemyX] = TARGET;
	}
	

	while (go_on) 
	{
       
		if (grays.empty())
		{
			return;
		}


		else // grays is not empty
		{
			pCurrent = grays.front();
			grays.pop(); // extract the first element from grays
			// 1. paint pCurrent black
			row = pCurrent->getRow();
			col = pCurrent->getCol();
			if (maze2[row][col] != START)
				maze2[row][col] = BLACK;
			// 2. check all the neighbors of pCurrent
			// go up
			if (maze2[row + 1][col] == SPACE || maze2[row + 1][col] == TARGET )
				go_on = CheckNeighbor(row + 1, col, pCurrent, maze2, &grays);
			// down
			if (go_on && (maze2[row - 1][col] == SPACE || maze2[row - 1][col] == TARGET))
				go_on = CheckNeighbor(row - 1, col, pCurrent, maze2,&grays);
			// left
			if (go_on && (maze2[row][col - 1] == SPACE || maze2[row][col - 1] == TARGET))
				go_on = CheckNeighbor(row, col - 1, pCurrent, maze2, &grays);
			// right
			if (go_on && (maze2[row][col + 1] == SPACE || maze2[row][col + 1] == TARGET))
				go_on = CheckNeighbor(row, col + 1, pCurrent, maze2, &grays);

		}
	}
	
}

void NPC::AstarSearch(int maze[MSZ][MSZ], double sm[MSZ][MSZ])
{
	int r, c, tr, tc;

	r = y;
	c = x;
	tr = targetY;
	tc = targetX;
	Cell* pCurrent;
	Cell* start = new Cell(r, c, tr, tc, 0, nullptr);
	priority_queue<Cell*, vector<Cell*>, CompareCells> pq;
	vector <Cell> grays;
	vector <Cell> black;
	vector<Cell>::iterator itGray;

	pq.push(start);
	grays.push_back(*start);
	// pq shouldn't be empty because we are going to reach the target beforehand
	while (!pq.empty())
	{
		pCurrent = pq.top();
		if (pCurrent->getH() < 0.001) // this is a targt cell
		{
			moveTowardsTarget(pCurrent);
			return;
		}
		else // target hasn't been reached
		{
			// 1. remove pCurrent from pq 
			pq.pop();
			// 2. find and remove pCurrent from grays
			itGray = find(grays.begin(), grays.end(), *pCurrent);
			if (itGray == grays.end()) // pCurrent wasn't found
			{
				cout << "Error: pCurrent wasn't found in grays\n";
				exit(1);
			}
			grays.erase(itGray);
			// 3. paint pCurrent black
			black.push_back(*pCurrent);
			// 4. take care of neighbors
			r = pCurrent->getRow();
			c = pCurrent->getCol();
			// up
			if (r + 1 < MSZ && maze[r + 1][c] != WALL)
				AddNeighbor(maze, sm, r + 1, c, pCurrent, pq, grays, black);
			// down-
			if (r - 1 >= 0 && maze[r - 1][c] != WALL)
				AddNeighbor(maze, sm, r - 1, c, pCurrent, pq, grays, black);
			// left
			if (c - 1 >= 0 && maze[r][c - 1] != WALL)
				AddNeighbor(maze, sm, r, c - 1, pCurrent, pq, grays, black);
			// right
			if (c + 1 < MSZ && maze[r][c + 1] != WALL)
				AddNeighbor(maze, sm, r, c + 1, pCurrent, pq, grays, black);
		}

	}
	cout << "Error pq is empty, target hasn't been found\n";
}

void NPC::moveTowardsTarget(Cell* pn)
{
	while (pn != nullptr)
	{
		Cell* parent = pn->getParent();
		if (parent->getParent() == nullptr) {
			setX(pn->getCol());
			setY(pn->getRow());
			break;
		}
		pn = parent;
	}
}

void NPC::dmgEnemy() {
	for (int i = 0; i < NUMSOLDIER; i++) {
		if(enemyTeam[i]->getX()==targetX && enemyTeam[i]->getY() == targetY)
			enemyTeam[i]->setHealth(enemyTeam[i]->getHealth() - 1);
	}
}

#include <iostream>

void NPC::dmgEnemy(int index) {
	enemyTeam[index]->setHealth(enemyTeam[index]->getHealth() - 1);
}
