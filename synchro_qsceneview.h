#ifndef SYNCHRO_QSCENEVIEW_H
#define SYNCHRO_QSCENEVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>
#include <QScrollBar>


class Synchro_qsceneview :public QGraphicsView
{

    Q_OBJECT

signals:
    void synchro_wheel(int delta);

public:
    Synchro_qsceneview(QWidget* parent = 0):QGraphicsView(parent){}

    void wheelEvent(QWheelEvent *event)
    {
        double scaleFactor = 1.15;
        if(event->delta() < 0)
            scaleFactor = 1.0/scaleFactor;

        this->horizontalScrollBar()->blockSignals(true);
        this->verticalScrollBar()->blockSignals(true);

        scale(scaleFactor, scaleFactor);

        this->horizontalScrollBar()->blockSignals(false);
        this->verticalScrollBar()->blockSignals(false);

        emit synchro_wheel(event->delta());
        emit this->horizontalScrollBar()->valueChanged(this->horizontalScrollBar()->value());
        emit this->verticalScrollBar()->valueChanged(this->verticalScrollBar()->value());
    }

public slots:
    void m_wheelEvent(int delta)
    {
        double scaleFactor = 1.15;
        if(delta < 0)
            scaleFactor = 1.0/scaleFactor;

        scale(scaleFactor, scaleFactor);
    }
};

#endif // SYNCHRO_QSCENEVIEW_H
