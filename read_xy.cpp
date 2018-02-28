/*
* read_xy.cpp
* Contains the implementation file for an interface that finds intersections,
* display intersections, display as a table, and export as CSV format.
* Serves as the main output interface
*
* Written by: Ziwei Chen
*         on: 8-10-2017
*         Jiaotong University All Rights Reserved
*/

#include "read_xy.h"
#include "ui_read_xy.h"
#include <QtGlobal>
#include <QtMath>

read_xy::read_xy(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::read_xy)
{
    ui->setupUi(this);
    ui -> points_table -> setRowCount(max_row);
    ui -> points_table -> setColumnCount(4);
    QStringList header;
    header << tr("Speed") << tr("Mass Flow Rate") << tr("Pressure Ratio") << tr("Efficiency");
    ui -> points_table -> setHorizontalHeaderLabels(header);
    connect(ui -> image_view, SIGNAL(mouse_pressed(QPoint)), this, SLOT(display_position(QPoint)));
}

//Store x,y values (Efficiency and Rotation won't be stored)
//into the table when click on the image
void read_xy::display_position(QPoint point){
    QMessageBox msgBox;
    if (tracking){
        int pos_x = point.x();
        int pos_y = point.y();
        if ((pos_y > y_max) or (pos_y < y_min)){
            msgBox.setText("Point out of bound! Can't recognize the coordinates.");
            msgBox.exec();
            return;
        }
        if ((pos_x > x_max) or (pos_x < x_min)){
            msgBox.setText("Point out of bound! Can't recognize the coordinates.");
            msgBox.exec();
            return;
        }
        double real_x, real_y;
        find_xy(real_x, real_y, pos_x, pos_y);
        add_to_table(real_x, real_y, 0, 0);
    }
    else{
        msgBox.setText("Haven't filled the coordinates!");
        msgBox.exec();
        ui -> image_view -> clear();
        return;
    }
}

//This function stores value(given as parameters) into the table
void read_xy::add_to_table(double real_x, double real_y, int rot, double eff){
    QString text_a = QString("%1").arg(rot);
    QString text_b = QString("%1").arg(real_x, 0, 'f', 4);
    QString text_c = QString("%1").arg(real_y, 0, 'f', 4);
    QString text_d = QString("%1").arg(eff, 0, 'f', 4);
    //Set the row to given values
    QTableWidgetItem *item1 = new QTableWidgetItem(text_a);
    ui -> points_table -> setItem(row, 0, item1);
    QTableWidgetItem *item2 = new QTableWidgetItem(text_b);
    ui -> points_table -> setItem(row, 1, item2);
    QTableWidgetItem* item3 = new QTableWidgetItem(text_c);
    ui -> points_table -> setItem(row, 2, item3);
    QTableWidgetItem* item4 = new QTableWidgetItem(text_d);
    ui -> points_table -> setItem(row, 3, item4);
    row ++;
    //Expands the table if necessary
    if (row > max_row - 1){
        max_row = max_row * 2;
        ui -> points_table -> setRowCount(max_row);
    }
}

void read_xy::find_xy(double &real_x, double & real_y, int pos_x, int pos_y){
    real_x = 0;
    real_y = 0;
    //find width and height of within the coordinate system
    int width_x = x_max - x_min;
    int height_y = y_max - y_min;
    //Calculate the x and y value at given position
    real_x = (double) (pos_x - x_min) / (double) width_x * (double) (END_X - START_X) + (double) START_X;
    real_y = (double) (height_y - pos_y + y_min) / (double) height_y * (double) (END_Y - START_Y) + (double)START_Y;
}

//Store the boundary of the X,Y axis
void read_xy::store_value(int maxx, int minx, int maxy, int miny){
    x_max = maxx;
    x_min = minx;
    y_max = maxy;
    y_min = miny;
}

void read_xy::draw_contour(){
    QPen pen(Qt::black);
    //Draw the rectangle for the coordinates
    ui -> image_view -> scene -> addLine(x_min, y_min, x_max, y_min, pen);
    ui -> image_view -> scene -> addLine(x_min, y_min, x_min, y_max, pen);
    ui -> image_view -> scene -> addLine(x_min, y_max, x_max, y_max, pen);
    ui -> image_view -> scene -> addLine(x_max, y_min, x_max, y_max, pen);
}

