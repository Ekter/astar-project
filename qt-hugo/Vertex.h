#ifndef VERTEX_H
#define VERTEX_H

#include <pair>
#include <QWidget>
#include <QPainter>
#define earthRadius 6371000
#define M_PI 3.14159265358979323846
class Vertex : public QWidget
{
public:
    // Default constructor
    Vertex(int id=0,double latitude=0, double longitude=0, QWidget *parent = nullptr) : QWidget(parent), ID(id) {

    double latRad  = latitude  * M_PI / 180.0;
    double longRad = longitude * M_PI / 180.0;

    x=earthRadius*longRad;
    y=earthRadius*log(tan(M_PI/4 + latRad/2));

    }

    // Copy constructor
    Vertex(const Vertex& other) : x(other.x), y(other.y), ID(other.ID) {}

    // Getters for x and y
    pair<double, double> getXY() const;
    int getID() const;

    // Setters for x and y
    void setXY(double newX, double newY);

private:
    double x;
    double y;
    const int ID;
};

#endif // VERTEX_H
