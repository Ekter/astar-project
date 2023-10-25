// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "view.h"

#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QPrinter>
#include <QPrintDialog>
#endif
#endif
#include <QtWidgets>
#include <QtMath>
#include <QGraphicsTextItem>

#if QT_CONFIG(wheelevent)

//Enable with Ctrl+Scrool to ZoomIn & ZoomOut
void GraphicsView::wheelEvent(QWheelEvent *e)
{
    if (e->modifiers() & Qt::ControlModifier) {
        if (e->angleDelta().y() > 0)
            view->zoomInBy(6);
        else
            view->zoomOutBy(6);
        e->accept();
    } else {
        QGraphicsView::wheelEvent(e);
    }
}
#endif

View::View(const QString &name, QWidget *parent)
    : QFrame(parent), scene(QGraphicsScene(this))
{

    //setFrameStyle(Box | StyledPanel);
    graphicsView = new GraphicsView(this);
    graphicsView->setRenderHint(QPainter::Antialiasing, false);
    graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
    graphicsView->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    graphicsView->setScene(&scene);

    int size = style()->pixelMetric(QStyle::PM_ToolBarIconSize);
    QSize iconSize(size, size);

    zoomSlider = new QSlider;
    zoomSlider->setMinimum(200);
    zoomSlider->setMaximum(500);
    zoomSlider->setValue(250);
    zoomSlider->setTickPosition(QSlider::TicksRight);

    // Label layout
    QHBoxLayout *labelLayout = new QHBoxLayout;
    label = new QLabel(name);

    resetButton = new QToolButton;
    resetButton->setText(tr("Reset View"));
    resetButton->setEnabled(false);

    selectModeButton = new QToolButton;
    selectModeButton->setText(tr("Select Mode"));
    selectModeButton->setCheckable(true);
    selectModeButton->setChecked(false);

    dragModeButton = new QToolButton;
    dragModeButton->setText(tr("Drag Mode"));
    dragModeButton->setCheckable(true);
    dragModeButton->setChecked(true);

    addGridButton = new QToolButton;
    addGridButton->setText(tr("Grid Activated"));
    addGridButton->setCheckable(true);
    addGridButton->setChecked(false);


    printButton = new QToolButton;
    printButton->setIcon(QIcon(QPixmap(":/fileprint.png")));

    QButtonGroup *pointerModeGroup = new QButtonGroup(this);
    pointerModeGroup->setExclusive(true);
    pointerModeGroup->addButton(selectModeButton);
    pointerModeGroup->addButton(dragModeButton);
    pointerModeGroup->addButton(addGridButton);

    labelLayout->addWidget(label);
    labelLayout->addStretch();
    labelLayout->addWidget(resetButton);
    labelLayout->addWidget(selectModeButton);
    labelLayout->addWidget(dragModeButton);
    labelLayout->addWidget(addGridButton);
    labelLayout->addStretch();
    labelLayout->addWidget(printButton);

    QGridLayout *topLayout = new QGridLayout;
    topLayout->addLayout(labelLayout, 0, 0);
    topLayout->addWidget(graphicsView, 1, 0);
    setLayout(topLayout);

    connect(resetButton, &QAbstractButton::clicked, this, &View::resetView);
    connect(zoomSlider, &QAbstractSlider::valueChanged, this, &View::setupMatrix);
    connect(graphicsView->verticalScrollBar(), &QAbstractSlider::valueChanged,
            this, &View::setResetButtonEnabled);
    connect(graphicsView->horizontalScrollBar(), &QAbstractSlider::valueChanged,
            this, &View::setResetButtonEnabled);
    connect(selectModeButton, &QAbstractButton::toggled, this, &View::togglePointerMode);
    connect(dragModeButton, &QAbstractButton::toggled, this, &View::togglePointerMode);
     connect(addGridButton, &QAbstractButton::toggled, this, &View::onAddGridView);
    connect(printButton, &QAbstractButton::clicked, this, &View::print);

    graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);

    setupMatrix();
    drawMap();
}

QGraphicsView *View::view() const
{
    return static_cast<QGraphicsView *>(graphicsView);
}

void View::resetView()
{
    zoomSlider->setValue(250);
    setupMatrix();
    graphicsView->ensureVisible(QRectF(0, 0, 0, 0));

    resetButton->setEnabled(false);
}

void View::onAddGridView()
{
    // Set background color to white
    graphicsView->setBackgroundBrush(Qt::white);

    graphicsView->setRenderHint(QPainter::Antialiasing);
    graphicsView->setRenderHint(QPainter::SmoothPixmapTransform);
    graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    for (int x = 0; x <= this->size().width(); x += 5)
    {
        scene.addLine(x, 0, x, this->size().height(), QPen(Qt::gray));
    }

    for (int y = 0; y <= this->size().height(); y += 5)
    {
        scene.addLine(0, y, this->size().width(), y, QPen(Qt::gray));
    }
}

