#include "qgraphicsview_mandeldrawer.h"
#include <QWheelEvent>

QGraphicsView_MandelDrawer::QGraphicsView_MandelDrawer(QWidget* Parent)
    : QGraphicsView(Parent)
{

}
void QGraphicsView_MandelDrawer::wheelEvent(QWheelEvent *event)
{
    OnZoom(event->angleDelta().y());
    event->accept();
}


void QGraphicsView_MandelDrawer::mouseMoveEvent(QMouseEvent *event)
{
    // if (event->button() == Qt::LeftButton)
    if (_pan)
    {
        // _pan = true;
        // _panStartX = event->x();
        // _panStartY = event->y();
        OnPan({event->x() - _panStartX, event->y() - _panStartY});
        setCursor(Qt::ClosedHandCursor);
        event->accept();
        return;
    }
    event->ignore();
}
void QGraphicsView_MandelDrawer::mousePressEvent(QMouseEvent *event)
{
    // if (event->button() == Qt::RightButton)
    {
        _pan = true;
        _panStartX = event->x();
        _panStartY = event->y();
        setCursor(Qt::ArrowCursor);
        event->accept();
        return;
    }
    event->ignore();
}
void QGraphicsView_MandelDrawer::mouseReleaseEvent(QMouseEvent *event)
{
    if (_pan)
    {
        _pan = false;
        // horizontalScrollBar()->setValue(horizontalScrollBar()->value() - (event->x() - _panStartX));
        // verticalScrollBar()->setValue(verticalScrollBar()->value() - (event->y() - _panStartY));
        _panStartX = event->x();
        _panStartY = event->y();

        event->accept();
        return;
    }
    event->ignore();
}
