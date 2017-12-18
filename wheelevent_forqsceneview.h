#ifndef WHEELEVENT_FORQSCENEVIEW_H
#define WHEELEVENT_FORQSCENEVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QScrollBar>
#include <QKeyEvent>
#include <QApplication>

class WheelEvent_forQSceneView:public QGraphicsView
{
    bool spacePressed;

public:
    WheelEvent_forQSceneView(QWidget* parent = 0):QGraphicsView(parent)
    {
        spacePressed = false;
    }
    void wheelEvent(QWheelEvent *event)
    {
        const double scaleFactor = 1.15;
        if(event->delta() > 0)
        {
            // Zoom in
            scale(scaleFactor, scaleFactor);
        }
        else
        {
            // Zooming out
            scale(1.0 / scaleFactor, 1.0 / scaleFactor);
        }
    }
    /*
    void mousePressEvent ( QMouseEvent * e )
    {
        if (e->button() == Qt::RightButton){
            this->setCursor(Qt::ClosedHandCursor);
            isTouched = true;
            setDragMode(QGraphicsView::ScrollHandDrag);
            QGraphicsView::mousePressEvent(e);
            QGraphicsView::mousePressEvent(new QMouseEvent(QEvent::MouseButtonPress, e->pos(), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
        }
        else QGraphicsView::mousePressEvent(e);
    }

    void mouseReleaseEvent ( QMouseEvent * e )
    {
        QGraphicsView::mouseReleaseEvent(e);
        if (isTouched)
        {
            isTouched = false;
            this->setCursor(Qt::ArrowCursor);
            setDragMode(QGraphicsView::NoDrag);
        }
    }

    void mouseMoveEvent ( QMouseEvent * e )
    {
        if (isTouched)
        {
            QMouseEvent *pres = new QMouseEvent(QEvent::MouseMove, e->pos(), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
            QGraphicsView::mouseMoveEvent(pres);
        }
        else QGraphicsView::mouseMoveEvent(e);
    }*/

    void keyPressEvent(QKeyEvent * event)
    {
        if(!spacePressed && !event->isAutoRepeat())
            if(event->key() == Qt::Key_Space)
            {
                setDragMode(QGraphicsView::ScrollHandDrag);
                spacePressed = true;
                qDebug() << "haha";
            }
        QGraphicsView::keyPressEvent(event);
    }

    void keyReleaseEvent(QKeyEvent * event)
    {
        if(event->key() == Qt::Key_Space && !event->isAutoRepeat())
        {
            setDragMode(QGraphicsView::NoDrag);
            spacePressed = false;
            qDebug() << "hehehe";
        }
        QGraphicsView::keyPressEvent(event);
    }
};

#endif // WHEELEVENT_FORQSCENEVIEW_H
