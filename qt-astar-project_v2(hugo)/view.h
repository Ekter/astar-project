// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef VIEW_H
#define VIEW_H

#include <QFrame>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include "graph.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QSlider;
class QToolButton;
QT_END_NAMESPACE

class View;

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView(View *v) : QGraphicsView(), view(v) { }

protected:
#if QT_CONFIG(wheelevent)
    void wheelEvent(QWheelEvent *) override;
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



public slots:
    void onLoadMapAction(void);
    void onAddRandomStartEnd(void);
    void onAddGridView(void);
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
    QLabel *label2;
    QToolButton *selectModeButton;
    QToolButton *dragModeButton;
    QToolButton *addGridButton;
    QToolButton *printButton;
    QToolButton *resetButton;
    QSlider *zoomSlider;
    Graph *graph = nullptr;
    QGraphicsScene scene;
};

#endif // VIEW_H
