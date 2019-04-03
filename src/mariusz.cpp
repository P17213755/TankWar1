#include "Mariusz.h"

using namespace std;

Mariusz::Mariusz()
{
	map = new Map(); //Creates a new map object
	clearMovement();
}


Mariusz::~Mariusz()
{
}

void Mariusz::reset()
{
	//forwards = true;
	path.clear(); //clear the old path 
	flag = false;
}

void Mariusz::move()
{
	rect.setSize(sf::Vector2f(200, 10));
	rect.setPosition(sf::Vector2f(getX(), getY()));
	rect.setFillColor(sf::Color(0, 255, 0, 50));
	if (friendlyBaseSpotted == true)rect.setFillColor(sf::Color(255, 0, 0, 50));
	rect.setRotation(turretTh);

	setCurrentNode();
	//if current node = node on the path, remove the node on the path so that the movement machine reads the next node as it's new target.
	//Update is done here - will handle State Machines
	turretMachine();
	AimingMachine();
	navigationMachine();
	movementMachine();

}

void Mariusz::DrawNode(sf::RenderTarget &target) {

}

void Mariusz::turretMachine() {
	const int IDLE = 0;
	const int FIRING = 1;
	int state = IDLE;

	//Transition Variables
	if ((enemySpotted == true || baseSpotted == true) && hasAmmo() && hasLineOfSight == true && friendlyBaseSpotted == false) { //if there is line of sight and the bot has ammo then set state to fire 
		state = FIRING;
	}
	else {
		firing = false;
		state = IDLE;
	}

	//State Switch
	switch (state) {
	case IDLE: return; break;
	case FIRING: {
		amountScanned = 0;
		firing = true;
		enemySpotted = false;
		baseSpotted = false;
		hasLineOfSight = false;
	} break;
		//default: throw new Exception("Invalid turret state");
	}
}

void Mariusz::movementMachine() {
	const int IDLE = 0;
	const int MOVE = 1;
	const int ROTATE = 2;
	int state = IDLE;

	std::string states[3] = {
		"IDLE",
		"MOVE",
		"ROTATE"
	};

	if (path.size() != 0) { // go through the A* path
		targetNode = &path.front();
		if (targetNode->column == currentNode->column && targetNode->row == currentNode->row) {
			path.pop_front();
			if (path.size() != 0) {
				targetNode = &path.front();
			}
		}
	}

	float deltaX = getX() - targetNode->getX();
	float deltaY = getY() - targetNode->getY();
	angleInDegree = atan2(deltaY, deltaX) * 180 / PI;
	float neededAngle = angleInDegree + 180;
	float rotational = pos.getTh();
	float deltaR = rotational - neededAngle;

	//Local Variables
	int distanceThreshold = 5;
	int directionThreshold = 5;
	bool facingDirection = (deltaR < directionThreshold && deltaR > -directionThreshold);
	bool atLocation = (path.size() == 0);
	int direction;

	//Transition Conditionals
	if (state == IDLE) {
		if (facingDirection && !atLocation) {
			state = MOVE;
		}
		else if (!facingDirection && !atLocation) {
			state = ROTATE;
		}
		else {
			amountScanned++;
		}
	}
	else if (state == MOVE) {
		if (atLocation) {
			state = IDLE;
		}
	}
	else if (state == ROTATE) {
		if (facingDirection) {
			state = MOVE;
		}
	}

	//State Switch

	switch (state) {
	case IDLE: {stop(); //If idle then stop movement
	} break;
	case MOVE: goForward(); break;
	case ROTATE: { //handles which direction the tank is facing depending on the position of the next node
		if (deltaR > 1 && deltaR < 180) {
			goLeft();
		}
		else if (deltaR < -1 && deltaR > -180) {
			goRight();
		}
		else if (deltaR > 180) {
			goRight();
		}
		else if (deltaR < -180) {
			goLeft();
		}
	} break;
		//default: throw new Exception("Invalid turret state");
	}
}

