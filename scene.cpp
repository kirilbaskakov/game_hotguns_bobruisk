#include "scene.h"

Scene::Scene(QObject *parent)
    : QGraphicsScene{parent}
{

}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    emit MouseMovedSignal(event->scenePos().x(), event->scenePos().y());
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit MousePressedSignal(event->scenePos().x(), event->scenePos().y(), event->button());
}

void Scene::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    emit WheelSignal(event->delta(), event->orientation());
}
