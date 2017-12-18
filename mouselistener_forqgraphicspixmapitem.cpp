#include "mouselistener_forqgraphicspixmapitem.h"
#include <QGraphicsScene>
//#include <QDebug>

void MouseListener_forQGraphicsPixmapItem::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(spacePressed)
    {
        QGraphicsPixmapItem::mousePressEvent(mouseEvent);
    }
    else
        if(mouseEvent->button() == Qt::LeftButton)
        {
            draw_mode = true;
            start_point = mouseEvent->scenePos();
        }
        else
        {
            if(mouseEvent->button() == Qt::MidButton)
            {
                mark_mode = true;
                start_point = mouseEvent->scenePos();
            }
            else
            {
                if(mouseEvent->button() == Qt::RightButton)
                {
                    right_btn_pressed = true;
                }
            }
        }
    //QGraphicsPixmapItem::mousePressEvent(mouseEvent);
    scene()->update();
}

void MouseListener_forQGraphicsPixmapItem::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(spacePressed || right_btn_pressed)
        QGraphicsPixmapItem::mouseMoveEvent(mouseEvent);
    else
    if(draw_mode || mark_mode)
    {
        QRectF rect;
        qreal x1 = start_point.x(),
                y1 = start_point.y(),
                x2 = mouseEvent->lastScenePos().x(),
                y2 = mouseEvent->lastScenePos().y();

        if(x1>x2)
        {
            if(y1>y2) rect.setCoords(x2,y2,x1,y1);
            else rect.setCoords(x2, y1, x1, y2);
        }
        else
        {
            if(y1>y2) rect.setCoords(x1,y2,x2,y1);
            else rect.setCoords(x1,y1,x2,y2);
        }
        if(rect.width() > 5.0 && rect.height() > 5.0)
        {
            bounding_rect.setRect(rect);
            if(!rect_added)
            {
                rect_added = true;
                scene()->addItem(&bounding_rect);
            }
        }
    }

}

void MouseListener_forQGraphicsPixmapItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(spacePressed)
    {
        /*right_btn_pressed = false;
        draw_mode = false;*/
        QGraphicsPixmapItem::mouseReleaseEvent(event);
    }
    else
        if(draw_mode || mark_mode || right_btn_pressed)
        {
            QRectF rect;
            qreal x1 = start_point.x(),
                    y1 = start_point.y(),
                    x2 = event->lastScenePos().x(),
                    y2 = event->lastScenePos().y();

            if(x1>x2)
            {
                if(y1>y2) rect.setCoords(x2,y2,x1,y1);
                else rect.setCoords(x2, y1, x1, y2);
            }
            else
            {
                if(y1>y2) rect.setCoords(x1,y2,x2,y1);
                else rect.setCoords(x1,y1,x2,y2);
            }
            if(rect_added)
            {
                scene()->removeItem(&bounding_rect);
                rect_added = false;
            }
            if(mark_mode)
            {
                mark_mode=false;
                QList<QGraphicsItem*> items = this->scene()->items(rect);
                for(QList<QGraphicsItem*>::iterator i = items.begin(); i != items.end(); i++)
                {
                    QGraphicsItem* item = *i;

                    if(item->type() == 3)
                    {
                        QGraphicsRectItem *rec=(QGraphicsRectItem*) item;
                        if(rec->pen() == QPen(Qt::red))
                            rec->setPen(QPen(Qt::green));
                    }
                }
            }
            else
                if(draw_mode)
                {
                    draw_mode=false;
                    if(rect.width() > 5.0 && rect.height() > 5.0)
                        this->scene()->addRect(rect,QPen(Qt::green));
                    else
                    {
                        QGraphicsItem* item = this->scene()->itemAt(event->lastScenePos(),QTransform());
                        if(item->type() == 3)
                        {
                            //this->scene()->removeItem(item);
                            QGraphicsRectItem *rec=(QGraphicsRectItem*) item;
                            if(rec->pen() == QPen(Qt::red))
                                rec->setPen(QPen(Qt::green));
                        }
                    }
                }
            else
                if(right_btn_pressed)
                {
                    QGraphicsItem* item = this->scene()->itemAt(event->lastScenePos(),QTransform());
                    if(item->type() == 3)
                        this->scene()->removeItem(item);
                }
        }
}

/*void MouseListener_forQGraphicsPixmapItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsItem* item = this->scene()->itemAt(mouseEvent->scenePos(),QTransform());
    if(item->type() == 3){
        this->scene()->removeItem(item);
    }
}*/

void MouseListener_forQGraphicsPixmapItem::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Control)
        mark_mode = true;

    if(event->key() == Qt::Key_Space)
        spacePressed = true;
}

void MouseListener_forQGraphicsPixmapItem::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Control)
        mark_mode = false;

    if(event->key() == Qt::Key_Space)
        spacePressed = false;
}

/*void MouseListener_forQGraphicsPixmapItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsItem* item = this->scene()->itemAt(event->scenePos(),QTransform());

    if(item->type() == 3){
        QGraphicsRectItem *rec=(QGraphicsRectItem*) item;
        if(rec->pen() == QPen(Qt::red))
            rec->setPen(QPen(Qt::green));
    }
}*/
