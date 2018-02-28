#ifndef CUSTOM_GRAPHICS_H
#define CUSTOM_GRAPHICS_H
/*
* custom_graphics.h
* Contains implementation for a customized QGraphicsView
*
* Written by: Ziwei Chen
*         on: 8-10-2017
*         Jiaotong University All Rights Reserved
*/

#include "draggable_ellipse.h"
#include <QMouseEvent>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPainter>
#include <QPainterPath>
#include <QVector>
#include <QString>

class custom_graphics : public QGraphicsView
{
    Q_OBJECT
public:
    custom_graphics(QWidget *parent = 0);
    void set_image(QImage image);
    void clear();
    void delete_number(int number);
    void drawPath(QGraphicsPathItem* path_item);
    void set_drag(bool dragover);
    void remove_path(QGraphicsPathItem* remove_one);
    void set_bound(QImage image);
    void add_point(QPointF point);
    QGraphicsScene* scene;

signals:
    void mouse_pressed(QPoint point);
    void dropadd_point(int number, QPoint point);

public slots:
    void mousePressEvent(QMouseEvent* event) override;

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    bool dragging = false;
    double x, y;
    int id = 0;
    //QGraphicsScene* scene;
    QVector<draggable_ellipse *> ellipse_storage;
    QPixmap paint_pixmap();
    void process_storage(draggable_ellipse* item);
};

#endif // CUSTOM_GRAPHICS_H