void Mariusz::setCurrentNode() {
	const int xwidth = 780;
	const int xheight = 570;
	float x = getX() - 17.5f;
	float y = getY() - 17.5f;
	int nodeSize = 780 / 22;
	int mx = (x / 35);
	int my = (y / 35);

	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 22; j++) {
			map->mapArray[i][j]->setCurrent(false);
		}
	}
	map->mapArray[my][mx]->setCurrent(true);

	currentNode = map->mapArray[my][mx];

	//std::cout << "X: " << getX() << " TX: " << targetNode->getX() << " Y: " << getY() << " TY: " << targetNode->getY() << endl;
}

void Mariusz::AimingMachine() {
	const int SCAN = 0;
	const int AIM = 1;
	int scanState = SCAN;

	//int ammoReserved = getEnemyBases(); // Value to return minimum require ammo to win game.

	//Transition Variables
	if (scanState == SCAN && friendlyBaseSpotted == false) {
		if (enemySpotted == true) {
			scanState = AIM;
		}
		else if (baseSpotted == true) {
			scanState = AIM;
		}
	}

	if (scanState == AIM && (enemySpotted == false || baseSpotted == false) && friendlyBaseSpotted == true) {
		scanState == SCAN;
	}

	if (scanState == AIM && friendlyBaseSpotted == true) {
		scanState == SCAN;
	}

	//State Switch
	switch (scanState) {
	case SCAN: {
		turretGoRight(); //when scanning for a base or an enemy, make the turret go right		
						 //Look for target
						 //Set target
	} break;
	case AIM: {
		if (friendlyBaseSpotted == false) {
			float deltaR = turretTh - turretAngle;
			if (deltaR > 1 && deltaR < 180) {
				turretGoLeft();
			}
			else if (deltaR < -1 && deltaR > -180) {
				turretGoRight();
			}
			else if (deltaR > 180) {
				turretGoRight();
			}
			else if (deltaR < -180) {
				turretGoLeft();
			}
			else {
				hasLineOfSight = true;
				stopTurret();
				clearMovement();
			}
		}
	} break;
		//default: throw new Exception("Invalid turret state");
	}

}

void Mariusz::navigationMachine() {

	while (!flag) {
		int randomXLoc = rand() % 13 + 2;
		int randomYLoc = rand() % 5 + 2;
		if (map->mapArray[randomXLoc][randomYLoc]->Path == true) {
			map->AStar(path, *currentNode, *map->mapArray[randomXLoc][randomYLoc]);
			flag = true;
		}

	}

	const int IDLE = 0;
	const int MOVE_RAND = 1;
	const int MOVE_AWAY = 2;
	const int MOVE_CENTRE = 3;
	int state = IDLE;

	int enemyDistanceThreshold = 100;
	int wallDistanceThreshold = 5;
	//	
	//Transition Conditionals
	if (state == IDLE) {
		if (getEnemyDistance < enemyDistanceThreshold || (enemySpotted == true && numberOfShells > buildingsRemain)) {
			state = MOVE_AWAY;
		}
		else if (amountScanned > 300 && baseSpotted == false) {
			flag = false;
			state = MOVE_RAND;
		}
	}
	else if (state == MOVE_RAND) {
		//if the enemy is within range of the player then stop so the Tank can shoot 
		if (getEnemyDistance < enemyDistanceThreshold || enemySpotted == true && numberOfShells > buildingsRemain) {
			clearMovement();
		}
		else { state = IDLE; }
	}
	else if (state == MOVE_AWAY) {
		if (getEnemyDistance > enemyDistanceThreshold) {//if the Tank is out of range of the player then go back to idle
			state = IDLE;
		}
		else if (getX() < 10 || getX() > 770 || getY() < 10 || getY() > 560) { //if the Tank has reached the border then go back to the centre
			flag = false;
			state = MOVE_CENTRE;
		}
	}

	switch (state) {
	case IDLE: {
		clearMovement();
	} break;
	case MOVE_RAND: { //Move to a new location depending on the current location 
		amountScanned = 0;
		int randomXLoc;
		int randomYLoc;
		while (!flag) {
			if (currentNode->row <= 8 && currentNode->column <= 11) { //if the current location is in the top left then move to the bottom right
				randomXLoc = rand() % 14 + 8;
				randomYLoc = rand() % 20 + 11;
			}
			if (currentNode->row <= 8 && currentNode->column >= 12) {//if the current location is in the top right then move to the bottom left
				randomXLoc = rand() % 14 + 8;
				randomYLoc = rand() % 11 + 2;
			}
			if (currentNode->row >= 9 && currentNode->column <= 11) {//if the current location is in the bottom left then move to the top left
				randomXLoc = rand() % 9 + 2;
				randomYLoc = rand() % 11 + 2;
			}
			if (currentNode->row >= 9 && currentNode->column >= 12) {//if the current location is in the bottom right then move to the top left
				randomXLoc = rand() % 9 + 2;
				randomYLoc = rand() % 11 + 2;
			}

			if (map->mapArray[randomXLoc][randomYLoc]->Path == true) {
				map->AStar(path, *currentNode, *map->mapArray[randomXLoc][randomYLoc]);
				flag = true;
			}

		}
	} break;
	case MOVE_AWAY: {//if the player gets too close then move the tank
		int moveTo = currentNode->row;
		while (!flag) {
			if (turretAngle <= 180) {
				moveTo = moveTo + 1;
			}
			if (turretAngle > 180) { moveTo = moveTo + 1; }
			if (map->mapArray[moveTo][currentNode->column]->Path == true) {
				map->AStar(path, *currentNode, *map->mapArray[moveTo][currentNode->column]);
				flag = true;
			}
		}
	} break;
	case MOVE_CENTRE: { //if the tank is on the end of the map then go to centre 
		int centreX = 8;
		int centreY = 10;
		while (!flag) {
			if (map->mapArray[centreX][centreY]->Path == true) {
				map->AStar(path, *currentNode, *map->mapArray[centreX][centreY]);
				flag = true;
			}
			centreX++;
			centreY++;
		}
	} break;
	}
}

