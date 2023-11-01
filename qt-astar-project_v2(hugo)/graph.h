#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <deque>
#include <set>

#include "vertex.h"



class Graph
{
public:
    Graph(int size);
    void addVertexToGraph(const Vertex& vertex);
    //std::vector<Vertex> vertices;
    std::vector<Edge> edges;

    void addEdgeToGraph(const u_int32_t vertex1, const u_int32_t vertex2, double distance);
    double distanceBetweenVertices(u_int32_t vertex1, u_int32_t vertex2) const;
    std::vector<u_int32_t> getAdjacentVectors(u_int32_t) const;
    Vertex getVertex(u_int32_t) const;
    std::vector<u_int32_t> bfs(u_int32_t vstart_id, u_int32_t vend_id);
    std::vector<u_int32_t> dijkstra(u_int32_t vstart_id, u_int32_t vend_id);
private:

    std::vector<Vertex> vertices_;
    std::unordered_map<u_int64_t, double> edges_;
    std::vector<u_int32_t> weights_;

};

#endif // GRAPH_H

