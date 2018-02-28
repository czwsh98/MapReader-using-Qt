/*
* mainwindow.h
* Contains the header file for the main interface that reads the image,
* and identifies the coordinate system.
*
* Written by: Ziwei Chen
*         on: 8-10-2017
*         Jiaotong University All Rights Reserved
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "read_xy.h"
#include "curve_lines.h"
#include <QMainWindow>
#include <QFileDialog>
#include <QFile>
#include <QPixmap>
#include <QImage>
#include <QTextStream>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void set_points(QPoint point);

private slots:
    void on_actionOpen_triggered();
    void on_next_page_clicked();
    void on_reset_button_clicked();
    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;
    QVector<QPoint> storage;
    curve_lines *a;
    QImage display_image;
    int angle;
    int x_min, x_max, y_min, y_max;
    void initialize_next();
};

#endif // MAINWINDOW_H
