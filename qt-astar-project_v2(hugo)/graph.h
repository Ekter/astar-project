#ifndef GRAPH_H
#define GRAPH_H

#include <QWidget>
#include <QPainter>
#include <QMainWindow>
#include <QSize>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QLabel>
#include <iostream>
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QGraphicsProxyWidget>

#include "vertex.h"

class Graph
{
public:
    Graph();
    void addVertexToGraph(const Vertex& vertex);
    std::vector<Vertex> vertices;
    std::vector<Edge> edges;

public slots:


private:

    Vertex vertex_test;

};

#endif // GRAPH_H
