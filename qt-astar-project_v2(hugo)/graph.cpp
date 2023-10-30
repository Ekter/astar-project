#include "graph.h"

Graph::Graph(int size)
{
    vertices_.resize(size);
}

void Graph::addVertexToGraph(const Vertex& vertex) {
    vertices_[vertex.getID()]=vertex;
}

void Graph::addEdgeToGraph(const u_int32_t vertex1_id, const u_int32_t vertex2_id, double distance) {
    u_int64_t min_id = std::min(vertex1_id,vertex2_id);
    u_int64_t max_id = std::max(vertex1_id,vertex2_id);
    edges_[min_id<<32+max_id]=distance;
}

double Graph::distanceBetweenVertices(const u_int32_t vertex1_id, const u_int32_t vertex2_id) const {
    u_int64_t min_id = std::min(vertex1_id,vertex2_id);
    u_int64_t max_id = std::max(vertex1_id,vertex2_id);
    return edges_[min_id<<32+max_id];
}

std::vector<u_int32_t> Graph::getAdjacentVectors(const u_int32_t vertex_id) const {
    return vertices_[vertex_id].getAdjacentVertices();
}