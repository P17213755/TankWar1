#pragma once
#include "Node.h"
#include <iostream>
#include <vector>
#include <list>
class Map
{
private:
	Node * node; //Reference to the node class
	static const short unsigned rows = 20;
	static const short unsigned Columns =26;
	int width = 35; //width of the Nodes
	int height = 35; //height of the nodes
	static const short unsigned Width = 780; //width of the map
	static const short unsigned Height = 570; //height of the map
	int xBuffer = 15; //buffer added to grid so it starts in the right position 
	int yBuffer = 10;

public:
	Node * mapArray[20][26]; //2D Array used for drawing Nodes
	Map();
	void DrawMap(sf::RenderTarget &target); //Draws the SFML shapes
	void notPath(int xPos, int yPos); //Function that sets the bases so they cant be traversed
	void setPath(int xPos, int yPos); //Sets them back once theyre destroyed
	static const unsigned nNodes = rows * Columns; // Number of points on the map
	bool Map::AStar(std::list<Node>& path, Node start, Node goal); //Handles pathfinding
	std::vector<Node*> getNeighbours(Node* node); //Gets the neighbour of the current node
	std::list<Node> Map::construct_path(std::list<Node>& path, std::list<Node>& closed, Node* node); //returns the path 
	~Map();
};

