#include "graph.h"

Graph::Graph(int size)
{
    vertices_.reserve(size);
    weights_.reserve()
}

void Graph::addVertexToGraph(const Vertex &vertex)
{
    vertices_[vertex.getID()] = vertex;
}

void Graph::addEdgeToGraph(const u_int32_t vertex1_id, const u_int32_t vertex2_id, double distance)
{
    edges_[((u_int64_t)std::min(vertex1_id, vertex2_id)) << 32 + std::max(vertex1_id, vertex2_id)] = distance;
}

double Graph::distanceBetweenVertices(const u_int32_t vertex1_id, const u_int32_t vertex2_id) const
{
    return edges_[(((u_int64_t)std::min(vertex1_id, vertex2_id)) << 32) + std::max(vertex1_id, vertex2_id)];
}

std::vector<u_int32_t> Graph::getAdjacentVectors(const u_int32_t vertex_id) const
{
    return vertices_[vertex_id].getNeighbours();
}

std::vector<u_int32_t> Graph::astar(u_int32_t vstart, u_int32_t vend)
{
    std::deque<u_int32_t> active_queue;
    std::set<u_int32_t> closed_set;
    std::vector<u_int32_t> traversal;
    setWeights(std::numeric_limits<u_int32_t>::max());
    // ID of the start vertex
    active_queue.push_back(vstart);
    do
    {
        // from the current vertex in the front of the queue
        // compute all vertices reachable in 1 step
        auto vcurrent = active_queue.front();
        active_queue.pop_front();
        if (vcurrent == vend)
            break;
        closed_set.insert(vcurrent);

        for (auto &&vnext : getAdjacentVectors(vcurrent))
        {
            if (std::find(closed_set.begin(), closed_set.end(), vnext) != closed_set.end())
            {
                continue;
            }
            auto g = weights_[vcurrent] + distanceBetweenVertices(vcurrent, vnext);
            auto f = g + heuristic_distance_estimator(vnext, vend);
            if (std::find(active_queue.begin(), active_queue.end(), vnext) == active_queue.end())
            {
                weights_[vnext] = g;

                vnext.set_estimate(f);
                active_queue.push_back(vnext);
            }
            else if (f < vnext.get_estimate())
            {
                weights_[vnext] = g;
                vnext.set_estimate(f);
            }
            std::partial_sort(active_queue.begin(), active_queue.begin() + 10, active_queue.end(), [this, vcurrent](const u_int32_t v1_id, const u_int32_t v2_id)
                              { return distanceBetweenVertices(vcurrent, v1_id) < distanceBetweenVertices(vcurrent, v2_id); });
        }
        // the partial sort ensure that the vertex with the smallest estimate
        // is the first on the active_queue
        active_queue.partial_sort_on_estimate();
    } while (active_queue.size() != 0);
    {
        auto vcurrent = active_queue.front();
        active_queue.pop_front();
        traversal.push_back(vcurrent);
    }
    return traversal;
}

std::vector<u_int32_t> Graph::dijkstra(u_int32_t vstart, u_int32_t vend)
{
    std::deque<u_int32_t> active_queue;
    std::set<u_int32_t> closed_set;
    std::vector<u_int32_t> traversal;

    setWeights(std::numeric_limits<u_int32_t>::max());
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
            if (std::find(active_queue.begin(), active_queue.end(), vnext) == active_queue.end())
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
        std::partial_sort(active_queue.begin(), active_queue.begin() + 10, active_queue.end(), [this, vcurrent](const u_int32_t v1_id, const u_int32_t v2_id)
                          { return distanceBetweenVertices(vcurrent, v1_id) < distanceBetweenVertices(vcurrent, v2_id); });
    } while (active_queue.size() != 0);
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
            if (std::find(active_queue.begin(), active_queue.end(), vnext) == active_queue.end())
            {
                active_queue.push_back(vnext);
            }
        }
    } while (!active_queue.empty());
    {
        u_int32_t vcurrent = active_queue.front();
        active_queue.pop_front();
        traversal.push_back(vcurrent);
    }
    return traversal;
}
