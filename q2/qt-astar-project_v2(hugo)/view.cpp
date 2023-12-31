// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "view.h"

#if QT_CONFIG(wheelevent)


bool MyEllipseItem::SelectModeState = false;
QBrush MyEllipseItem::defaultBrush;
QBrush MyEllipseItem::hoverBrush;
QBrush MyEllipseItem::startEndBrush;
QPen MyEllipseItem::pen;
qreal MyEllipseItem::pointSize;

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

std::vector<MyEllipseItem*> startEndVertices;
std::vector<MyEllipseItem*> vertices_display;
QGraphicsPixmapItem* iconItemStart;
QGraphicsPixmapItem* iconItemEnd;

void MyEllipseItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event){
   if (this->SelectModeState){
       setBrush(this->hoverBrush);
   }
}

void MyEllipseItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* event) {
   setBrush(this->defaultBrush);
}

void GraphicsView::mousePressEvent(QMouseEvent *event) {
   if (event->button() == Qt::LeftButton) {
       if (MyEllipseItem::SelectModeState) {
           QPointF scenePos = mapToScene(event->pos());  // Convert the mouse click position to scene coordinates
           qDebug() << "Mouse clicked at scene position: " << scenePos;
           qDebug() << "Size of the startEndVertices array: " << startEndVertices.size();

           if (startEndVertices.size() < 2) {
               // Find the closest vertex within a certain distance
               qreal minDistance = std::numeric_limits<qreal>::max();
               MyEllipseItem* closestVertex = nullptr;

               qDebug() << "mindistance" << minDistance ;

               for (unsigned int i=0; i< vertices_display.size(); i++){

                   double x = static_cast<double>(scenePos.x());
                   double y = static_cast<double>(scenePos.y());

                   double deltaX = x - vertices_display[i]->vertex_->getX() ;
                   double deltaY = y - vertices_display[i]->vertex_->getY();

                   double distance = std::sqrt(deltaX * deltaX + deltaY * deltaY);

                   //qDebug() << "Distance2 to vertex " << vertices_display[i]->vertex_->getID() << ": " << distance;


                   if (distance < minDistance) {
                       minDistance = distance;
                       closestVertex = vertices_display[i];
                       qDebug() << "Selected closest vertex " << vertices_display[i]->vertex_->getID();
                   }
               }

               if (closestVertex != nullptr) {
                   // Select the closest vertex
                   qDebug() << "Selected final closest vertex " << closestVertex->vertex_->getID();
                   startEndVertices.emplace_back(closestVertex);
                   closestVertex->setBrush(MyEllipseItem::startEndBrush);

                   view->addFlagsStartEnd();
               }
           }

            QGraphicsView::mousePressEvent(event);

       } else {
           // Pan the view
           QGraphicsView::mousePressEvent(event);
       }
   }
}



void View::onRemoveStartEnd(void) {
   if (mapLoaded){

       for (unsigned int i=0; i< startEndVertices.size(); i++){
           qDebug() << startEndVertices[i]->vertex_->getID();

           startEndVertices[i]->setBrush(Qt::blue);
           startEndVertices[i]->update();

       }
       if (startEndVertices.size()>=1){
           scene.removeItem(triangleItemStart);
           scene.removeItem(circleItemStart);
           scene.removeItem(circleItem1Start);
       }
       if (startEndVertices.size()>=2){
           scene.removeItem(triangleItemEnd);
           scene.removeItem(circleItemEnd);
           scene.removeItem(circleItem1End);
       }

       startEndVertices.clear();
       scene.update();
    }

}




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


    printButton = new QToolButton;
    printButton->setIcon(QIcon(QPixmap(":/Images/fileprint.png")));

    QButtonGroup *pointerModeGroup = new QButtonGroup(this);
    pointerModeGroup->setExclusive(true);
    pointerModeGroup->addButton(selectModeButton);
    pointerModeGroup->addButton(dragModeButton);

    labelLayout->addWidget(label);
    labelLayout->addStretch();
    labelLayout->addWidget(resetButton);
    labelLayout->addWidget(selectModeButton);
    labelLayout->addWidget(dragModeButton);
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
    zoomSlider->setValue(230);
    setupMatrix();
    graphicsView->ensureVisible(QRectF(0, 0, 0, 0));

    resetButton->setEnabled(false);
}

