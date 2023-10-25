#ifndef VERTEX_H
#define VERTEX_H

#include <QWidget>
#include <QPainter>
#include <cmath>
#include <QPolygon>
#include <QGraphicsEllipseItem>

#define earthRadius 6371000

class Vertex : public QWidget
{
public:
    // Default constructor
    Vertex(QWidget *parent = nullptr) : QWidget(parent), x(0), y(0), ID(0) {}

    // Parameterized constructor
    Vertex(double xCoord, double yCoord, double ID, QGraphicsEllipseItem *vertex_display, QWidget *parent = nullptr);

    Vertex(const Vertex& vertex);

    // Getters for x and y
    double getX() const;
    double getY() const;
    double getID() const;

    // Setters for x and y
    void setX(double newX);
    void setY(double newY);

    QGraphicsEllipseItem* vertex_display;

private:
    double x;
    double y;
    double ID;




};

#endif // VERTEX_H