//Display numbers on x, y axis
void read_xy::display_number(){
    double number_x = START_X;
    double number_y = START_Y;
    double pos_x = x_min;
    double pos_y = y_max;
    //We expect 5 intermediate values between the starting x and ending x values(Same for y)
    for (int i = 0; i < 7; i ++){
        //Initialize a QGraphicsTextItem to display at certain position
        QGraphicsTextItem* item = new QGraphicsTextItem;
        item -> setPos(pos_x, pos_y);
        item -> setPlainText(QString::number(number_x, 'f', 2));
        pos_x = pos_x + (x_max-x_min) / 6;
        number_x = number_x + (END_X - START_X) / 6;
        ui -> image_view -> scene -> addItem(item);
    }

    //Repeat the same process for y
    pos_x = x_min;
    pos_y = y_max;
    for (int i = 0; i < 7; i ++){
        QGraphicsTextItem* item = new QGraphicsTextItem;
        item->setPos(pos_x - 30, pos_y);
        item->setPlainText(QString::number(number_y, 'f', 2));
        pos_y = pos_y - (y_max-y_min) / 6;
        number_y = number_y + (END_Y - START_Y) / 6;
        ui -> image_view -> scene -> addItem(item);
    }

    display_curve_num();
}

void read_xy::display_curve_num(){
    //Display information about all rotation curves
    for (int i = 0; i < rot_info.size(); i ++){
        QGraphicsTextItem* item = new QGraphicsTextItem;
        double x = rot_point[i][49].x();
        double y = rot_point[i][49].y();
        item->setPos(x + 10, y);
        item->setPlainText(QString::number(rot_info[i]));
        ui -> image_view -> scene -> addItem(item);
    }

    //Display information about all efficiency curves
    for (int i = 0; i < eff_info.size(); i ++){
        QGraphicsTextItem* item = new QGraphicsTextItem;
        double x = eff_point[i][49].x();
        double y = eff_point[i][49].y();
        item->setPos(x, y + 10);
        item->setPlainText(QString::number(eff_info[i]));
        ui -> image_view -> scene -> addItem(item);
    }
}

void read_xy::initialize(QImage store){
    image = store;
    ui -> image_view -> set_image(image);
}

//Draw rotation curve paths to the scene
void read_xy::draw_rot_path(){
    for (int i = 0; i < rot_path.size(); i ++)
        ui -> image_view -> drawPath(rot_path[i]);
}

//Draw efficiency curve paths to the scene
void read_xy::draw_eff_path(){
    for (int i = 0; i < eff_path.size(); i++){
        QVector<QGraphicsPathItem*> temp_path = eff_path[i];
        for (int j = 0; j < temp_path.size(); j ++)
            ui -> image_view -> drawPath(temp_path[j]);
    }
}

//Draw all QGraphicsPathItem to the scene
void read_xy::draw_paths(){
    draw_rot_path();
    draw_eff_path();
}

read_xy::~read_xy()
{
    delete ui;
}

/*
* After validating the input x,y axis value,
* Start finding intersection and drawing coordinates
*/
void read_xy::on_confirm_clicked()
{
    QMessageBox msgBox;
    START_X = ui -> x_min -> text().toDouble();
    if (START_X < 0.0){
        msgBox.setText("Invalid X_MIN!");
        msgBox.exec();
        return;
    }
    END_X = ui -> x_max -> text().toDouble();
    if (END_X < START_X){
        msgBox.setText("Invalid X_MAX!");
        msgBox.exec();
        return;
    }
    START_Y = ui -> y_min -> text().toDouble();
    if (START_Y < 0.0){
        msgBox.setText("Invalid Y_MIN!");
        msgBox.exec();
        return;
    }
    END_Y = ui -> y_max -> text().toDouble();
    if (END_Y < START_Y){
        msgBox.setText("Invalid Y_MAX!");
        msgBox.exec();
        return;
    }
    ui -> image_view -> set_bound(image);
    msgBox.setText("You may now click on the graph to track coordinates!");
    msgBox.exec();
    //Draw the boundary of the coordinate system
    draw_contour();
    //Draw curve and efficiency lines on the scene
    draw_paths();
    tracking = true;
    //Display number and information on the corresponding curves
    display_number();
    //Find intersections of curves
    find_intersections();
}

//Delete current row and the point displayed when click the button
void read_xy::on_delete_button_clicked()
{
    ui -> points_table -> removeRow(ui -> points_table -> currentRow());
    ui -> image_view -> delete_number(ui -> points_table -> currentRow());
}

