#include "Map.h"
#include <algorithm>
#include <functional>
Map::Map() {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < Columns; j++) {
			//sets each of the node in the 2D Array
			node = new Node(xBuffer + width * j, yBuffer + height * i, width, height, i, j);
			mapArray[i][j] = node;
		}

	}

}

void Map::DrawMap(sf::RenderTarget &target) {
	for (int i = 0; i < 22; i++) {
		for (int j = 0; j < 16; j++) {
			mapArray[j][i]->DrawNode(target);
		}
	}
}

void Map::notPath(int xPos, int yPos) { //checks each of the inputted bases to see which node theyre inside and set these to be not a path
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < Columns; j++) {
			if (xPos < (mapArray[i][j]->getX() - 35 / 2) + 35 &&
				xPos + 50 > mapArray[i][j]->getX() - 35 / 2 &&
				yPos < (mapArray[i][j]->getY() - 35 / 2) + 35 &&
				50 + yPos > mapArray[i][j]->getY() - 35 / 2) {
				// collision detected!
				mapArray[i][j]->setWall();
				mapArray[i][j]->Path = false;
			}
		}

	}
}

void Map::setPath(int xPos, int yPos) {

}


//Completed A* Search
bool Map::AStar(std::list<Node>& path, Node start, Node goal) {
	std::list<Node> open; // List of open nodes to be explored
	std::list<Node> closed; // List of closed nodes which have been explored
	closed.empty(); //empty closed list
	Node currentNode = start; //set current node to start
	Node goalNode = goal;
	currentNode.parent = -1;
	currentNode.g = 0;
	currentNode.f = currentNode.g + currentNode.score(currentNode.g, currentNode.getX(), currentNode.getY(), goal.getX(), goal.getY()); //score first node
	open.push_back(currentNode);
	while (!open.empty()) { //whilst there is nodes in the open list
		std::stable_sort(open.begin(), open.end(),//sort the open list for node with smallest f cost
			std::mem_fun_ref(&Node::lowerOutcome));
		if (currentNode.row == goal.row  &&  currentNode.column == goal.column) {
			path = construct_path(path, closed, &goal);//if the current node  has reached the goal then return path
			return true;
		}

		currentNode = open.front(); // Take the node that scored best on the open list
		open.pop_front(); // Take it off the open list
		closed.push_back(currentNode); // Add that node to the closed list
		mapArray[currentNode.row][currentNode.column]->setGoal();
		std::list<Node>::iterator cIterator;
		std::list<Node>::iterator oIterator;
		for (auto &neighbour : getNeighbours(&currentNode)) { // for each of the neighbours of the current node
			bool foundClosed = false;
			for (cIterator = closed.begin(); cIterator != closed.end(); ++cIterator) { //look to see if it is on the closed list
				if (neighbour->row == cIterator->row && neighbour->column == cIterator->column) {
					foundClosed = true;
				}
			}
			if (!foundClosed) {
				neighbour->f = neighbour->g + neighbour->score(neighbour->g, neighbour->getX(), neighbour->getY(), goal.getX(), goal.getY()); //score current node
				bool foundOpen = false;
				for (oIterator = open.begin(); oIterator != open.end(); ++oIterator) { //look to see if its on the open list 
					if (neighbour->row == oIterator->row && neighbour->column == oIterator->column) {
						foundOpen = true;
					}
				}
				if (!foundOpen) {
					open.push_back(*neighbour); // put node on open list 
					mapArray[neighbour->row][neighbour->column]->setClosed();
				}
				else {
					Node* openNeighbour = neighbour;
					if (neighbour->g < openNeighbour->g) { //compare g cost 
						openNeighbour->g = neighbour->g;
						openNeighbour->parent = neighbour->parent;
					}
				}
			}
		}
	}
	return false;
}

std::vector<Node*> Map::getNeighbours(Node* node) {
	int r = node->row;
	int c = node->column;
	float diagonal_cost = 1.414;
	int normal_cost = 1;

	//Row, Column, isDiagonal
	int ne[8][3] = { // all the possible directions for the node
		{ r, c + 1, 0 },
	{ r, c - 1, 0 },
	{ r + 1, c, 0 },
	{ r - 1, c, 0 },
	{ r + 1, c + 1, 1 },
	{ r + 1, c - 1, 1 },
	{ r - 1, c - 1, 1 },
	{ r - 1, c + 1, 1 },
	};

	std::vector<Node*> neighbours;
	std::vector<bool> diagonals;
	for (int j = 0; j < 8; j++) { //stops the node being out of bounds
		if (ne[j][0] < 16 && ne[j][1] < 22) {
			if (mapArray[ne[j][0]][ne[j][1]]->Path == true) {
				neighbours.push_back(mapArray[ne[j][0]][ne[j][1]]);
				diagonals.push_back(ne[j][2]);
			}

		}
	}

	for (int i = 0; i < neighbours.size(); i++) { //assign cost depending on whether it is diagonal or straight
		if (diagonals[i]) { //Diagonal
			neighbours[i]->g = node->g + diagonal_cost; // eg. 1.414 (pythagoras)
		}
		else {
			neighbours[i]->g = node->g + normal_cost; // eg. 1
			neighbours[i]->parent = node->parent;
		}
	}
	return neighbours;
}




std::list<Node> Map::construct_path(std::list<Node>& path, std::list<Node>& closed, Node* node) {

	int parent;
	std::list<Node>::iterator graphListIter;

	Node* currentNode = node; // Set the current node to goal node
	parent = currentNode->parent; // Set parent to be the parent of the current node
	path.push_front(*currentNode); // Add the current node the the start of the path
	closed.pop_back(); // Remove the curretn node from the closed list
					   //Now work backwards throught the closed list reconstructing the path
	for (graphListIter = closed.end(), graphListIter--; graphListIter != closed.begin(); --graphListIter)
	{
		// Add it to the path
		path.push_front(*graphListIter);
		// Remove this node from the closed list
		closed.erase(graphListIter);
		// Start working backward throguht the cloased list again from the end
		graphListIter = closed.end();
	}
	return path;
}


Map::~Map()
{
}


