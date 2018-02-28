/*
* custom_graphics.cpp
* Contains implementation for a customized QGraphicsView
*
* Written by: Ziwei Chen
*         on: 8-10-2017
*         Jiaotong University All Rights Reserved
*/

#include "custom_graphics.h"
double rad = 5.0;

custom_graphics::custom_graphics(QWidget *parent) :
     QGraphicsView(parent)
{
    scene = new QGraphicsScene();
}

void custom_graphics::add_point(QPointF point){
    //Initialize a draggable_ellipse at given position
    draggable_ellipse* item_append = new draggable_ellipse(point.x() - rad, point.y() - rad, rad * 2.0, rad* 2.0);
    //Add to the QGraphicsScene
    item_append -> set_id(id);
    id ++;
    scene -> addItem(item_append);
    ellipse_storage.append(item_append);
}

void custom_graphics::mousePressEvent(QMouseEvent *event){
    //Dragging is bool used to switch between Placing Points and Dragging Points
    if (!dragging){
        setCursor(Qt::ArrowCursor);
        QPoint point = event -> pos();
        add_point(point);
        emit mouse_pressed(point);
    }
    //Dragging Start
    else{
        QPointF scenePoint = mapToScene(event->pos()) ;
        //Retrieve the item at the mouse press position
        QGraphicsItem *received = scene -> itemAt(scenePoint, transform());
        //If satisfies the given flag, then it is the draggable_ellipse item that we define
        if (received -> flags() == QGraphicsItem::ItemIsFocusable){
            draggable_ellipse* ellipse_received = dynamic_cast<draggable_ellipse*>(received);
            //QMimeData is used for storing data/information about the draggable_ellipse
            QMimeData *mime = new QMimeData;
            mime -> setText(QString::number(ellipse_received -> return_id()));
            //Construct a QDrag for dragging
            QDrag *drag = new QDrag(this);
            drag -> setMimeData(mime);
            setCursor(Qt::ClosedHandCursor);
            //Call paint_pixmap function to create an image displayed while dragging in process
            QPixmap pixmap = paint_pixmap();
            drag -> setPixmap(pixmap);
            //If drag -> exec satisfies MoveAction, then hide the original ellipseitem
            if (drag -> exec(Qt::MoveAction | Qt::CopyAction, Qt::CopyAction) == Qt::MoveAction)
                ellipse_received -> hide();
        }
    }
}

QPixmap custom_graphics::paint_pixmap(){
    QPixmap pixmap(10, 10);
    pixmap.fill(Qt::black);
    QPainter painter(&pixmap);
    //painter.translate(15, 15);
    painter.setPen(QPen(Qt::green));
    painter.setBrush(QBrush(Qt::SolidPattern));
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawEllipse(-15, -15, 8, 8);
    painter.end();
    return pixmap;
}

void custom_graphics::set_drag(bool dragover){
    dragging = dragover;
}

//This function initializes the image for the QGraphicsScene,
//and adjust the window to the image size
void custom_graphics::set_image(QImage image){
    QPixmap pix_image = QPixmap::fromImage(image);
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(pix_image);
    scene -> addItem(item);
    this -> resize(scene -> itemsBoundingRect().width() + 10, scene -> itemsBoundingRect().height() + 10);
    this -> setScene(scene);
}

void custom_graphics::set_bound(QImage image){
    QPixmap pix_image = QPixmap(image.width(), image.height());
    pix_image.fill(Qt::white);
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(pix_image);
    scene -> addItem(item);
    this -> resize(scene -> itemsBoundingRect().width(), scene -> itemsBoundingRect().height());
    this -> setScene(scene);
}

void custom_graphics::clear(){
    for (int i = 0; i < ellipse_storage.size(); i ++)
        scene -> removeItem(ellipse_storage[i]);
    ellipse_storage.clear();
    id = 0;
}

void custom_graphics::delete_number(int number){
    scene -> removeItem(ellipse_storage[number]);
    ellipse_storage.remove(number);
}

void custom_graphics::drawPath(QGraphicsPathItem* path_item){
    scene -> addItem(path_item);
}

void custom_graphics::dragEnterEvent(QDragEnterEvent* event){
    setCursor(Qt::ClosedHandCursor);
    event -> setAccepted(true);
    update();
}

void custom_graphics::dragMoveEvent(QDragMoveEvent* event){
    setCursor(Qt::ClosedHandCursor);
    event->setAccepted(true);
    update();
}

//Drop the item
void custom_graphics::dropEvent(QDropEvent* event){
    //Drop only if it comes from a mouse press event
    if (event -> mimeData() -> hasText()){
        event->setAccepted(true);
        //Initialize a new item
        draggable_ellipse* new_item = new draggable_ellipse(event->pos().x() - rad, event->pos().y() - rad, rad* 2.0, rad * 2.0);
        //Assign the same id as the item used to have before dragging
        new_item -> set_id(event -> mimeData() -> text().toInt());
        //Add the to the scene
        scene -> addItem(new_item);
        //Modify the point storage that contains the old information and update it
        process_storage(new_item);
        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }
        update();
        //Emit a signal that the drop actio has been finished
        emit(dropadd_point(new_item -> return_id(), event->pos()));
    }
}

void custom_graphics::process_storage(draggable_ellipse *item){
    int num = item -> return_id();
    ellipse_storage[num] = item;
}

void custom_graphics::remove_path(QGraphicsPathItem *remove_one){
    scene -> removeItem(remove_one);
}
