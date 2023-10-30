#ifndef VERTEX_H
#define VERTEX_H

#include <utility>
#include <cmath>
#include <vector>
#define earthRadius 6371000
#define M_PI 3.14159265358979323846
class Vertex
{
public:
    // Default constructor
    Vertex(int id=0,double latitude=0, double longitude=0) : ID(id) {

    double latRad  = latitude  * M_PI / 180.0;
    double longRad = longitude * M_PI / 180.0;

    x=earthRadius*longRad;
    y=earthRadius*log(tan(M_PI/4 + latRad/2));

    }

    // Copy constructor
    //Vertex(const Vertex& other) : x(other.x), y(other.y), ID(other.ID) {}

    // Getters for x and y
    std::pair<double, double> getXY() const;
    int getID() const;

    // Setters for x and y
    void setXY(double newX, double newY);
    void getNeighbours();

private:
    double x;
    double y;
    const int ID;
    std::vector<u_int32_t> neighbours;
};

#endif // VERTEX_H
