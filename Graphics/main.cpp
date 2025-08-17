
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "glut.h"
#include <queue>
#include "Cell.h"
#include <iostream>
#include "Room.h"
#include "CompareCells.h"
#include "Bullet.h"
#include "Grenade.h"
#include "MedicNPC.h"
#include "AmmoSupply.h"
#include "definitions.h"
#include "HealthSupply.h"
#include <deque>
#include <windows.h>

using namespace std;

const int WIDTH = 700;
const int HEIGHT = 700;

const int NUM_ROOMS = 12;


Room* rooms[NUM_ROOMS];

bool bulletFired = false;
bool grenadeThrown = false;
bool startFight = false;
bool gameOver = false;
Grenade* pg = nullptr;
Bullet* pb = nullptr;
AmmoSupply* ateam1 = nullptr;
AmmoSupply* ateam2 = nullptr;

HealthSupply* hteam1 = nullptr;
HealthSupply* hteam2 = nullptr;

NPC* team1[NUMSOLDIER];
NPC* team2[NUMSOLDIER];


std::deque<NPC*> npcQueue;
NPC* currentNPC = nullptr;
bool scanningShotActive = false;

int winTeamInteger;



int maze[MSZ][MSZ] = { 0 }; // WALLs
double security_map[MSZ][MSZ] = {0}; // 


void RestorePath(Cell* pc)
{
	int r, c;
	while (pc != nullptr)
	{
		r = pc->getRow();
		c = pc->getCol();
		if (maze[r][c] == WALL)
			maze[r][c] = SPACE;
		pc = pc->getParent();
	}
}

// row, col are the indices of neighbor cell
void AddNeighbor(int r, int c, Cell* pCurrent,
	priority_queue<Cell*, vector<Cell*>, CompareCells>& pq,
	vector <Cell>& grays,  vector <Cell> &black) // blacks shouldn't be changed
{
	double newg,cost;
	vector<Cell>::iterator itGray;
	vector<Cell>::iterator itBlack;


	if (maze[r][c] == WALL) cost = WALL_COST;
	else cost = SPACE_COST;
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
				while (!pq.empty() &&  !((*pq.top()) == (*pNeighbor)))
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

// run A* from room at index1 to room at index2
void BuildPath(int index1, int index2)
{
	int r, c, tr, tc;

	r = rooms[index1]->getCenterY();
	c = rooms[index1]->getCenterX();
	tr = rooms[index2]->getCenterY();
	tc = rooms[index2]->getCenterX();
	Cell* pCurrent;
	Cell* start = new Cell(r,c ,tr ,tc , 0, nullptr);
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
			RestorePath(pCurrent);
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
			if (r + 1 < MSZ)
				AddNeighbor(r + 1, c, pCurrent, pq, grays, black);
			// down-
			if (r - 1 >=0)
				AddNeighbor(r - 1, c, pCurrent, pq, grays, black);
			// left
			if (c - 1 >= 0)
				AddNeighbor(r , c-1, pCurrent, pq, grays, black);
			// right
			if (c + 1 < MSZ)
				AddNeighbor(r, c + 1, pCurrent, pq, grays, black);
		}

	}
	cout << "Error pq is empty, target hasn't been found\n";
}

void BuildPathBetweenTheRooms()
{
	int i, j;

	for (i = 0;i < NUM_ROOMS;i++)
		for (j = i + 1;j < NUM_ROOMS;j++)
		{
			BuildPath(i, j); // A*
			cout << "The path from " << i << " to " << j << " has been paved\n";
		}
}

void CalculateCord(int team1spawn, int& randomX, int& randomY)
{
	int centerX = rooms[team1spawn]->getCenterX();
	int centerY = rooms[team1spawn]->getCenterY();
	int width = rooms[team1spawn]->getWidth();
	int height = rooms[team1spawn]->getHeight();

	int leftBoundary = centerX - width / 2;
	int rightBoundary = centerX + width / 2;
	int topBoundary = centerY - height / 2;
	int bottomBoundary = centerY + height / 2;

	randomX = leftBoundary + rand() % width;
	randomY = topBoundary + rand() % height;
}

