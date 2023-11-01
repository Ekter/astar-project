#include "graph.h"
#include "utils.h"
#include <queue>

Graph::Graph(QWidget *parent) : QWidget(parent), gridSize(5)
{
}

void Graph::paintEvent(QPaintEvent* event) {

    QPainter painter(this);
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Create a gray rectangle when the map is not loaded
    QRect rect(0, 0, size().width(), size().height()); // Adjust the position and size as needed
    QBrush grayBrush(Qt::lightGray);
    painter.fillRect(rect, grayBrush);

    for (const Vertex& vertex : vertices) {
        displayVertex(vertex);
    }

    updateGrid();
}

//Vertex and Edge display method
void Graph::displayVertex(const Vertex& vertex){

    QPointF center(vertex.getX()/2, vertex.getY() / 2);
    qreal radius = 5;
    int numPoints = 100;

    QPolygonF circlePolygon;

    for (int i = 0; i < numPoints; ++i) {
        qreal angle = 2.0 * M_PI * i / numPoints;
        qreal x = center.x() + radius * qCos(angle);
        qreal y = center.y() + radius * qSin(angle);
        circlePolygon << QPointF(x, y);
    }

    QPen blackPen(Qt::black);
    blackPen.setStyle(Qt::SolidLine);
    scene->addPolygon(circlePolygon, blackPen);

}

void Graph::pushButtonCreation(void){

    double size_button= 30;
    double space = 20;

    // Create a zoom push buttons
    QPushButton* zoomIn = new QPushButton("+");
    zoomIn->setGeometry(size().width()-2*size_button-2*space,size().height()-size_button-space, size_button, size_button); // Set the button's position and size
    zoomIn->setStyleSheet("QPushButton { padding: 0px; margin: 0px; }");
    QGraphicsProxyWidget* zoomInbuttonItem = scene->addWidget(zoomIn);
    zoomInbuttonItem->setZValue(1.0);

    // Connect the zoom-in button to the zoomIn function
    connect(zoomIn, &QPushButton::clicked, this, &Graph::zoomIn);

    QPushButton* zoomOut = new QPushButton("-");
    zoomOut->setGeometry(size().width()-size_button-space, size().height()-size_button-space, size_button, size_button); // Set the button's position and size
    zoomOut->setStyleSheet("QPushButton { padding: 0px; margin: 0px; }");
    QGraphicsProxyWidget* zoomOutbuttonItem = scene->addWidget(zoomOut);
    zoomOutbuttonItem->setZValue(1.0);

    // Connect the zoom-out button to the zoomOut function
    connect(zoomOut, &QPushButton::clicked, this, &Graph::zoomOut);
}


void Graph::setupGridView(void)
{
    // Set background color to white
    view->setBackgroundBrush(Qt::white);

    view->setRenderHint(QPainter::Antialiasing);
    view->setRenderHint(QPainter::SmoothPixmapTransform);
    view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    for (int x = 0; x <= size().width(); x += gridSize)
    {
        scene->addLine(x, 0, x, size().height(), QPen(Qt::gray));
    }

    for (int y = 0; y <= size().height(); y += gridSize)
    {
        scene->addLine(0, y, size().width(), y, QPen(Qt::gray));
    }

    view->show();

    pushButtonCreation();


}

void Graph::updateGrid()
{
    // Get the current transformation matrix from the QGraphicsView
    QTransform currentTransform = view->transform();

    // Extract the scale factor
    double currentScale = currentTransform.m11(); // Assuming uniform scaling

    // Calculate the scaled grid size
    int scaledGridSize = gridSize * currentScale; // Adjust gridSize based on zoom level

    // Clear the existing grid lines
    scene->clear(); // Clear existing items including grid lines

    // Re-add new grid lines with the updated grid spacing
    for (int x = 0; x <= size().width(); x += scaledGridSize)
    {
        scene->addLine(x, 0, x, size().height(), QPen(Qt::gray));
    }

    for (int y = 0; y <= size().height(); y += scaledGridSize)
    {
        scene->addLine(0, y, size().width(), y, QPen(Qt::gray));
    }

    view->show();

    pushButtonCreation();
}


