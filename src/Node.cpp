#include "Node.h"



Node::Node(int xPosIn, int yPosIn, int heightIn, int widthIn, int r, int c)
{
	//set the local variables to the parameters 
	xPos = xPosIn;
	yPos = yPosIn;
	row = r;
	column = c;
	height = heightIn;
	width = widthIn;
	current = false;
	//set the size, position and color of the nodes
	rect.setSize(sf::Vector2f(width, height));
	rect.setPosition(sf::Vector2f(xPos, yPos));
	rect.setFillColor(sf::Color(255, 255, 255, 50));
	rect.setOutlineThickness(1);
	rect.setOutlineColor(sf::Color(255, 255, 255, 100));

}

//Sets the current nodes color 
void Node::setCurrent(bool b) {
	if (!current && b) {
		rect.setFillColor(sf::Color(255, 0, 255, 150));
		current = true;
	}
	else if (current && !b) {
		rect.setFillColor(sf::Color(255, 255, 255, 50));
		current = false;
	}
}

void Node::DrawNode(sf::RenderTarget &target) {
	target.draw(rect);
}

int Node::getX() {
	return xPos + width / 2;
}

int Node::getY() {
	return yPos + height / 2;
}

void Node::setWall() {
	Path == false;
	rect.setFillColor(sf::Color(0, 0, 0, 100));
}

void Node::setPath() {
	Path == true;
	rect.setFillColor(sf::Color(255, 255, 255, 50));
}

void Node::setGoal() {
	rect.setFillColor(sf::Color(255, 0, 0, 100));
}

void Node::setStart() {
	rect.setFillColor(sf::Color(0, 255, 0, 100));
}

void Node::setOpen() {
	rect.setFillColor(sf::Color(255, 255, 255, 100));
}

void Node::setClosed() {
	rect.setFillColor(sf::Color(255, 255, 0, 100));
}

int Node::lowerOutcome(const Node other) const {
	return f < other.f;
}


Node::~Node()
{
}
