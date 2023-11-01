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
    std::vector<Edge> edges;

    void addEdgeToGraph(const u_int32_t vertex1, const u_int32_t vertex2, double distance);
    double distanceBetweenVertices(u_int32_t vertex1, u_int32_t vertex2) const;
    std::vector<u_int32_t> getAdjacentVectors(u_int32_t vertex_id) const {return vertices_[vertex_id].getNeighbours();};
    Vertex& getVertex(u_int32_t v_id) {return vertices_[v_id];};
    const Vertex& getVertex(u_int32_t v_id) const {return vertices_[v_id];};
    void setEstimate(u_int32_t v_id, double estimate) {estimates_[v_id] = estimate;};
    double getEstimate(u_int32_t v_id) {return estimates_[v_id];};
    std::vector<u_int32_t> bfs(u_int32_t vstart_id, u_int32_t vend_id);
    std::vector<u_int32_t> dijkstra(u_int32_t vstart_id, u_int32_t vend_id);
    std::vector<u_int32_t> astar(u_int32_t vstart_id, u_int32_t vend_id);
    const std::vector<Vertex> getVertices() const {return vertices_;};
    int getSize() const {return size_;};

private:
    int size_;
    std::vector<Vertex> vertices_;
    std::unordered_map<u_int64_t, double> edges_;
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
    double heuristic_distance_estimator(u_int32_t v1_id, u_int32_t v2_id) const
    {
        return sqrt(pow(vertices_[v1_id].getX() - vertices_[v2_id].getX(), 2) + pow(vertices[v1_id].getY() - vertices_[v2_id].getY(), 2));
    };
    
};

#endif // GRAPH_H

