#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

#include "Vertex.h"



class Graph
{
public:
    Graph(int size);
    void addVertexToGraph(const Vertex& vertex);
    void addEdgeToGraph(const u_int32_t vertex1, const u_int32_t vertex2, double distance);
    double distanceBetweenVertices(u_int32_t vertex1, u_int32_t vertex2) const;
    std::vector<u_int32_t> getAdjacentVectors(u_int32_t) const;
    Vertex getVertex(u_int32_t) const;
private:

    std::vector<Vertex> vertices_;
    std::unordered_map<u_int64_t, double> edges_;

};

#endif // GRAPH_H

