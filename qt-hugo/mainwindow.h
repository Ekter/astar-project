#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include "graph.h"
#include <iostream>
#include <QFileDialog>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Solve_clicked();

    void on_AddGrid_clicked();

    void on_LoadMap_clicked();

    void on_toolButton_pressed();

    void on_zoomIn_clicked();

    void on_zoomOut_clicked();

private:
    Ui::MainWindow *ui;
    Graph *graph;
};
#endif // MAINWINDOW_H
