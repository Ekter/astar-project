// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef VIEW_H
#define VIEW_H

#include <QFrame>
#include <QGraphicsView>
#include "graph.h"
#include "qnamespace.h"
#include <QBrush>
#include <QColor>
#include <QtWidgets>
#include <QtMath>
#include <QGraphicsTextItem>
#include <QGraphicsEllipseItem>
#include <QPainter>
#include <QApplication>
#include <QProgressDialog>
#include <QtConcurrent/QtConcurrentRun>
#include <QLabel>
#include <QThread>
#include <QTimer>

#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QPrinter>
#include <QPrintDialog>
#endif

#endif



QT_BEGIN_NAMESPACE
class QLabel;
class QSlider;
class QToolButton;
QT_END_NAMESPACE

// Map boundaries defined when loading a specific set of coordinates
struct MapBoundaries {
    double xMin;
    double xMax;
    double yMin;
    double yMax;
};

// Worker thread to perform the time-consuming task
class Worker : public QThread {
    Q_OBJECT

public:
    Worker(QObject* parent = nullptr) : QThread(parent) {}

signals:
    void progressUpdated(int value);

};



//Class that represent the vertex on my map
// ------------------------------------------------------------------------------------------------------------------------
class MyEllipseItem : public QGraphicsEllipseItem {
public:
    QGraphicsScene* scene_;
    MyEllipseItem(Vertex& vertex, qreal size, QGraphicsScene* scene) : vertex_{&vertex} ,QGraphicsEllipseItem(vertex.getX()-size/2, vertex.getY()-size/2, size, size) {
        setAcceptHoverEvents(true);
        SelectModeState = false;
        scene_ = scene;
        defaultBrush = QBrush(Qt::blue);
        hoverBrush = QBrush(Qt::red);
        startEndBrush = QBrush(Qt::green);
        pen = QPen(Qt::blue);
        pen.setWidthF(0.1);
        pointSize = 0.5;

        this->setBrush(MyEllipseItem::defaultBrush);
        this->setPen(MyEllipseItem::pen);
        this->setAcceptHoverEvents(true);

    }

    Vertex* vertex_;

    static bool SelectModeState;
    static QBrush defaultBrush;
    static QBrush hoverBrush;
    static QBrush startEndBrush;
    static QPen pen;
    static qreal pointSize;

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;

    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

private:

};

// ------------------------------------------------------------------------------------------------------------------------

class View;

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView(View *v) : QGraphicsView(), view(v) { }

protected:
#if QT_CONFIG(wheelevent)
    void wheelEvent(QWheelEvent *) override;
    void mousePressEvent(QMouseEvent *event) override;
#endif

private:
    View *view;
};

class View : public QFrame
{
    Q_OBJECT
public:
    explicit View(const QString &name, QWidget *parent = nullptr);

    QGraphicsView *view() const;
    void drawMap();
    void loadMap();
    Vertex* getVertexByID(const double& ID);
    void addFlagsStartEnd(void);




public slots:
    void onLoadMapAction(void);
    void onAddRandomStartEnd(void);
    void onRemoveStartEnd(void);
    void zoomIn(void);
    void zoomOut(void);
    void zoomInBy(int level);
    void zoomOutBy(int level);
    void zoomOutAction(void);
    void zoomInAction(void);


private slots:
    void resetView();
    void setResetButtonEnabled();
    void setupMatrix();
    void togglePointerMode();
    void print();

private:

    GraphicsView *graphicsView;
    QLabel *label;
    QToolButton *selectModeButton;
    QToolButton *dragModeButton;
    QToolButton *printButton;
    QToolButton *resetButton;
    QSlider *zoomSlider;
    Graph *graph = nullptr;
    QGraphicsScene scene;
    MapBoundaries mapBoundaries;
    bool mapLoaded = false;

    //Start and End point
    QGraphicsPolygonItem* triangleItemEnd;
    QGraphicsEllipseItem* circleItemEnd;
    QGraphicsEllipseItem* circleItem1End;

    QGraphicsPolygonItem* triangleItemStart;
    QGraphicsEllipseItem* circleItemStart;
    QGraphicsEllipseItem* circleItem1Start;

};

#endif // VIEW_H
