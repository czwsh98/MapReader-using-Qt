/*
* draggable_ellipse.h
* Contains header file for a customized QGraphicsEllipseItem
*
* Written by: Ziwei Chen
*         on: 8-10-2017
*         Jiaotong University All Rights Reserved
*/

#ifndef DRAGGABLE_ELLIPSE_H
#define DRAGGABLE_ELLIPSE_H

#include <QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>
#include <QDrag>
#include <QMimeData>
#include <QByteArray>
#include <QWidget>

class draggable_ellipse : public QGraphicsEllipseItem{
public:
    draggable_ellipse();
    draggable_ellipse(qreal x, qreal y, qreal width, qreal height);
    int return_id();
    void set_id(int id);

private:
    int number;
};

#endif // DRAGGABLE_ELLIPSE_H
