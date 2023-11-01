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
    Vertex(int id = 0, double latitude = 0, double longitude = 0)
        : ID(id == 0 ? idCounter++ : id), // Initialize the const member variable here
          x(earthRadius * longitude * M_PI / 180.0),
          y(earthRadius * log(tan(M_PI / 4 + latitude * M_PI / 360.0)))
    { }

    Vertex &Vertex::operator=(const Vertex &other)
    {
        if (this != &other)
        {
            this->x = other.x;
            this->y = other.y;
            this->neighbours = other.neighbours;
        }
        return *this;
    }

    // Getters for x and y
    std::pair<double, double> getXY() const;
    int getID() const;

    std::vector<u_int32_t> getNeighbours() const { return neighbours; }
    void addNeighbour(int neighbourID) { neighbours.push_back(neighbourID); }

private:
    static int idCounter;
    double x;
    double y;
    const int ID;
    std::vector<u_int32_t> neighbours;
};

#endif // VERTEX_H

int Vertex::idCounter = 0;