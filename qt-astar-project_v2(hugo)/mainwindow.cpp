// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "mainwindow.h"
#include "view.h"

#include <QHBoxLayout>
#include <QSplitter>
#include <QPushButton>
#include "QGraphicsRectItem"

#include <QMenuBar>
#include <QMenu>
#include <QAction>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , splitter(new QSplitter(this)), graphview(new View("Map View"))

{

    //Add graph view
    splitter->addWidget(graphview);

    //Add sideBar and Menubar
    addSideBar();
    createMenuBar();

    //Set the size of the mainWindow
    setCentralWidget(splitter);
    setMinimumSize(1000, 600);
    setWindowTitle(tr("QT - PathFinding Project"));
}

void MainWindow::addSideBar(){
    // Create a side widget for push buttons
    QWidget *sideWidget = new QWidget;
    QVBoxLayout *sideLayout = new QVBoxLayout;

    QPushButton *randomStartEndButton = new QPushButton("Random Start/End");
    // Connect the button click signal to a slot
    connect(randomStartEndButton, &QPushButton::clicked, this->graphview, &View::onAddRandomStartEnd);

    QPushButton *clearStartEndButton = new QPushButton("Clear Start/End");
    // Connect the button click signal to a slot
    connect(clearStartEndButton, &QPushButton::clicked, this->graphview, &View::onRemoveStartEnd);



    sideLayout->addWidget(randomStartEndButton);
    sideLayout->addWidget(clearStartEndButton);
    sideWidget->setLayout(sideLayout);

    int sideWidgetWidth = width() / 4;
    sideWidget->setFixedWidth(sideWidgetWidth);

    // Add the side widget to the splitter
    splitter->addWidget(sideWidget);
}

void MainWindow::createMenuBar()
{
    // Create the File menu
    QMenu *fileMenu = menuBar()->addMenu(tr("File"));

    QAction *loadMapAction = new QAction(tr("Load Map"), this);
    connect(loadMapAction, &QAction::triggered, this->graphview, &View::onLoadMapAction);
    fileMenu->addAction(loadMapAction);

    QAction *exitAction = new QAction(tr("Exit"), this);
    connect(exitAction, &QAction::triggered, this, &MainWindow::onExitAction);
    fileMenu->addAction(exitAction);

    // Create the View menu
    QMenu *viewMenu = menuBar()->addMenu(tr("View"));

    QAction *zoomInAction = new QAction(tr("Zoom In"), this);
    connect(zoomInAction, &QAction::triggered, this->graphview, &View::zoomInAction);
    viewMenu->addAction(zoomInAction);

    QAction *zoomOutAction = new QAction(tr("Zoom Out"), this);
    connect(zoomOutAction, &QAction::triggered, this->graphview, &View::zoomOutAction);
    viewMenu->addAction(zoomOutAction);

}

void MainWindow::onExitAction()
{
    this->close();
}
