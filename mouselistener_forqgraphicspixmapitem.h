#ifndef MOUSELISTENER_FORQGRAPHICSPIXMAPITEM_H
#define MOUSELISTENER_FORQGRAPHICSPIXMAPITEM_H

#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
#include <QPen>
#include <QKeyEvent>
#include <QDebug>

class MouseListener_forQGraphicsPixmapItem :public QGraphicsPixmapItem
{
protected:
    bool rect_added;
    bool right_btn_pressed;
    bool draw_mode;
    bool mark_mode;
    bool spacePressed;
    QPointF start_point;
    QGraphicsRectItem bounding_rect;

public:
    explicit MouseListener_forQGraphicsPixmapItem(const QPixmap & pixmap, QGraphicsItem * parent = 0):QGraphicsPixmapItem(pixmap, parent)
    {
        rect_added = false;
        draw_mode = false;
        right_btn_pressed = false;
        mark_mode = false;
        spacePressed = false;
        //this->setAcceptHoverEvents(true);
        this->setFlag(QGraphicsItem::ItemIsFocusable, true);
        bounding_rect.setPen(QPen(Qt::green));
    }

    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    //void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void keyPressEvent(QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent * event);
    //void hoverMoveEvent(QGraphicsSceneHoverEvent * event);
};

#endif // MOUSELISTENER_FORQGRAPHICSPIXMAPITEM_H
