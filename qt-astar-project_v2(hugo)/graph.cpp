#include "graph.h"

Graph:: Graph(int size)
{
    vertices.resize(size);

}

void Graph::addVertexToGraph(const Vertex& vertex){
    vertices[vertex.getID()]=vertex;
}



