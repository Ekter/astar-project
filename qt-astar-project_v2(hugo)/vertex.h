#ifndef VERTEX_H
#define VERTEX_H

#include <cmath>
#include <cstdint>
#include <vector>
#define earthRadius 6378000.137
#define M_PI 3.14159265358979323846

struct Edge
{
    Edge() {}
    Edge(double source_vid, double dest_vid) : source_vid{source_vid}, dest_vid{dest_vid} {}
    double source_vid;
    double dest_vid;
};

class Vertex
{
public:
    int idCounter = 0;
    Vertex(int id = 0, double latitude = 0, double longitude = 0)
        : ID_(id == 0 ? idCounter++ : id), // Initialize the const member variable here
          x(earthRadius * longitude * M_PI / 180.0),
          y(earthRadius * log(tan(M_PI / 4 + latitude * M_PI / 360.0)))
    {
    }

    Vertex &operator=(const Vertex &other)
    {
        if (this != &other)
        {
            this->x = other.x;
            this->y = other.y;
            this->neighbours = other.neighbours;
        }
        return *this;
    }

    Vertex(const Vertex &vertex) : x(vertex.x), y(vertex.y), ID_(vertex.ID_){};

    // Getters for x and y
    double getX() const
    {
        return x;
    }

    double getY() const
    {
        return y;
    }
    uint32_t getID() const { return ID_; };

    std::vector<uint32_t> getNeighbours() const { return neighbours; }
    void addNeighbour(uint32_t neighbourID) { neighbours.push_back(neighbourID); }

private:

    double x;
    double y;
    const int ID_;
    std::vector<uint32_t> neighbours;
};
//int Vertex::idCounter = 0;

#endif // VERTEX_H