void SetupDungeon()
{
	int i,j;
	int cx, cy, w, h;
	bool hasOverlap;

	for (i = 0;i < NUM_ROOMS;i++)
	{
		do
		{
			hasOverlap = false;
			w = 6 + rand() % (MSZ / 5);
			h = 6 + rand() % (MSZ / 5);
			cx = 2 + w / 2 + rand() % (MSZ - w - 4);
			cy = 2 + h / 2 + rand() % (MSZ - h - 4);
			for(j=0;j<i && !hasOverlap;j++)
				hasOverlap = rooms[j]->Overlap(cx, cy, w, h);
		} while (hasOverlap); // check the validity of the room
			
		rooms[i] = new Room(cx, cy, w, h,maze);
	}

	for (i = 0;i < 300;i++)
		maze[rand() % MSZ][rand() % MSZ] = WALL;

	for ( i = 0; i < 4; )
	{
		int x = rand() % MSZ;
		int y = rand() % MSZ;
		if (maze[y][x] == SPACE) 
		{
			if (i % 2 == 0) 
			{
				if(i==0)
					ateam1 = new AmmoSupply(y, x, TEAM1);
				else
					hteam1 = new HealthSupply(y, x, TEAM1);
				i++;
			}
			else{
				if (i == 1)
					ateam2 = new AmmoSupply(y, x, TEAM2);
				else
					hteam2 = new HealthSupply(y, x, TEAM2);
				i++;
			}
				

		}
			
	}
	
	
	int team1spawn = rand() % NUM_ROOMS;
	int team2spawn;
	do{
		team2spawn = rand() % NUM_ROOMS;
	} while (team1spawn == team2spawn);

	for (i = 0; i < NUMSOLDIER; i++)
	{
		int randomX1, randomY1, randomX2, randomY2;
		CalculateCord(team1spawn, randomX1, randomY1);
		CalculateCord(team2spawn, randomX2, randomY2);
		if (i == NUMSOLDIER-1) {
			team1[i] = new MedicNPC(randomX1, randomY1, ateam1, hteam1, TEAM1,team1[0],team1[1], ateam1, hteam1);
			team2[i] = new MedicNPC(randomX2, randomY2, ateam2, hteam2, TEAM2, team2[0], team2[1], ateam2, hteam2);
		}
		else {
			team1[i] = new NPC(randomX1, randomY1, TEAM1);
			team2[i] = new NPC(randomX2, randomY2, TEAM2);
		}
		
		
	}
	for (int k = 0; k < NUMSOLDIER; k++) {
		team1[k]->setEnemyTeam(team2);
		team2[k]->setEnemyTeam(team1);

	}

	for (int i = 0; i < NUMSOLDIER-1; i++)
	{
		team1[i]->setMedic(team1[2]);
		team2[i]->setMedic(team2[2]);
	}

 // setup queue
	for (int i = 0; i < NUMSOLDIER; i++)
	{
		npcQueue.push_back(team1[i]);
		npcQueue.push_back(team2[i]);
	}

	// Set the first NPC in the queue as the current NPC
	if (!npcQueue.empty()) {
		currentNPC = npcQueue.front();
	}
	else {
		currentNPC = nullptr; // Safety check in case there are no NPCs
	}


	BuildPathBetweenTheRooms();

}

void init()
{
	glClearColor(0.5, 0.5, 0.5, 0);// color of window background
	glOrtho(0, MSZ, 0, MSZ, -1, 1); // set the coordinates system

	srand(time(0));

	SetupDungeon();
}

void ShowDungeon()
{
	int i, j;
	double s;

	for(i=0;i<MSZ;i++)
		for (j = 0;j < MSZ;j++)
		{
			s = security_map[i][j];
			//1. set color of cell
			switch (maze[i][j])
			{
			case SPACE:
				//glColor3d(1-s, 1-s, 1-s); // gray
				glColor3d(1, 1, 1); // gray
				break;
			case WALL:
				glColor3d(0.3, 0.3, 0.4); // dark gray
				break;
			}
			// show cell
			glBegin(GL_POLYGON);
			glVertex2d(j, i);
			glVertex2d(j, i + 1);
			glVertex2d(j+1, i + 1);
			glVertex2d(j + 1, i );
			glEnd();

			ateam1->show();
			ateam2->show();
			hteam1->show();
			hteam2->show();

			for (int i = 0; i < NUMSOLDIER; i++)
			{
				team1[i]->show();
				team2[i]->show();
			}
		}
}

