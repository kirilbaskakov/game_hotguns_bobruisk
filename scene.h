#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = nullptr);

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    void wheelEvent(QGraphicsSceneWheelEvent *event);
signals:
    void MouseMovedSignal(int x, int y);
    void MousePressedSignal(int x, int y, Qt::MouseButton button);
    void WheelSignal(int delta, Qt::Orientation orientation);
};

#endif // SCENE_H
