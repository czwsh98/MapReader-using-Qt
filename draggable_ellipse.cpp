/*
* draggable_ellipse.cpp
* Contains the implementation for a customized QGraphicsEllipseItem
*
* Written by: Ziwei Chen
*         on: 8-10-2017
*         Jiaotong University All Rights Reserved
*/

#include "draggable_ellipse.h"
#include <QtWidgets>

draggable_ellipse::draggable_ellipse()
{
    setFlags(QGraphicsItem::ItemIsSelectable);
    setFlags(QGraphicsItem::ItemIsFocusable);
    setCursor(Qt::OpenHandCursor);
    setFlags(QGraphicsItem::ItemIsSelectable);
    setAcceptedMouseButtons(Qt::LeftButton);
}

draggable_ellipse::draggable_ellipse(qreal x, qreal y, qreal width, qreal height){
    setFlags(QGraphicsItem::ItemIsSelectable);
    setFlags(QGraphicsItem::ItemIsFocusable);
    setRect(x, y, width, height);
    setBrush(QBrush(Qt::SolidPattern));
    setPen(QPen(Qt::green));
    setCursor(Qt::OpenHandCursor);
    setAcceptHoverEvents(true);
    setZValue(1);
}

void draggable_ellipse::set_id(int id){
    number = id;
}

int draggable_ellipse::return_id(){
    return number;
}