void GenerateSecurityMap()
{
	int numSimulations = 1000;
	int i;

	for (i = 0;i < numSimulations;i++)
	{
		Grenade* g = new Grenade(rand() % MSZ, rand() % MSZ);

		g->SimulateExplosion(maze, security_map);
	}

}

void GenerateSecurityMapLoop(int times) {
	for (int i = 0; i < times; i++)
	{
		GenerateSecurityMap();
	}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer

	ShowDungeon();
	if (pb != nullptr)
		pb->show();
	if (pg != nullptr)
		pg->show();
	if (!gameOver) {
		for (NPC* npc : npcQueue) {
			npc->show();
			if (npc->getBullet() != nullptr)
				npc->getBullet()->show();
			if (npc->getGrenade() != nullptr)
				npc->getGrenade()->show();
		}
	}
	else {
		// Set text position in the center of the screen
		int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
		int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);

		// Set up 2D orthographic projection (fixed for 800x600)
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, windowWidth, 0, windowHeight, -1, 1);  // Coordinates from bottom-left (0,0) to top-right (windowWidth, windowHeight)

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		const char* text;
		// Define the string to display
		if(winTeamInteger == 0)
			text = "Team Blue Wins!";
		else
			text = "Team Red Wins!";

		// Calculate the width of the text
		int textLength = strlen(text);
		int textWidth = textLength * 9; // Assuming each character is about 9 pixels wide

		// Set the position of the string in the center of the screen
		glRasterPos2f((windowWidth - textWidth) / 2, windowHeight / 2);

		// Render the string character by character
		for (int i = 0; i < textLength; i++) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
		}

		glFlush();
	}
	

	glutSwapBuffers(); // show all
}

void updateSecurityMap(int enemyTeam, NPC* currentSoldier)
{

	int r, c;
	double update_security_map[MSZ][MSZ] = { 0 }; // 

	for (int i = 0; i < NUMSOLDIER; i++)
	{
		if (enemyTeam == TEAM1)
		{
			r = team1[i]->getY();
			c = team1[i]->getX();
		}
		else
		{
			r = team2[i]->getY();
			c = team2[i]->getX();
		}
		Grenade* g = new Grenade(r, c);


		g->SimulateExplosion(maze, update_security_map);
	}
	for (int i = 0; i < MSZ; i++)
		for (int j = 0; j < MSZ; j++)
			update_security_map[i][j] += security_map[i][j];
	currentSoldier->setSecurityMap(update_security_map);
}

void updateCurrentNPC()
{
	if (npcQueue.empty()) return;

	// Move current NPC to the back of the queue
	npcQueue.push_back(npcQueue.front());
	npcQueue.pop_front();

	// Update currentNPC to the new front of the queue
	currentNPC = npcQueue.front();
}

void checkWinCondition() {
	bool team1Dead = true;
	bool team2Dead = true;
	for (int i = 0; i < NUMSOLDIER; i++)
	{
		if (team1[i]->getHealth() > 0)
			team1Dead = false;
		if (team2[i]->getHealth() > 0)
			team2Dead = false;
	}
	if (team1Dead)
	{
		startFight = false;
		gameOver = true;
		winTeamInteger=1;
	}
	if (team2Dead)
	{
		startFight = false;
		gameOver = true;
		winTeamInteger = 0;
	}
}

void updateMaze(int maze[MSZ][MSZ], double sm[MSZ][MSZ], bool isTeam1, bool erase)
{
	if (isTeam1) {
		if (team1[0]->getHealth() > 0)
			maze[static_cast<int>(team1[0]->getY())][static_cast<int>(team1[0]->getX())] = erase ? SPACE : TARGETONE;
		if (team1[1]->getHealth() > 0)
			maze[static_cast<int>(team1[1]->getY())][static_cast<int>(team1[1]->getX())] = erase ? SPACE : TARGETTWO;
		if (team1[2]->getHealth() > 0)
			maze[static_cast<int>(team1[2]->getY())][static_cast<int>(team1[2]->getX())] = erase ? SPACE : TARGETTHREE;
	}
	else {
		if (team2[0]->getHealth() > 0)
			maze[static_cast<int>(team2[0]->getY())][static_cast<int>(team2[0]->getX())] = erase ? SPACE : TARGETONE;
		if (team2[1]->getHealth() > 0)
			maze[static_cast<int>(team2[1]->getY())][static_cast<int>(team2[1]->getX())] = erase ? SPACE : TARGETTWO;
		if (team2[2]->getHealth() > 0)
			maze[static_cast<int>(team2[2]->getY())][static_cast<int>(team2[2]->getX())] = erase ? SPACE : TARGETTHREE;
	}
}

