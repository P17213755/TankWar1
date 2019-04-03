#pragma once
#pragma once
#include "aitank.h"
#include <iostream>

#include "Node.h"
#include "Map.h"
class Mariusz : public AITank
{
private:
	int timer = 100;
	int timeElapsed = 0;
	bool hasCollided;
	bool enemySpotted = false; //for if the enemy has been spotted
	bool baseSpotted = false; //For if our base has been spotted
	bool friendlyBaseSpotted = false;
	bool firing = false;
	bool hasLineOfSight = false;
	int turretAngle = 0;
	int baseAngle = 0;
	int angleInDegree = 0;
	int baseAngleInDegree = 0;
	bool flag = false;
	std::list<Node> path;
	int amountScanned = 0;
	std::vector<int> AIbases;
	int distance = 0;
	int xPos;
	sf::RectangleShape rect;
	Node* currentNode;
	Node* targetNode;
	float getEnemyDistance = 1000;
public:
	Mariusz();
	~Mariusz();
	int buildingsRemain = 10;
	void move();
	void reset();
	void collided();
	//Each of the state Machines used
	void turretMachine();
	void movementMachine();
	void AimingMachine();
	void navigationMachine();
	void markTarget(Position p);
	void markEnemy(Position p);
	void markBase(Position p);
	void markShell(Position p);
	bool isFiring();
	void score(int thisScore, int enemyScore);
	void DrawNode(sf::RenderTarget &target);
	void setCurrentNode();
	sf::RectangleShape drawRange()  const { return rect; }
	Node* node;
	Map* map;
};