void Mariusz::collided()
{

}

void Mariusz::markTarget(Position p)
{
	baseSpotted = true;

	float deltaX = getX() - p.getX();
	float deltaY = getY() - p.getY();

	angleInDegree = atan2(deltaY, deltaX) * 180 / PI; //gets the angle of the player tank in relation to the enemy 
	turretAngle = angleInDegree + 180;
	//std::cout << "Target spotted at (" << p.getX() << ", " << p.getY() << ")\n";
}

void Mariusz::markEnemy(Position p)
{
	enemySpotted = true;
	xPos = p.getX();
	//std::cout << "Enemy spotted at (" << p.getX() << ", " << p.getY() << ")\n";

	float deltaX = getX() - p.getX();
	float deltaY = getY() - p.getY();

	angleInDegree = atan2(deltaY, deltaX) * 180 / PI; //gets the angle of the player tank in relation to the enemy 
	turretAngle = angleInDegree + 180;

	float dx = (float)(p.getX() - getX());
	float dy = (float)(p.getY() - getY());
	getEnemyDistance = sqrt(dx * dx + dy * dy);

}

void Mariusz::markBase(Position p)
{

	float deltaX = getX() - p.getX();
	float deltaY = getY() - p.getY();

	baseAngleInDegree = atan2(deltaY, deltaX) * 180 / PI; //gets the angle of the player tank in relation to the enemy 
	baseAngle = baseAngleInDegree + 180;

	float pairOne = (p.getX() - getX()) * (p.getX() - getX());
	float pairTwo = (p.getY() - getY()) * (p.getY() - getY());
	float distance = sqrt(pairOne + pairTwo);

	float range = 0;

	if (distance <= 50) range = 50;
	if (distance > 50 && distance <= 100) range = 25;
	if (distance > 100) range = 12.5;

	//std::cout << "Base spotted at (" << p.getX() << ", " << p.getY() << ")\n";
}

void Mariusz::markShell(Position p)
{
	//std::cout << "Shell spotted at (" << p.getX() << ", " << p.getY() << ")\n";
}

bool Mariusz::isFiring()
{
	return firing; // return true if firing 

}

void Mariusz::score(int thisScore, int enemyScore)
{
	std::cout << "MyScore: " << thisScore << "\tEnemy score: " << enemyScore << std::endl;
}
