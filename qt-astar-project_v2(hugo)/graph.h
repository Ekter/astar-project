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

#include "Vertex.h"

class Graph : public QGraphicsScene
{
public:
    Graph();
    void addVertexToGraph(const Vertex& vertex);
    std::vector<Vertex> vertices;

public slots:


private:

    Vertex vertex_test;
    //std::vector<Edge> edges;
};

#endif // GRAPH_H
