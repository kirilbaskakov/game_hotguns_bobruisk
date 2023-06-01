#ifndef GAMINGOBJECT_H
#define GAMINGOBJECT_H

#include "QPointF"
#include "pixmap.h"
#include "QRectF"
#include "QTransform"
#include <QObject>
#include <QJsonObject>
#include <cmath>

class GamingObject : public QObject
{
    Q_OBJECT
public:
    GamingObject();

    QPointF GetPosition() const;
    void SetPosition(QPointF new_position);

    QPointF GetCenter() const;

    double GetAngle() const;
    void SetAngle(double new_angle);

    int GetId() const;

    Pixmap GetPixmap() const;
    void SetPixmap(Pixmap pixmap);

    void SetIsObstacle(bool is_obstacle);
    bool GetIsObstacle();

    void SetIsVisibleThrough(bool is_visible_through);
    bool GetIsVisibleThrough();

    void SetZValue(int new_z_value);
    int  GetZValue();

    void Rotate(double angle);

    void Move(QPointF offset);

    bool IsUpdated();
    void Updated();

    virtual QJsonObject ToJson();
    virtual void FromJson(QJsonObject json);

    virtual void Collide(const GamingObject* object);
    virtual void Clicked();
    virtual void Hover();
private:
    static int objects_amount;

    bool is_updated = false;

    QPointF position = QPointF(0, 0);
    double angle = 0;

    Pixmap pixmap;

    bool is_obstacle = true;
    bool is_visible_through = false;

    int z_value = 0;

    int id;
};

#endif // GAMINGOBJECT_H
