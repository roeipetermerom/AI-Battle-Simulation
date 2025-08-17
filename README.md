## AI-Battle-Simulation
Two teams of computer-controlled characters fight for survival in a dungeon full of rooms, corridors, supplies, and obstacles.

The project demonstrates how artificial intelligence can be used to create smart decision-making characters in a game environment.

## Features

🕹 Two Teams – each with 2 Fighters + 1 Carrier
🔫 Combat System – single-shot bullets and grenades
💊 Supplies – random ammo & health depots on the map
🧱 Obstacles & Cover – characters can hide strategically
🤝 Teamwork – carriers deliver ammo/health to teammates
🎭 Personalities – aggressive or cautious fighters
🗺 Pathfinding – A* algorithm for navigation
🔄 Decision Making – Finite State Machine (FSM) controls goals and priorities
⚡ Dynamic Safety Maps – movement adapts to ongoing battles

## Gameplay Description

The simulation consists of two teams, each containing:
- 2 Fighters (combat units)
- 1 Carrier (non-combat support unit)

The objective of each team is to eliminate the opposing team.

# Environment

- The world is a dungeon map with rooms and corridors.
- Ammo depots and health supplies (2 of each type) are placed randomly on the map.
- Obstacles are scattered in rooms, creating cover zones that can be used tactically.

## AI Features

1. Finite State Machine (FSM)
   Characters switch between states like Patrolling, Chasing, Attacking, Reloading, Requesting Help.
   Decision-making is dynamic and depends on health, ammo, and environment.

2. Pathfinding with A*
   Characters move intelligently across rooms and corridors.
   When a battle occurs, the safety map is updated, forcing characters to re-evaluate their paths.

4. Personality System
   Each fighter is assigned random traits:
   - Aggressive (prefers combat)
   - Cautious (avoids unnecessary risks)
   
   Different team compositions may lead to different outcomes.

5. Teamwork
   Carriers do not fight but deliver ammo and health kits.
   Fighters can request supplies when needed.

6. Combat System
   Two attack types are supported:
   - Single bullet shots
   - Grenade throws
   
   No friendly fire between teammates.

## Tech Stack

- Platform: Visual Studio
- Language: C++
- Graphics: OpenGL (GLUT)

## Installation

1. Clone or download the project.
2. Open the project in your IDE.
3. Build and run the executable.
4. Watch as the two AI teams battle for survival.


<img width="1741" height="1740" alt="image" src="https://github.com/user-attachments/assets/01011035-382f-4be2-814b-6587b25cfafb" />

<img width="416" height="467" alt="image" src="https://github.com/user-attachments/assets/856b30bf-99b2-455c-b991-21f93bd63df5" />

