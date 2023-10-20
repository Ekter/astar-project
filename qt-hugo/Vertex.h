#ifndef VERTEX_H
#define VERTEX_H

#include <QWidget>
#include <QPainter>

class Vertex : public QWidget
{
public:
    // Default constructor
    Vertex(QWidget *parent = nullptr) : QWidget(parent), x(0), y(0), ID(0) {}

    // Parameterized constructor
    Vertex(double xCoord, double yCoord, double ID=0, QWidget *parent = nullptr);

    // Copy constructor
    Vertex(const Vertex& other) : x(other.x), y(other.y), ID(other.ID) {}

    // Getters for x and y
    double getX() const;
    double getY() const;
    double getID() const;

    // Setters for x and y
    void setX(double newX);
    void setY(double newY);
    void setID(double newID);


private:
    double x;
    double y;
    double ID;
};

#endif // VERTEX_H
