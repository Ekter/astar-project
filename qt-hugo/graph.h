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
#include "edge.h"

class Graph : public QWidget
{
public:
    Graph(QWidget *parent = nullptr);
    void addVertex(const Vertex &vertex);

protected:
    void paintEvent(QPaintEvent *event) override;


public slots:
    void setupGridView(void);
    void updateGrid(void);
    void loadMap(void);
    void zoomIn(void);
    void zoomOut(void);
    void resetZoom(void);
    void displayVertex(const Vertex& vertex);
    void pushButtonCreation(void);
    void astar(uint32_t vstart, uint32_t vend);
    void dijkstra(uint32_t vstart, uint32_t vend);
    void bfs(uint32_t vstart);


private:
    QGraphicsView *view;
    QGraphicsScene *scene;
    int gridSize;
    std::vector<Vertex> vertices;
    std::vector<Edge> edges;
    qreal zoomInFactor = 1.1;
    qreal zoomOutFactor = 0.9;
    QPainter painter;
};

#endif // GRAPH_H