void View::drawMap(){

    if(graph == nullptr){
        //Create a gray background by default
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

        QDialog dialog;
        QProgressBar progressBar(&dialog);
        QLabel label("Drawing Map. Please wait...", &dialog);

        progressBar.setRange(0, 100);
        progressBar.setValue(0);

        QVBoxLayout layout(&dialog);
        layout.addWidget(&label);
        layout.addWidget(&progressBar);


        dialog.setWindowTitle("Loading...Please Wait");
        dialog.setFixedSize(300, 100);
        dialog.setModal(true);
        dialog.show();

        //Draw the map
        scene.clear();
        this->setDisabled(false);
        scene.setBackgroundBrush(Qt::white);

        scene.addLine(0, 0, this->size().width(),0, QPen(Qt::gray));
        scene.addLine(this->size().width(), 0, this->size().width(), this->size().height(), QPen(Qt::gray));
        scene.addLine(0, 0, 0, this->size().height(), QPen(Qt::gray));
        scene.addLine(0,this->size().height() , this->size().width(), this->size().height(), QPen(Qt::gray));

        scene.addLine(-10, -10, this->size().width()+10,-10, QPen(Qt::gray));
        scene.addLine(this->size().width()+10, -10, this->size().width()+10, this->size().height()+10, QPen(Qt::gray));
        scene.addLine(-10, -10, -10, this->size().height()+10, QPen(Qt::gray));
        scene.addLine(-10,this->size().height()+10 , this->size().width()+10, this->size().height()+10, QPen(Qt::gray));

        //scene.addItem(new QGraphicsEllipseItem(20,30,pointSize,pointSize));
        qDebug() << "Vertices lenght "<< graph->vertices.size();
        qDebug() << "x min boundaries "<< mapBoundaries.xMin;
        qDebug() << "x max boundaries "<< mapBoundaries.xMax;
        qDebug() << "y min boundaries "<< mapBoundaries.yMin;
        qDebug() << "y max boundaries "<< mapBoundaries.yMax;


        if (!graph->vertices.empty()){
            for (unsigned int i = 0; i < graph->vertices.size(); i++){

               progressBar.setValue(0.01*i);

               progressBar.setValue(i*50/graph->vertices.size());
               dialog.show();
               //QCoreApplication::processEvents();

                //Convert the x and y to fit into the map view

                double x_mapped = (graph->vertices[i].getX() - mapBoundaries.xMin) / (mapBoundaries.xMax - mapBoundaries.xMin) *this->size().width();
                double y_mapped = (graph->vertices[i].getY() - mapBoundaries.yMin) / (mapBoundaries.yMax - mapBoundaries.yMin) * this->size().height();

                graph->vertices[i].setX(x_mapped);
                graph->vertices[i].setY(y_mapped);


                MyEllipseItem* vertex_display = new MyEllipseItem(graph->vertices[i],
                                                                  MyEllipseItem::pointSize,
                                                                  &scene
                    );

                // Store the pointer to the QGraphicsEllipseItem in your 'vertices_display' container
                vertices_display.emplace_back(vertex_display);

                scene.addItem(vertex_display);
            }
            for (unsigned int i = 0; i < graph->vertices.size(); i++){
                //progressBar.setValue(i*50/graph->vertices.size());
                //QCoreApplication::processEvents();

                for (unsigned int j = 0; j < graph->edges.size(); j++){
                    if (graph->vertices[i].getID()== graph->edges[j].source_vid){
                        qDebug() << graph->edges[j].dest_vid;



                        Vertex* vertex = this->getVertexByID(graph->edges[j].dest_vid);


                        graph->vertices[i].add2connectedVertices(vertex);

                        QPen linePen;
                        linePen.setColor(Qt::black);
                        linePen.setWidthF(0.1);

                        scene.addLine(graph->vertices[i].getX(), graph->vertices[i].getY(), vertex->getX(), vertex->getY(), linePen);

                        continue;

                    }
                }
            }

        }

    dialog.accept();
    }



}

Vertex* View::getVertexByID(const double& ID) {

    for (unsigned int k = 0; k < graph->vertices.size(); k++){
        if (graph->vertices[k].getID() == ID){
            return &graph->vertices[k];
        }
    }
    return nullptr;
}

void View::onLoadMapAction(){

    graph = new Graph();
    loadMap();
    setupMatrix();

    drawMap();


    resetView();
}


void View::loadMap() {

    QDialog dialog;
    QProgressBar progressBar(&dialog);
    QLabel label("Reading the File. Please wait...", &dialog);

    progressBar.setRange(0, 100);
    progressBar.setValue(0);

    QVBoxLayout layout(&dialog);
    layout.addWidget(&label);
    layout.addWidget(&progressBar);


    dialog.setWindowTitle("Loading...Please Wait");
    dialog.setFixedSize(300, 100);
    dialog.setModal(true);
    dialog.show();

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

                progressBar.setValue(0.006*linesRead);

                QString line = in.readLine();
                //# V,vertexid,longitude,latitude,x*,y* Vertex line readed format
                if (line.startsWith("V")) {
                    QStringList parts = line.split(",");
                    if (parts.size() >= 5) {
                        double longitude = parts.at(2).toDouble();
                        double latitude = parts.at(3).toDouble();
                        double ID = parts.at(1).toDouble();

                        Vertex vertex = Vertex(longitude, latitude, ID);


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
                        double source_id = parts.at(1).toDouble();
                        double dest_id = parts.at(2).toDouble();
                        double length = parts.at(4).toDouble();

                        Edge edge= Edge(source_id,dest_id);

                        graph->edges.emplace_back(edge);

                    }
                }
            }

            file.close();

            //According to the set of coordinate it get the boundaries of my point to draw the map
            mapBoundaries.xMin = minX;
            mapBoundaries.xMax = maxX;
            mapBoundaries.yMin = minY;
            mapBoundaries.yMax = maxY;


        } else {
            QMessageBox::critical(this, "Error", "Failed to open the selected file.");
        }
    }

    mapLoaded = true;
    dialog.accept();
}



