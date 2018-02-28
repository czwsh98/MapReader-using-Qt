/*
* efficiency_lines.cpp
* Contains implementations for the efficiency line interface
*
* Written by: Ziwei Chen
*         on: 8-10-2017
*         Jiaotong University All Rights Reserved
*/

#include "efficiency_lines.h"
#include "ui_efficiency_lines.h"
#include <QtMath>

efficiency_lines::efficiency_lines(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::efficiency_lines)
{
    ui -> setupUi(this);
    curr_index = 0;
    connect(ui -> curve_approx, SIGNAL(mouse_pressed(QPoint)), this, SLOT(add_point(QPoint)));
    connect(ui -> curve_approx, SIGNAL(dropadd_point(int, QPoint)), this, SLOT(drop_add(int, QPoint)));
}

efficiency_lines::~efficiency_lines()
{
    delete ui;
}

//Add point when clicked to QGraphicsView
void efficiency_lines::add_point(QPoint point){
    QPointF point_append(point);
    set_text(point_storage.size());
    point_storage.append(point_append);
    if (point_storage.size() % 4 == 0){
        draw();
        ui -> text_instructions -> setText(tr("Voilà!"));
    }
}

/*
* Number represents the index of the point in the point storage to modify
* When the drop action is implemented
*/
void efficiency_lines::drop_add(int number, QPoint point){
    clear_path();
    point_storage[number] = point;
    draw();
}

//Set instruction text given the number of points in point storage
void efficiency_lines::set_text(int number){
    if (number % 4 == 1)
        ui -> text_instructions -> setText(tr("Click to set Control Point 1"));
    else if (number % 4 == 2)
        ui -> text_instructions -> setText(tr("Click to set Control Point 2"));
}

//Draw Bezier curve for all points in point storage
void efficiency_lines::draw(){
    QLinearGradient myGradient;
    QPen myPen(Qt::red);
    myPen.setWidth(3);
    clear_path();
    for (int i = 0; i < point_storage.size() / 4; i ++){
        QPainterPath myPath;
        myPath.moveTo(point_storage[i * 4]);
        myPath.cubicTo(point_storage[i * 4 + 1], point_storage[i * 4 + 2], point_storage[i * 4 + 3]);
        QGraphicsPathItem* path_item = new QGraphicsPathItem;
        path_item -> setPath(myPath);
        path_item -> setBrush(myGradient);
        path_item -> setPen(myPen);
        //Set opacity to make it less visible for better clarity
        path_item -> setOpacity(0.4);
        temp_path.append(path_item);
        ui -> curve_approx -> drawPath(path_item);
    }
}

//Given the bezier curve path drawn, find a set of points that can represent the curve
void efficiency_lines::calculate_points(){
    QVector<QPointF> points_on_curve;
    for (int num = 0; num < point_storage.size() / 4; num ++){
        //A set of 50 points representing by t from 0 to 1 with a step of 0.02
        double t = 0;
        double step = 0.02;
        /*
        *Detailed formula can be found on wikipedia:
        * https://en.wikipedia.org/wiki/B%C3%A9zier_curve, section about the Cubic Bezier curve
        */
        while (t <= 1){
            QPointF to_append;
            double p0_x = point_storage[num * 4].rx();
            double p0_y = point_storage[num * 4].ry();
            double p1_x = point_storage[num * 4 + 1].rx();
            double p1_y = point_storage[num * 4 + 1].ry();
            double p2_x = point_storage[num *4 + 2].rx();
            double p2_y = point_storage[num *4 + 2].ry();
            double p3_x = point_storage[num *4 + 3].rx();
            double p3_y = point_storage[num *4 + 3].ry();
            to_append.setX(qPow((1.0 - t), 3)*p0_x+3*qPow((1.0 - t), 2)*t*p1_x + 3*(1.0 - t)*qPow(t,2)*p2_x + qPow(t,3)*p3_x);
            to_append.setY(qPow((1.0 - t), 3)*p0_y+3*qPow((1.0 - t), 2)*t*p1_y + 3*(1.0 - t)*qPow(t,2)*p2_y + qPow(t,3)*p3_y);
            points_on_curve.append(to_append);
            t = t + step;
        }
    }
    //Store the vector of points to the 2D vector
    path_point_storage.append(points_on_curve);
}

//Switch between two modes: drag and set up points
void efficiency_lines::on_start_drag_clicked()
{
    if (ui -> start_drag -> text() == tr("Drag")){
        ui -> start_drag -> setText(tr("Set up Points"));
        ui -> curve_approx -> set_drag(true);
    }
    else{
        ui -> start_drag -> setText(tr("Drag"));
        ui -> curve_approx -> set_drag(false);
    }
}

//Store path and points information to vectors and go to the next page
void efficiency_lines::on_confirm_button_clicked()
{
    if (info_storage.size() != path_storage.size()){
        QMessageBox msgBox;
        msgBox.setText("Info Mismatched. Please Try Again! All information has been cleared.");
        info_storage.clear();
        ui -> info_text -> setText(tr("Current Hold = 0"));
        msgBox.exec();
        return;
    }
    ui -> curve_approx -> clear();
    this -> hide();
    a -> initialize_eff(path_point_storage, path_storage, info_storage);
    a -> show();
}

//Initialize current page and the next page as well
void efficiency_lines::initialize_image(QImage store){
    ui -> curve_approx -> set_image(store);
    a = new read_xy();
    a -> initialize(store);
}

void efficiency_lines::initialize_next(int maxx, int minx, int maxy, int miny){
    a -> store_value(maxx, minx, maxy, miny);
}

//Clear all paths in QGraphicsScene
void efficiency_lines::clear_path(){
    for (int i = 0; i < temp_path.size(); i++)
        ui -> curve_approx -> remove_path(temp_path[i]);
    temp_path.clear();
}

//After clicking the button, store information about the current line into the vector
void efficiency_lines::on_next_line_button_clicked()
{
    if ((ui -> line_info -> text().isEmpty())){
        QMessageBox msgBox;
        msgBox.setText("Please Enter Info for the Curve You Have Drawn (In your drawing order)");
        msgBox.exec();
        return;
    }
    else{
        info_storage.append(ui -> line_info -> text().toDouble());
        ui -> line_info -> clear();
        ui -> info_text -> setText(tr("Current Hold = %1").arg(info_storage.size()));
        //After making sure that we are going to the next line,
        //Store path and points info to the storage
        path_storage.append(temp_path);
        calculate_points();
        temp_path.clear();
        point_storage.clear();
        //Clear all points displayed (for greater clarity)
        ui -> curve_approx -> clear();
    }
}

//Store information from previous page to the next
void efficiency_lines::store_prev_info(QVector<QVector<QPointF> > prev_points, QVector<QGraphicsPathItem *> prev_path,
                                       QVector<int> prev_info){
    a -> initialize_rot(prev_points, prev_path, prev_info);
}

//Clear all points displayed in the scene and in the storage
void efficiency_lines::on_delete_button_clicked()
{
    ui -> curve_approx -> clear();
    clear_path();
    point_storage.clear();
}

//Clear the info storage
void efficiency_lines::on_clear_store_clicked()
{
    info_storage.clear();
    ui -> info_text -> setText(tr("Current Hold = %1").arg(info_storage.size()));
}
