/*
* curve_lines.h
* Contains header file for an interface that identifies rotation curves
*
* Written by: Ziwei Chen
*         on: 8-10-2017
*         Jiaotong University All Rights Reserved
*/

#ifndef CURVE_LINES_H
#define CURVE_LINES_H

#include <QWidget>
#include <QGraphicsPathItem>
#include <QPainterPath>
#include <QVector>
#include "efficiency_lines.h"
#include "info_point.h"

namespace Ui {
class curve_lines;
}

class curve_lines : public QWidget
{
    Q_OBJECT

public:
    explicit curve_lines(QWidget *parent = 0);
    ~curve_lines();
    void initialize_image(QImage store);
    void initialize_next(int maxx, int minx, int maxy, int miny);
    void clear_path();
    void calculate_points(int num);

public slots:
    void add_point(QPoint point);
    void drop_add(int number, QPoint point);

private slots:
    void on_start_drag_clicked();
    void on_confirm_button_clicked();
    void on_next_line_button_clicked();
    void on_delete_progress_clicked();
    void on_storage_clear_clicked();

private:
    Ui::curve_lines *ui;
    efficiency_lines* a;
    int recorded_index;
    QVector <QPointF> point_storage;
    QVector <QGraphicsPathItem*> path_storage;
    QVector <QVector<QPointF> > path_point_storage;
    QVector <int> info_storage;
    void draw();
    void set_text(int number);
    void find_max_min(double &max, double &min, QVector<QPointF> storage);

};

#endif // CURVE_LINES_H
