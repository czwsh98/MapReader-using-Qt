/*
* read_xy.h
* Contains the header file for an interface that finds intersections,
* display intersections, display as a table, and export as CSV format.
* Serves as the main output interface
*
* Written by: Ziwei Chen
*         on: 8-10-2017
*         Jiaotong University All Rights Reserved
*/

#ifndef READ_XY_H
#define READ_XY_H

#include <QMessageBox>
#include <QWidget>
#include <QTextStream>
#include <QFileDialog>
#include <QGraphicsPathItem>
#include <QImage>
#include <QFile>
#include <QInputDialog>
#include <QString>
#include <QTableWidget>
#include <QTableWidgetItem>
#include "info_point.h"
#include "mergesort.h"

namespace Ui {
class read_xy;
}

class read_xy : public QWidget
{
    Q_OBJECT

public:
    explicit read_xy(QWidget *parent = 0);
    void store_value(int maxx, int minx, int maxy, int miny);
    void initialize(QImage store);
    ~read_xy();
    double START_X;
    double END_X;
    double START_Y;
    double END_Y;
    void initialize_rot(QVector<QVector<QPointF> > prev_points, QVector<QGraphicsPathItem *> prev_path,
                        QVector<int> prev_info);
    void initialize_eff(QVector <QVector<QPointF>> prev_points,
                        QVector < QVector<QGraphicsPathItem*> > prev_path, QVector <double> prev_info);
    void draw_paths();

public slots:
    void display_position(QPoint point);

private slots:
    void on_confirm_clicked();
    void on_delete_button_clicked();
    void on_clear_clicked();
    void on_export_button_clicked();

private:
    Ui::read_xy *ui;
    int x_max, x_min, y_max, y_min;
    int row = 0, max_row = 10;
    QImage image;
    bool tracking = false;
    QVector <QVector < QPointF> > rot_point;
    QVector <QGraphicsPathItem*> rot_path;
    QVector <int> rot_info;
    QVector < QVector<QGraphicsPathItem*> > eff_path;
    QVector <QVector<QPointF> > eff_point;
    QVector <double> eff_info;
    QVector<info_point> intersection_storage;

    void find_xy(double &real_x, double & real_y, int pos_x, int pos_y);
    void add_to_table(double real_x, double real_y, int rot, double eff);
    void display_number();
    void display_curve_num();
    void draw_rot_path();
    void draw_eff_path();
    void find_intersections();
    void draw_intersection();
    void draw_contour();
    bool find_diff(double num1, double num2, double diff);
    bool find_intersect_in(QPointF point, QVector<QPointF> to_find);
    void find_min_max(QVector<QPointF> vector, double &min, double &max);
    int binary_search(QPointF point, int start, int end, QVector<QPointF>to_find);
    void modify_item_at(int index, info_point item);
    int find_repeat(info_point item);
};

#endif // READ_XY_H