//Delete all rows and points
void read_xy::on_clear_clicked()
{
    ui -> image_view -> clear();
    ui -> points_table -> setRowCount(0);
    ui -> points_table -> setRowCount(10);
    row = 0;
    max_row = 10;
}

//Initialize the rotation information and paths to the storage
void read_xy::initialize_rot(QVector<QVector<QPointF> > prev_points, QVector<QGraphicsPathItem *> prev_path,
                             QVector<int> prev_info){
    for (int i = 0; i < prev_points.size(); i ++){
        QVector<QPointF> temporary;
        QVector<QPointF> temp_points = prev_points[i];
        for (int j = 0; j < temp_points.size(); j++)
            temporary.append(temp_points[j]);
        rot_point.append(temporary);
    }

    for (int i = 0; i < prev_path.size(); i++)
        rot_path.append(prev_path[i]);

    for (int i = 0; i < prev_info.size(); i ++)
        rot_info.append(prev_info[i]);
}

//Initialize the efficiency information and paths to the storage
void read_xy::initialize_eff(QVector <QVector<QPointF>> prev_points,
                             QVector < QVector<QGraphicsPathItem*> > prev_path,
                             QVector <double> prev_info){
    for (int i = 0; i < prev_points.size(); i ++){
        //Create a temporary 1D vector to help store into 2D vector
        QVector<QPointF> temporary;
        QVector<QPointF> temp_points = prev_points[i];
        for (int j = 0; j < temp_points.size(); j++)
            temporary.append(temp_points[j]);
        eff_point.append(temporary);
    }

    for (int i = 0; i < prev_path.size(); i++){
        QVector<QGraphicsPathItem*> temporary;
        QVector<QGraphicsPathItem*> temp_path = prev_path[i];
        for (int j = 0; j < temp_path.size(); j++)
            temporary.append(temp_path[j]);
        eff_path.append(temporary);
    }

    for (int i = 0; i < prev_info.size(); i ++)
        eff_info.append(prev_info[i]);
}

//Find intersections in rotation and efficiency lines
void read_xy::find_intersections(){
    //For each efficiency line, find points which the rotation curve also contains (within certain systematic errors)
    for (int i = 0; i < eff_point.size(); i ++){
        QVector <QPointF> temp_points = eff_point[i];
        double min, max;
        //For each point on current rotation line, find points that match the point in efficiency line
        for (int k = 0; k < rot_point.size(); k ++){
            QVector <QPointF> rot_temp = rot_point[k];
            //Find the y range of rotation line, for quicker searching
            find_min_max(rot_temp, min, max);
            for (int j = 0; j < temp_points.size(); j ++){
                /*
                * Only if efficiency line is within the y range of the rotation line,
                * Possible intersection points exist
                */
                if ((temp_points[j].y() >= min) && (temp_points[j].y() <= max))
                    if (find_intersect_in(temp_points[j], rot_temp)){
                        //Store the intersection information into a customized info_point class
                        info_point item(rot_info[k],eff_info[i],
                                        temp_points[j].x(),temp_points[j].y());
                        //Find if current intersection information has been recorded already
                        int index = find_repeat(item);
                        //If recorded, average the existing information with the current one
                        //(Improve accuracy)
                        if (index != -1)
                            modify_item_at(index, item);
                        //Else we can safely append to the intersection storage
                        else
                            intersection_storage.append(item);
                    }
            }
        }
    }
    //Rearrange items in the ascending order of rotation speed
    merge_sort(intersection_storage);
    //Draw all intersection points in the storage
    draw_intersection();
}

//Display intersection points to the table and draw an ellipse on the image
void read_xy::draw_intersection(){
    for (int i = 0; i < intersection_storage.size(); i ++){
        double real_x, real_y;
        //Given the x y point in QGraphicsScene, map the coordinates to the actual coordinate system
        find_xy(real_x, real_y, intersection_storage[i].return_pos_x(),
                intersection_storage[i].return_pos_y());
        add_to_table(real_x, real_y, intersection_storage[i].return_rot(),
                     intersection_storage[i].return_eff());
        ui -> image_view -> add_point(QPointF(intersection_storage[i].return_pos_x(),
                                              intersection_storage[i].return_pos_y()));
    }
}
/*
* Intersection storage holds certain number of repeated intersection points
* This function finds the repetitive points
*/
int read_xy::find_repeat(info_point item){
    for (int i = 0; i < intersection_storage.size(); i++){
        if (find_diff(item.return_pos_x(), intersection_storage[i].return_pos_x(), 7))
            if (find_diff(item.return_pos_y(), intersection_storage[i].return_pos_y(), 7))
                return i;
    }
    return -1;
}

