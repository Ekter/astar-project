#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <deque>
#include <set>

#include "Vertex.h"

class Graph
{
public:
    Graph(int size);
    void addVertexToGraph(const Vertex &vertex);
    void addEdgeToGraph(const u_int32_t vertex1, const u_int32_t vertex2, double distance);
    double distanceBetweenVertices(u_int32_t vertex1, u_int32_t vertex2) const;
    std::vector<u_int32_t> getAdjacentVectors(u_int32_t) const;
    Vertex getVertex(u_int32_t) const;
    std::vector<u_int32_t> bfs(u_int32_t vstart_id, u_int32_t vend_id);
    std::vector<u_int32_t> dijkstra(u_int32_t vstart_id, u_int32_t vend_id);
    std::vector<u_int32_t> astar(u_int32_t vstart_id, u_int32_t vend_id);

private:
    std::vector<Vertex> vertices_;
    std::unordered_map<u_int64_t, double> edges_;
    std::vector<u_int32_t> weights_;
    std::vector<u_int32_t> estimates;
    void setWeights(u_int32_t new_weight)
    {
        for (auto i = 0; i < weights_.size(); i++)
        {
            weights_[i] = new_weight;
        }
    };
    double heuristic_distance_estimator(u_int32_t v1_id, u_int32_t v2_id) const
    {
        std::pair<double, double> c1 = vertices_[v1_id].getXY();
        std::pair<double, double> c2 = vertices_[v2_id].getXY();
        return sqrt(pow(c1.first - c2.first, 2) + pow(c1.second - c2.second, 2));
    };
};

#endif // GRAPH_H

