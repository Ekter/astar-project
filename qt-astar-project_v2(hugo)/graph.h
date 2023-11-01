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

    void addEdgeToGraph(const uint32_t vertex1, const uint32_t vertex2, double distance);
    double distanceBetweenVertices(uint32_t vertex1, uint32_t vertex2);
    std::vector<uint32_t> getAdjacentVectors(uint32_t vertex_id) const {return vertices_[vertex_id].getNeighbours();};
    Vertex getVertex(uint32_t v_id) const {return vertices_[v_id];};
    void setEstimate(uint32_t v_id, double estimate) {estimates_[v_id] = estimate;};
    double getEstimate(uint32_t v_id) {return estimates_[v_id];};
    std::vector<uint32_t> bfs(uint32_t vstart_id, uint32_t vend_id);
    std::vector<uint32_t> dijkstra(uint32_t vstart_id, uint32_t vend_id);
    std::vector<uint32_t> astar(uint32_t vstart_id, uint32_t vend_id);

private:
    std::vector<Vertex> vertices_;
    std::unordered_map<uint64_t, double> edges_;
    std::vector<double> weights_;
    std::vector<double> estimates_;
    void setWeights(double new_weight)
    {
        for (auto i = 0; i < weights_.size(); i++)
        {
            weights_[i] = new_weight;
        }
    };
    void setEstimates(double new_estimate)
    {
        for (auto i = 0; i < estimates_.size(); i++)
        {
            estimates_[i] = new_estimate;
        }
    };
    double heuristic_distance_estimator(uint32_t v1_id, uint32_t v2_id) const
    {
        return sqrt(pow(vertices_[v1_id].getX() - vertices_[v2_id].getX(), 2) + pow(vertices_[v1_id].getY() - vertices_[v2_id].getY(), 2));
    };
    
};

#endif // GRAPH_H