void idle()
{
	//if (bulletFired)
	//	pb->move(maze);

	//if (grenadeThrown)
	//	pg->expand(maze);
	bool updated = false;
	if (startFight) {
		if (currentNPC != nullptr) {
			//bool temp = currentNPC->getTeam() == TEAM1 ? updateSecurityMap(TEAM2, currentNPC) : updateSecurityMap(TEAM1, currentNPC);

			// update the maze for target locations
			bool currIsTeamOne = currentNPC->getTeam() == TEAM1;
			if (currIsTeamOne) {
				updateMaze(maze, security_map, !currIsTeamOne, false);
				updateSecurityMap(TEAM2, currentNPC);
			}
			else {
				updateMaze(maze, security_map, !currIsTeamOne, false);
				updateSecurityMap(TEAM1, currentNPC);
			}

			if (!currentNPC->getshotFired() && !currentNPC->getGrenadeThrown()) {
				// NPC takes its action
				currentNPC->DoSomething(maze, security_map);
			}
			else if (currentNPC->getshotFired())
			{
				Bullet* bullet = currentNPC->getBullet();
				bool enemyWasHit = bullet->dmgShot(maze, currentNPC->getTargetX(), currentNPC->getTargetY());
				if (enemyWasHit) {
					currentNPC->setShotFired(false);
					updateCurrentNPC(); // Move to next NPC in queue
					updated = true;
				}
			}
			else if (currentNPC->getGrenadeThrown()) { //simulate granade explosion
				Grenade* grenade = currentNPC->getGrenade();
				if (!grenade->dmgGrenade(maze)) {
					currentNPC->setGrenadeThrown(false);
					updateCurrentNPC(); // Move to next NPC in queue
					updated = true;
				}
			}
			//remove target locations from maze
			if (!currentNPC->getshotFired() && !currentNPC->getGrenadeThrown() && updated == false) {
				updateCurrentNPC(); // Move to next NPC in queue
			}

			if (currIsTeamOne) {
				updateMaze(maze, security_map, !currIsTeamOne, true);
			}
			else {
				updateMaze(maze, security_map, !currIsTeamOne, true);
			}

		}
	}
	checkWinCondition();
	glutPostRedisplay(); // Indirect call to display
}

void menu(int choice)
{
	switch (choice)
	{
	case 1: // fire bullet
		bulletFired = true;
		pb->setIsMoving(true);
		break;
	case 2: // throw grenade
		grenadeThrown = true;
		pg->explode();
		break;
	case 3: // security map
		GenerateSecurityMap();
		break;
	case 4:3;
		GenerateSecurityMapLoop(1);
		startFight = true;
		break;
	}
}

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
//		pb = new Bullet(MSZ*x/(double)WIDTH,
//			MSZ* (HEIGHT - y) / (double)HEIGHT, (rand() % 360)* PI / 180);

		pg = new Grenade(MSZ * (HEIGHT - y) / (double)HEIGHT, MSZ * x / (double)WIDTH);
	}
}

void main(int argc, char* argv[]) 
{
	glutInit(&argc, argv);
	// definitions for visual memory (Frame buffer) and double buffer
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(600, 20);
	glutCreateWindow("BFS");

	// display is a refresh function
	glutDisplayFunc(display);
	// idle is a update function
	glutIdleFunc(idle);
	
	glutMouseFunc(mouse);

	// menu
	glutCreateMenu(menu);
	glutAddMenuEntry("Fire bullet", 1);
	glutAddMenuEntry("Throw Grenade", 2);
	glutAddMenuEntry("Generate Security Map", 3);
	glutAddMenuEntry("Start fight", 4);

	glutAttachMenu(GLUT_RIGHT_BUTTON);


	init();

	glutMainLoop();
}