void Graph::loadMap() {
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

                        Vertex vertex();
                        latLonToXY(&vertex, longitude, latitude);

                        minX = qMin(minX, vertex.getX());
                        maxX = qMax(maxX, vertex.getX());
                        minY = qMin(minY, vertex.getY());
                        maxY = qMax(maxY, vertex.getY());

                        vertices.push_back(vertex);

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


void Graph::zoomIn()
{
    view->scale(zoomInFactor, zoomInFactor);
    QMessageBox::warning(this, "Title", "Warning message.");
}

// Handle zoom out button click
void Graph::zoomOut()
{
    view->scale(zoomOutFactor, zoomOutFactor);
}

void Graph::resetZoom()
{
    view->resetTransform();
}

void astar(uint32_t vstart, uint32_t vend) {
    container<uint32_t> active_queue;
    set<uint32_t> closed_set;
    set_all_vertex_weight_to_max_value();
    // ID of the start vertex
    active_queue.push_end(vstart);
    do {
// from the current vertex in the front of the queue
// compute all vertices reachable in 1 step
        auto vcurrent = active_queue.pop_front();
        if (vcurrent == vend) break;
            closed_set.add(vcurrent);
        for(vnext in adjacency_list of vcurrent) {
            if (vnext is in closed_set) {
                continue;
}
    auto g = vcurrent.get_weight() + get_edge_w(vcurrent, vnext);
    auto f = g + heuristic_distance_estimator(vnext, vend);
    if (vnext is not already in active_queue) {
    vnext.set_weight(g);
    vnext.set_estimate(f);
    active_queue.push_end(vnext);
    } else if (f < vnext.get_estimate()) {
        vnext.set_weight(g);
    vnext.set_estimate(f);
}
}
// the partial sort ensure that the vertex with the smallest estimate
// is the first on the active_queue
active_queue.partial_sort_on_estimate();
} while (active_queue.size() != 0)
}

Graph::dijkstra(uint32_t vstart, uint32_t vend) {
container<uint32_t> active_queue;
set<uint32_t> closed_set;
set_all_vertex_weight_to_max_value();
// ID of the start vertex
active_queue.push_end(vstart);
do {
// from the current vertex in the front of the queue
// compute all vertices reachable in 1 step
auto vcurrent = active_queue.pop_front();
if (vcurrent == vend) break;
closed_set.add(vcurrent);
for(vnext in adjacency list of vcurrent) {
if (vnext is in closed_set) {
continue;
}
auto w = vcurrent.get_weight() + get_edge_w(vcurrent, vnext);
if (vnext is not already in active_queue) {
vnext.set_weight(w);
active_queue.push_end(vnext);
} else if (w < vnext.get_weight()) {
vnext.set_weight(w);
}
}
// the partial sort ensure that the vertex with the smallest w
// is the first on the active_queue
active_queue.partial_sort();
} while (active_queue.size() != 0)
}



Graph::bfs(uint32_t vstart) {
queue<uint32_t> active_queue;
set<uint32_t> closed_set;
vector<uint32_t> traversal;

// ID of the start vertex
active_queue.push(vstart);
do {
// from the current vertex in the front of the queue
// compute all vertices reachable in 1 step
auto vcurrent = active_queue.pop();
closed_set.add(vcurrent);
for(vnext in adjacency_list of vcurrent) {
if (const bool is_in = closed_set.contains(vnext)) {
continue;
}
if (vnext is not already in active_queue) {
active_queue.push(vnext);
}
}
} while (active_queue.size() != 0){
        vcurrent = active_queue.front();
        active_queue.pop();
        traversal.push_back(vcurrent);
}
return traversal;
}