void View::onAddRandomStartEnd(void)
{
    if (mapLoaded){

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> distribution(0, graph->vertices.size());
        int first_index = distribution(gen);
        int second_index = distribution(gen);
        startEndVertices.emplace_back(vertices_display[first_index]);
        startEndVertices.emplace_back(vertices_display[second_index]);

        startEndVertices[0]->setBrush(MyEllipseItem::startEndBrush);
        startEndVertices[1]->setBrush(MyEllipseItem::startEndBrush);

        vertices_display[first_index]->setBrush(MyEllipseItem::startEndBrush);
        vertices_display[second_index]->setBrush(MyEllipseItem::startEndBrush);


        qDebug() << vertices_display[first_index]->vertex_->getID();
        qDebug() << vertices_display[second_index]->vertex_->getID();
        addFlagsStartEnd();
    }


}

void View::addFlagsStartEnd(void){

    if (startEndVertices.size()>=1){

        double size = 20;

        // Create a filled triangle
        QPolygonF triangle;
        triangle << QPointF(startEndVertices[0]->vertex_->getX() -size/2, startEndVertices[0]->vertex_->getY() - size ) << QPointF(startEndVertices[0]->vertex_->getX() + size/2 , startEndVertices[0]->vertex_->getY() - size) << QPointF(startEndVertices[0]->vertex_->getX(), startEndVertices[0]->vertex_->getY());
        triangleItemStart = scene.addPolygon(triangle, QPen(Qt::NoPen), QBrush(Qt::blue));

        // Create a filled circle
        QRectF circleRect(startEndVertices[0]->vertex_->getX() -size/2,startEndVertices[0]->vertex_->getY() -3*size/2 , size, size);
        circleItemStart = scene.addEllipse(circleRect, QPen(Qt::NoPen), QBrush(Qt::blue));

        QRectF circleRect1(startEndVertices[0]->vertex_->getX() -size/4,startEndVertices[0]->vertex_->getY() -3*size/2 +size/4, size/2, size/2);
        circleItem1Start = scene.addEllipse(circleRect1, QPen(Qt::NoPen), QBrush(Qt::white));

        if (startEndVertices.size()>=2){
            // Create a filled triangle
            QPolygonF triangle;
            triangle << QPointF(startEndVertices[1]->vertex_->getX() -size/2, startEndVertices[1]->vertex_->getY() - size ) << QPointF(startEndVertices[1]->vertex_->getX() + size/2 , startEndVertices[1]->vertex_->getY() - size) << QPointF(startEndVertices[1]->vertex_->getX(), startEndVertices[1]->vertex_->getY());
            triangleItemEnd = scene.addPolygon(triangle, QPen(Qt::NoPen), QBrush(Qt::red));

            // Create a filled circle
            QRectF circleRect(startEndVertices[1]->vertex_->getX() -size/2,startEndVertices[1]->vertex_->getY() -3*size/2 , size, size);
            circleItemEnd = scene.addEllipse(circleRect, QPen(Qt::NoPen), QBrush(Qt::red));

            QRectF circleRect1(startEndVertices[1]->vertex_->getX() -size/4,startEndVertices[1]->vertex_->getY() -3*size/2 +size/4, size/2, size/2);
            circleItem1End = scene.addEllipse(circleRect1, QPen(Qt::NoPen), QBrush(Qt::white));

        }

    }

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
    bool selectMode = selectModeButton->isChecked();
    MyEllipseItem::SelectModeState = selectMode;
    graphicsView->setDragMode(selectModeButton->isChecked()
                              ? QGraphicsView::RubberBandDrag
                              : QGraphicsView::ScrollHandDrag);
    graphicsView->setInteractive(selectModeButton->isChecked());
}

void View::print() {
#if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printdialog)
    QPrinter printer;
    QPrintDialog dialog(&printer, this);
    if (dialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        if (painter.begin(&printer)) {
            graphicsView->render(&painter);
            painter.end(); // End painting on the printer
            if (printer.outputFormat() == QPrinter::PdfFormat) {
                // Ensure PDF generation was successful
                if (printer.outputFileName().isEmpty()) {
                    QMessageBox::critical(this, "Error", "PDF file name is empty.");
                } else {
                    qDebug() << "PDF file saved to: " << printer.outputFileName();
                }
            }
        } else {
            QMessageBox::critical(this, "Error", "Could not start painting on the printer.");
        }
    } else {
        QMessageBox::critical(this, "Error", "Print dialog not accepted");
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
    if (mapLoaded){
        zoomInBy(50);
    }

}

void View::zoomOutAction()
{
    if (mapLoaded){
        zoomOutBy(50);
    }

}

