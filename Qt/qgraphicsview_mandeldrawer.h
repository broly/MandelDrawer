#ifndef QGRAPHICSVIEW_MANDELDRAWER_H
#define QGRAPHICSVIEW_MANDELDRAWER_H

#include <QObject>
#include <QGraphicsView>

class QGraphicsView_MandelDrawer : public QGraphicsView
{
    Q_OBJECT
public:
    QGraphicsView_MandelDrawer(QWidget* Parent);

    void wheelEvent(QWheelEvent *event) override;

    std::function<void(float)> OnZoom;
    std::function<void(QPoint)> OnPan;

    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;

    bool _pan;
    int _panStartX, _panStartY;
signals:

};

#endif // QGRAPHICSVIEW_MANDELDRAWER_H