void View::drawMap(){

    if(graph == nullptr){
        //Create a gray backgruond by default
        scene.setBackgroundBrush(Qt::gray);

        //Disable the button in the graphview interface
        this->setDisabled(true);

        //Indication to create a map
        QGraphicsTextItem *textItem = new QGraphicsTextItem("Load a map. Go to File and click on LoadMap");
        textItem->setPos(100, 100);
        textItem->setDefaultTextColor(QColor(128, 128, 128));
        scene.addItem(textItem);
    }

    else{
        //Draw the map
        scene.clear();
        this->setDisabled(false);
        scene.setBackgroundBrush(Qt::white);
        if (!graph->vertices.empty()){
            for (unsigned int i = 0; i < graph->vertices.size(); i++){
                sc
            }
        }



    }
}

void View::onLoadMapAction(){
    graph = new Graph();
    loadMap();
    drawMap();
}


void View::loadMap() {
    QString initialPath = QDir::currentPath();
    QString filePath = QFileDialog::getOpenFileName(this, "Open Text File", initialPath, "Text Files (*.txt)");

    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);

            double minX = std::numeric_limits<double>::max();
            double maxX = std::numeric_limits<double>::lowest();
            double minY = std::numeric_limits<double>::max();
            double maxY = std::numeric_limits<double>::lowest();

            int linesRead = 0;

            while (!in.atEnd()) {
                QString line = in.readLine();
                //# V,vertexid,longitude,latitude,x*,y* Vertex line readed format
                if (line.startsWith("V")) {
                    QStringList parts = line.split(",");
                    if (parts.size() >= 5) {
                        double longitude = parts.at(2).toDouble();
                        double latitude = parts.at(3).toDouble();
                        double ID = parts.at(1).toDouble();

                        Vertex vertex(longitude, latitude, ID, new QGraphicsEllipseItem());

                        minX = qMin(minX, vertex.getX());
                        maxX = qMax(maxX, vertex.getX());
                        minY = qMin(minY, vertex.getY());
                        maxY = qMax(maxY, vertex.getY());

                        graph->vertices.emplace_back(vertex);

                        ++linesRead;
                    }
                }
                //# E,source_vid,dest_vid,length,name,extra0,extra1 Edge line readed format
                else if (line.startsWith("E,")){
                    QStringList parts = line.split(",");
                    if (parts.size() >= 5) {
                        double source_id = parts.at(2).toDouble();
                        double dest_id = parts.at(3).toDouble();
                        double length = parts.at(4).toDouble();

                    }
                }
            }

            file.close();

        } else {
            QMessageBox::critical(this, "Error", "Failed to open the selected file.");
        }
    }
}

void View::onAddRandomStartEnd(void)
{

    QPixmap startIcon(":/Images/location_end.png");
    QPixmap endIcon(":/Images/location_end.png");

    startIcon = startIcon.scaled(QSize(32, 32), Qt::KeepAspectRatio);
    endIcon = endIcon.scaled(QSize(32, 32), Qt::KeepAspectRatio);
    QGraphicsPixmapItem* startItem = new QGraphicsPixmapItem(startIcon);
    QGraphicsPixmapItem* endItem = new QGraphicsPixmapItem(endIcon);
    startItem->setPos(1000, 10);
    endItem->setPos(200, 20);


    scene.addItem(startItem);
    scene.addItem(endItem);
}

void View::setResetButtonEnabled()
{
    resetButton->setEnabled(true);
}

void View::setupMatrix()
{
    qreal scale = qPow(qreal(2), (zoomSlider->value() - 250) / qreal(50));

    QTransform matrix;
    matrix.scale(scale, scale);

    graphicsView->setTransform(matrix);
    setResetButtonEnabled();
}

void View::togglePointerMode()
{
    graphicsView->setDragMode(selectModeButton->isChecked()
                              ? QGraphicsView::RubberBandDrag
                              : QGraphicsView::ScrollHandDrag);
    graphicsView->setInteractive(selectModeButton->isChecked());
}

void View::print()
{
#if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printdialog)
    QPrinter printer;
    QPrintDialog dialog(&printer, this);
    if (dialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        graphicsView->render(&painter);
    }
#endif
}

void View::zoomIn()
{
    zoomSlider->setValue(zoomSlider->value() + 1);
}

void View::zoomOut()
{
    zoomSlider->setValue(zoomSlider->value() - 1);
}

void View::zoomInBy(int level)
{
    zoomSlider->setValue(zoomSlider->value() + level);
}

void View::zoomOutBy(int level)
{
    zoomSlider->setValue(zoomSlider->value() - level);
}

void View::zoomInAction()
{
    zoomInBy(50);
}

void View::zoomOutAction()
{
    zoomOutBy(50);
}

