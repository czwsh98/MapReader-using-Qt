/*
* mainwindow.cpp
* Contains the implementation for the main interface that reads the image,
* and identifies the coordinate system.
*
* Written by: Ziwei Chen
*         on: 8-10-2017
*         Jiaotong University All Rights Reserved
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui -> setupUi(this);
    connect(ui -> image_view, SIGNAL(mouse_pressed(QPoint)), this, SLOT(set_points(QPoint)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Open file when the button is triggered
void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                            tr("Open Image"), "/Users/Simon", tr("Image Files (*.png *.jpg *.bmp)"));
    if(fileName == NULL)
        return;
    QImage image(fileName);
    if ((image.height() >= 395) && (image.width() >= 454))
        display_image = image.scaled(454, 395, Qt::KeepAspectRatio);
    else
        display_image = image;
    ui -> image_view -> set_image(display_image);
}

//Go to the next page and hide the current page
void MainWindow::on_next_page_clicked()
{
    if (storage.size() < 2){
        QMessageBox msgBox;
        msgBox.setText(tr("Haven't set Points!"));
        msgBox.exec();
        return;
    }
    this -> hide();
    a = new curve_lines();
    initialize_next();
    a -> show();
}

//Initialize the next page
void MainWindow::initialize_next(){
    a -> initialize_image(display_image);
    a -> initialize_next(x_max, x_min, y_max, y_min);
}

/*
* Set the top left point and the bottom right point
* to obtain the boundary of the coordinate system
*/
void MainWindow::set_points(QPoint point){
    if(storage.size() == 2){
        QMessageBox msgBox;
        msgBox.setText(tr("Already have two points!"));
        msgBox.exec();
        return;
    }
    else{
        storage.append(point);
        if (storage.size() == 2){
            x_max = storage[1].x();
            x_min = storage[0].x();
            y_max = storage[1].y();
            y_min = storage[0].y();
            ui -> tl_label -> setText(QString("Top Left: X is %1 Y is %2").arg(QString::number(x_min),
                                                                               QString::number(y_min)));
            ui -> br_label -> setText(QString("Bottom Right: X is %1 Y is %2").arg(QString::number(x_max),
                                                                                   QString::number(y_max)));
        }
    }
}

//Reset and clear all points on the QGraphicsView
void MainWindow::on_reset_button_clicked()
{
    ui -> image_view -> clear();
    storage.clear();
    ui -> tl_label -> setText(QString("Top Left: "));
    ui -> br_label -> setText(QString("Bottom Right: "));
}

//About menu
void MainWindow::on_actionAbout_triggered()
{
    QMessageBox msgBox;
    msgBox.setText(tr("Developed By Ziwei Chen in 2017."));
    msgBox.setInformativeText(tr("Jiaotong Universirt All Rights Reserved."));
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
}
