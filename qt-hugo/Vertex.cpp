#include "Vertex.h"

// Parameterized constructor
Vertex::Vertex(double xCoord, double yCoord, double ID, QWidget *parent) : x(xCoord), y(yCoord), ID(ID), QWidget(parent) {}

// Getters for x and y
double Vertex::getX() const
{
    return x;
}

double Vertex::getY() const
{
    return y;
}

double Vertex::getID() const
{
    return ID;
}

// Setters for x and y
void Vertex::setX(double newX)
{
    x = newX;
}

void Vertex::setY(double newY)
{
    y = newY;
}

void Vertex::setID(double newID)
{
    ID = newID;
}

