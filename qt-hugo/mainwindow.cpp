#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    graph = ui->graph;


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_Solve_clicked()
{

}


void MainWindow::on_AddGrid_clicked() {
    if (graph) {
        graph->updateGrid();
    }
}


void MainWindow::on_LoadMap_clicked()
{
    if (graph) {
        graph->loadMap();
    }
}


void MainWindow::on_toolButton_pressed()
{

}


void MainWindow::on_zoomIn_clicked()
{
    graph->zoomIn();
}


void MainWindow::on_zoomOut_clicked()
{
    graph->zoomOut();
}

