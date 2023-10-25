#include "vertex.h"
#include <QGraphicsEllipseItem>
#include <QBrush>

Vertex::Vertex(double longitude, double latitude, double ID,QGraphicsEllipseItem *vertex_display, QWidget *parent) :
    QWidget(parent),
    x(earthRadius*longitude * M_PI / 180.0),
    y(earthRadius*log(tan(M_PI/4 + (latitude  * M_PI / 180.0)/2))),
    ID(ID)
{
    // Initialize the default properties
    qreal pointSize = 5; // Default size of the point
    QBrush brush(Qt::blue); // Default fill color

    // Create the default QGraphicsEllipseItem
    vertex_display = new QGraphicsEllipseItem(0, 0, pointSize, pointSize);
    vertex_display->setBrush(brush);
    vertex_display->setPos(x, y);
}






Vertex::Vertex(const Vertex& vertex) : x(vertex.x),y(vertex.y)
{
    qDebug() << "Copied" ;
}

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

