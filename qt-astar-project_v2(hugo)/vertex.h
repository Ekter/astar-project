#ifndef VERTEX_H
#define VERTEX_H

#include <QWidget>
#include <QPainter>
#include <cmath>
#include <QPolygon>
#include <QGraphicsEllipseItem>

#define earthRadius 6378000.137

struct Edge
{
    Edge() {}
    Edge(double source_vid, double dest_vid): source_vid{source_vid}, dest_vid{dest_vid}  {}
    double source_vid;
    double dest_vid;
};

class Vertex : public QWidget
{
public:
    // Default constructor
    Vertex(QWidget *parent = nullptr) : QWidget(parent), x(0), y(0), ID_{0} {}

    // Parameterized constructor
    Vertex(double xCoord, double yCoord, double ID, QWidget *parent = nullptr);

    Vertex(const Vertex& vertex);

    void Geo2pixel(double longitude, double latitude, double& x, double& y);

    // Getters for x and y
    double getX() const;
    double getY() const;
    double getID() const;

    // Setters for x and y
    void setX(double newX);
    void setY(double newY);

    //Adder
    void add2connectedVertices(Vertex* vertex);

    std::vector<Vertex*> connectedVertices;

private:
    double x;
    double y;
    double ID_;
};

#endif // VERTEX_H
