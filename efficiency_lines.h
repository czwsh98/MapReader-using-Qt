/*
* efficiency_lines.h
* Contains the header file for the efficiency line interface
*
* Written by: Ziwei Chen
*         on: 8-10-2017
*         Jiaotong University All Rights Reserved
*/

#ifndef EFFICIENCY_LINES_H
#define EFFICIENCY_LINES_H

#include <QWidget>
#include <QGraphicsPathItem>
#include <QPainterPath>
#include <QVector>
#include "read_xy.h"
#include "info_point.h"

namespace Ui {
class efficiency_lines;
}

class efficiency_lines : public QWidget
{
    Q_OBJECT

public:
    explicit efficiency_lines(QWidget *parent = 0);
    void initialize_image(QImage store);
    void initialize_next(int maxx, int minx, int maxy, int miny);
    void clear_path();
    void calculate_points();
    void store_prev_info(QVector <QVector <QPointF> > prev_points, QVector <QGraphicsPathItem*> prev_path,
                         QVector<int> prev_info);
    ~efficiency_lines();

public slots:
    void add_point(QPoint point);
    void drop_add(int number, QPoint point);

private slots:
    void on_start_drag_clicked();
    void on_confirm_button_clicked();
    void on_next_line_button_clicked();
    void on_delete_button_clicked();
    void on_clear_store_clicked();

private:
    Ui::efficiency_lines *ui;
    read_xy* a;
    int recorded_index;
    int curr_index;
    QVector <QPointF> point_storage;
    QVector < QVector<QGraphicsPathItem*> > path_storage;
    QVector <QGraphicsPathItem*> temp_path;
    QVector <QVector<QPointF> > path_point_storage;
    QVector <double> info_storage;
    void draw();
    void set_text(int number);
    void find_max_min(double &max, double &min, QVector<QPointF> storage);
};

#endif // EFFICIENCY_LINES_H
