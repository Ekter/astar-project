#include "graph.h"

Graph::Graph(int size)
{
    vertices_.resize(size);
}

void Graph::addVertexToGraph(const Vertex &vertex)
{
    vertices_[vertex.getID()] = vertex;
}

void Graph::addEdgeToGraph(const u_int32_t vertex1_id, const u_int32_t vertex2_id, double distance)
{
    u_int64_t min_id = std::min(vertex1_id, vertex2_id);
    u_int64_t max_id = std::max(vertex1_id, vertex2_id);
    edges_[min_id << 32 + max_id] = distance;
}

double Graph::distanceBetweenVertices(const u_int32_t vertex1_id, const u_int32_t vertex2_id) const
{
    u_int64_t min_id = std::min(vertex1_id, vertex2_id);
    u_int64_t max_id = std::max(vertex1_id, vertex2_id);
    return edges_[min_id << 32 + max_id];
}

std::vector<u_int32_t> Graph::getAdjacentVectors(const u_int32_t vertex_id) const
{
    return vertices_[vertex_id].getNeighbours();
}

void astar(uint32_t vstart, uint32_t vend)
{
    container<uint32_t> active_queue;
    set<uint32_t> closed_set;
    set_all_vertex_weight_to_max_value();
    // ID of the start vertex
    active_queue.push_end(vstart);
    do
    {
        // from the current vertex in the front of the queue
        // compute all vertices reachable in 1 step
        auto vcurrent = active_queue.pop_front();
        if (vcurrent == vend)
            break;
        closed_set.add(vcurrent);
        for (vnext in adjacency_list of vcurrent)
        {
            if (vnext is in closed_set)
            {
                continue;
            }
            auto g = vcurrent.get_weight() + get_edge_w(vcurrent, vnext);
            auto f = g + heuristic_distance_estimator(vnext, vend);
            if (vnext is not already in active_queue)
            {
                vnext.set_weight(g);
                vnext.set_estimate(f);
                active_queue.push_end(vnext);
            }
            else if (f < vnext.get_estimate())
            {
                vnext.set_weight(g);
                vnext.set_estimate(f);
            }
        }
        // the partial sort ensure that the vertex with the smallest estimate
        // is the first on the active_queue
        active_queue.partial_sort_on_estimate();
    } while (active_queue.size() != 0)
}

std::vector<u_int32_t> Graph::dijkstra(u_int32_t vstart, u_int32_t vend)
{
    std::deque<u_int32_t> active_queue;
    std::set<u_int32_t> closed_set;
    std::vector<u_int32_t> traversal;

    for (u_int32_t i = 0; i < vertices_.size(); i++)
    {
        weights_[i] = std::numeric_limits<u_int32_t>::max();
    }
    // ID of the start vertex
    active_queue.push_back(vstart);
    do
    {
        // from the current vertex in the front of the queue
        // compute all vertices reachable in 1 step
        u_int32_t vcurrent = active_queue.front();
        active_queue.pop_front();
        if (vcurrent == vend)
            break;
        closed_set.insert(vcurrent);
        for (auto &&vnext : getAdjacentVectors(vcurrent))
        {
            if (closed_set.find(vnext) != closed_set.end())
            {
                continue;
            }
            auto w = weights_[vcurrent] + distanceBetweenVertices(vcurrent, vnext);
            if (std::find(active_queue.begin(), active_queue.end(), vnext)==active_queue.end())
            {
                weights_[vnext] = w;
                active_queue.push_back(vnext);
            }
            else if (w < weights_[vnext])
            {
                weights_[vnext] = w;
            }
        }
        // the partial sort ensure that the vertex with the smallest w
        // is the first on the active_queue
        std::partial_sort(active_queue.begin(), active_queue.end(), [](const Vertex &v1, const Vertex &v2) { return v1.get_weight() < v2.get_weight(); });
        active_queue.partial_sort();
    } while (active_queue.size() != 0)
    {
        u_int32_t vcurrent = active_queue.front();
        active_queue.pop_front();
        traversal.push_back(vcurrent);
    }
    return traversal;
}

std::vector<u_int32_t> Graph::bfs(u_int32_t vstart, u_int32_t vend)
{
    std::deque<u_int32_t> active_queue;
    std::set<u_int32_t> closed_set;
    std::vector<u_int32_t> traversal;

    // ID of the start vertex
    active_queue.push_back(vstart);
    do
    {
        // from the current vertex in the front of the queue
        // compute all vertices reachable in 1 step
        u_int32_t vcurrent = active_queue.front();
        active_queue.pop_front();
        closed_set.insert(vcurrent);
        for (auto &&vnext : getAdjacentVectors(vcurrent))
        {
            if (closed_set.find(vnext) != closed_set.end())
            {
                continue;
            }
            if (std::find(active_queue.begin(), active_queue.end(), vnext)==active_queue.end())
            {
                active_queue.push_back(vnext);
            }
        }
    } while (active_queue.size() != 0);
    {
        u_int32_t vcurrent = active_queue.front();
        active_queue.pop_front();
        traversal.push_back(vcurrent);
    }
    return traversal;
}
