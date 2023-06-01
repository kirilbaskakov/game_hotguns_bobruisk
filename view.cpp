#include "view.h"

View::View(QWidget *parent)
    : QGraphicsView(parent)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setDragMode(DragMode::NoDrag);

    scene = new Scene(parent);
    setScene(scene);

    QCursor cursor;
    cursor.setShape(Qt::CrossCursor);
    setCursor(cursor);

    setMouseTracking(true);

    QObject::connect(scene, &Scene::MouseMovedSignal, this, &MouseMovedSlot);
    QObject::connect(scene, &Scene::MousePressedSignal, this, &MousePressedSlot);
    QObject::connect(scene, &Scene::WheelSignal, this, &WheelSlot);
    scene->setSceneRect(0, 0, size().width(), size().height());
}

void View::AddGamingObject(GamingObject *object, bool is_static)
{
    QGraphicsPixmapItem* item = scene->addPixmap(object->GetPixmap());
    item->setShapeMode(QGraphicsPixmapItem::HeuristicMaskShape);
    item->setPos(object->GetPosition());
    item->setTransformOriginPoint(item->boundingRect().center());
    item->setRotation(object->GetAngle());
    item->setZValue(object->GetZValue());
    if (is_static) {
        static_items[object->GetId()] = {item, object};
    }
    items[object->GetId()] = item;
    objects[item] = object;
}

void View::RemoveGamingObject(GamingObject *object)
{
    int id = object->GetId();
    scene->removeItem(items[id]);
    objects.erase(objects.find(items[id]));
    static_items.erase(static_items.find(id));
    delete *items.find(id);
    items.erase(items.find(id));
}

void View::ClearGamingObjects()
{
    QList<GamingObject*> objects_list = objects.values();
    for (auto object : objects_list) {
        RemoveGamingObject(object);
    }
    objects.clear();
    items.clear();
    static_items.clear();
    central_item = -1;
}

QList<GamingObject*> View::GetCollidingObjects(const GamingObject *object)
{
    QList<QGraphicsItem*> colliding_items = items[object->GetId()]->collidingItems();
    QList<GamingObject*> colliding_objects;
    for (QGraphicsItem* item : colliding_items) {
        QGraphicsPixmapItem* pixmap_item = dynamic_cast<QGraphicsPixmapItem*>(item);
        if (pixmap_item && objects.count(pixmap_item)) {
            colliding_objects.push_back(objects[pixmap_item]);
        }
    }
    return colliding_objects;
}

bool View::IsObjectVisible(const GamingObject *object, const GamingObject *by)
{
    bool is_visible = true;
    QPointF end = items[object->GetId()]->sceneBoundingRect().center();
    QPointF start = items[by->GetId()]->sceneBoundingRect().center();
    QGraphicsItem* line = dynamic_cast<QGraphicsItem*> (scene->addLine(QLineF(start, end)));
    QList<QGraphicsItem*> colliding_items = line->collidingItems();
    for (auto item : colliding_items) {
        QGraphicsPixmapItem* pixmap_item = dynamic_cast<QGraphicsPixmapItem*>(item);
        GamingObject* obj = objects[pixmap_item];
        if (pixmap_item && obj->GetId() != object->GetId() && obj->GetId() != by->GetId() && !obj->GetIsVisibleThrough()) {
            is_visible = false;
            break;
        }
    }
    scene->removeItem(line);
    return is_visible;

}

void View::RepaintGamingObject(GamingObject *object)
{
    if (object->IsUpdated()) {
        return;
    }
    object->Updated();
    QGraphicsPixmapItem* item = items[object->GetId()];
    item->setPixmap(object->GetPixmap());
    item->setPos(object->GetPosition());
    item->setTransformOriginPoint(item->boundingRect().center());
    item->setRotation(object->GetAngle());
    item->setZValue(object->GetZValue());
    UpdateSceneRect();
}

void View::SetCentralObject(const GamingObject *object)
{
    central_item = object->GetId();

    UpdateSceneRect();
}

void View::MouseMovedSlot(int x, int y)
{
    for (auto object : objects) {
        if (items[object->GetId()]->contains(QPoint(x, y))) {
            object->Hover();
        }
    }
    emit MouseMovedSignal(x, y);
}

void View::MousePressedSlot(int x, int y, Qt::MouseButton button)
{
    QList<int> objects_id;
    for (auto object : objects) {
        objects_id.append(object->GetId());
    }
    int max_z_value = -100;
    int object_id = -1;
    for (int id : objects_id) {
        GamingObject* object = objects[items[id]];
        if (items[id]->sceneBoundingRect().contains(x, y) && objects[items[id]]->GetZValue() > max_z_value) {
            max_z_value = objects[items[id]]->GetZValue();
            object_id = id;
        }
    }
    if (object_id != -1) {
        objects[items[object_id]]->Clicked();
    }
    emit MousePressedSignal(x, y, button);
}

void View::WheelSlot(int delta, Qt::Orientation orientation)
{
    emit WheelSignal(delta, orientation);
}

void View::UpdateSceneRect()
{
    QRectF prev_scene = sceneRect();
    if (central_item == -1) {
        scene->setSceneRect(0, 0, size().width(), size().height());
    } else {
        QRectF rect = items[central_item]->sceneBoundingRect();
        scene->setSceneRect(rect.x() - size().width() / 2 + rect.width() / 2, rect.y() - size().height() / 2 + rect.height() / 2, size().width(), size().height());
    }
    double x = sceneRect().x();
    double y = sceneRect().y();
    double dx = x - prev_scene.x();
    double dy = y - prev_scene.y();
    for (auto static_item : static_items.values()) {
        static_item.second->Move(QPointF(dx, dy));
        static_item.first->moveBy(dx, dy);
    }
}