/*
* When finding repetitive points, this function averages all
* repetitive values
*/
void read_xy::modify_item_at(int index, info_point item){
    double x = intersection_storage[index].return_pos_x();
    double y = intersection_storage[index].return_pos_y();
    //Average the intersection point for great accuracy
    intersection_storage[index].set_pos_x( (x + item.return_pos_x()) / 2);
    intersection_storage[index].set_pos_y( (y + item.return_pos_y()) / 2);
}

/*
* Find the range of the rotation curve(for the sake of faster
* searching of finding intersection)
*/
void read_xy::find_min_max(QVector<QPointF> vector, double &min, double &max){
    min = 999999;
    max = 0;
    for (int i = 0; i < vector.size(); i++){
        if (vector[i].y() > max)
            max = vector[i].y();
        if (vector[i].y() < min)
            min = vector[i].y();
    }
}

bool read_xy::find_intersect_in(QPointF point, QVector<QPointF> to_find){
    int start = 0;
    int end = to_find.size() - 1;
    /*
    * Binary search finds points in to_find vector that matches the given point in terms of
    * x coordinate
    */
    int result = binary_search(point, start, end, to_find);
    if (result != -1){
        /*
        * Error in Y coordinates is expected to be within 4.5
        * If X and Y coordinates all match, then return true for found
        */
        if(find_diff(point.y(), to_find[result].y(), 4.5))
            return true;
        else
            return false;
    }
    else
        return false;
}

/*
* Perform binary search to find if the given point exists in the to_find vector
* Notice that the point to find in the vector may not be exact, but within certain error.
*/
int read_xy::binary_search(QPointF point, int start, int end, QVector<QPointF> to_find){
    if (start > end)
        return -1;
    else{
        int middle = (start + end) / 2;
        //Error in X coordinates is expected to be within 2.5. If so, then found!
        if (find_diff(point.x(), to_find[middle].x(), 2.5)){
            return middle;
        }
        else if (point.x() > to_find[middle].x())
            return binary_search(point, middle + 1, end, to_find);
        else
            return binary_search(point, start, middle - 1, to_find);
    }
}

//Find if the number 1 and number 2 is within certain error called "diff"
bool read_xy::find_diff(double num1, double num2, double diff){
    if (qAbs(num1 - num2) <= diff)
        return true;
    else
        return false;
}

//Export data points to csv format
void read_xy::on_export_button_clicked()
{
    //T and P value each represents temperature and pressure; to be reduced later
    double t = QInputDialog::getDouble(this, tr("Enter the temperature value "),
                                        tr("Temperature(K): "), 0, -2147483647, 2147483647, 3);
    double p = QInputDialog::getDouble(this, tr("Enter the pressure value"),
                                        tr("Pressure(Pa): "), 0, -2147483647, 2147483647, 3);
    QString csvName= QFileDialog::getSaveFileName(this, tr("Export to csv"), "untitled.csv", tr("(*.csv)"));
    QFile record(csvName);
    if(!csvName.isEmpty()) {
        ui -> points_table -> setRowCount(row);
        record.open(QFile::WriteOnly);
        QTextStream out(&record);
        out << "\"Reduced Speed\",\"Reduced Mass Flow Rate\",\"Pressure Ratio\",\"Efficiency\"\n";
        for(int i = 0; i < ui -> points_table -> rowCount(); i++) {
            //Reduce the speed using the formula: speed/sqrt(t)
            QString text1 = QString::number(ui -> points_table -> item(i, 0)->data(0).toDouble()/qSqrt(t), 'f', 4);
            //Reduce the mass flow using the formula: flow*1000*sqrt(t)/p
            QString text2 = QString::number(ui -> points_table -> item(i, 1)->data(0).toDouble()*qSqrt(t)*1000/p, 'f', 4);
            QString text3 = QString::number(ui -> points_table -> item(i, 2)->data(0).toDouble(), 'f', 4);
            QString text4 = QString::number(ui -> points_table -> item(i, 3)->data(0).toDouble());
            out << "\"" << text1 << "\",\"" << text2 << "\",\"" << text3 << "\",\""
                << text4 <<  "\"\n";
        }
   }
}
