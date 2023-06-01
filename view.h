#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QList>
#include <QMouseEvent>
#include <QHash>
#include "gamingobject.h"
#include "scene.h"
#include <QCursor>

class View : public QGraphicsView
{
    Q_OBJECT
public:
    explicit View(QWidget *parent = nullptr);

    void AddGamingObject(GamingObject* object, bool is_static = false);

    void RemoveGamingObject(GamingObject* object);

    void RepaintGamingObject(GamingObject* object);

    void ClearGamingObjects();

    QList<GamingObject*> GetCollidingObjects(const GamingObject* object);

    bool IsObjectVisible(const GamingObject* object, const GamingObject* by);

    void SetCentralObject(const GamingObject* object);
private slots:
    void MouseMovedSlot(int x, int y);
    void MousePressedSlot(int x, int y, Qt::MouseButton button);
    void WheelSlot(int delta, Qt::Orientation orientation);
signals:
    void MouseMovedSignal(int x, int y);
    void MousePressedSignal(int x, int y, Qt::MouseButton button);
    void WheelSignal(int delta, Qt::Orientation orientation);

private:

    void UpdateSceneRect();

    Scene* scene;

    int central_item = -1;

    QHash<int, QGraphicsPixmapItem*> items;
    QHash<int, std::pair<QGraphicsPixmapItem*, GamingObject*>> static_items;
    QHash<QGraphicsPixmapItem*, GamingObject*> objects;
};

#endif // VIEW_H
