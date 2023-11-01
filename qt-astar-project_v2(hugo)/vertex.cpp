#include "vertex.h"
#include <QGraphicsEllipseItem>
#include <QBrush>

Vertex::Vertex(double longitude, double latitude, double ID, QWidget *parent) :
    QWidget(parent),
    ID_{ID}
{
    ID_= ID;
    Geo2pixel(longitude, latitude, x,y);
}

 void Vertex::Geo2pixel(double longitude, double latitude, double& x, double& y){


    if (latitude < -90 || latitude > 90) {
        throw std::range_error("Latitude out of range (valid range: ±90°)");
    }

    if (longitude < -180 || longitude > 180) {
        throw std::range_error("Longitude out of range (valid range: ±180°)");
    }

    //Mercator projection

    // Convert latitude and longitude to radians
    double phi = latitude * M_PI / 180.0;
    double lambda = longitude * M_PI / 180.0;

    // Calculate x and y using Mercator projection formulas
    x = earthRadius * lambda;
    y = earthRadius * log(tan(M_PI / 4 + phi / 2));

     //test projection 2

    //double mapWidth    = 800;
    //double mapHeight   = 600;

    // get x value
    //x = (longitude+180)*(mapWidth/360);

        // convert from degrees to radians
        //double latRad = latitude*M_PI/180;

    // get y value
    //double mercN = log(tan((M_PI/4)+(latRad/2)));
    //y     = (mapHeight/2)-(mapWidth*mercN/(2*M_PI));

}






Vertex::Vertex(const Vertex& vertex) : x(vertex.x),y(vertex.y), ID_(vertex.ID_)
{
    qDebug() << "Vertex Copied" ;
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
    return ID_;
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

void Vertex::add2connectedVertices(Vertex* vertex){

    connectedVertices.emplace_back(vertex);
}

