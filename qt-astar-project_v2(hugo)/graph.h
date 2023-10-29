#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <map>
#include <utility>

#include "Vertex.h"



class Graph
{
public:
    Graph(int size);
    void addVertexToGraph(const Vertex& vertex);
    void addEdgeToGraph(const Vertex& vertex1, const Vertex& vertex2, double distance);

private:

    std::vector<uint32_t> vertices;
    std::map<pair<uint32_t, uint32_t>, double> edges;

        //std::vector<Edge> edges;
};

#endif // GRAPH_H
