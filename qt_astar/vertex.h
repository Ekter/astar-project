#ifndef VERTEX_H
#define VERTEX_H

#include <cmath>
#include <cstdint>
#include <vector>
#include <QDebug>
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
    uint32_t idCounter = 0;
    Vertex(uint32_t id = -1, double latitude = 0, double longitude = 0)
        : ID_(id == -1 ? idCounter++ : id), // Initialize the const member variable here
          x_(earthRadius * longitude * M_PI / 180.0),
          y_(earthRadius * log(tan(M_PI / 4 + latitude * M_PI / 360.0))),
          initialized{false}

    {
    }

    Vertex &operator=(const Vertex &other)
    {
        this->initialized = true;
        this->x_ = other.x_;
        this->y_ = other.y_;
        this->neighbours = other.neighbours;
        return *this;
    }

    Vertex(const Vertex &vertex) : x_(vertex.x_), y_(vertex.y_), ID_(vertex.ID_){};

    // Getters for x and y
    double getX() const
    {
        return x_;
    }

    double getY() const
    {
        return y_;
    }

    // Setters for x and y
    void setX(double x)
    {
        x_ =x;
    }

    void setY(double y)
    {
        y_=y;
    }
    uint32_t getID() const { return ID_; };

    std::vector<uint32_t> getNeighbours() const { return neighbours; }
    void addNeighbour(uint32_t neighbourID) { neighbours.push_back(neighbourID); }
    bool initialized;
private:

    double x_;
    double y_;
    const uint32_t ID_;
    std::vector<uint32_t> neighbours;

};

#endif // VERTEX_H

