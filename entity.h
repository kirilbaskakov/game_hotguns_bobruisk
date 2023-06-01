#ifndef ENTITY_H
#define ENTITY_H

#include "gamingobject.h"
#include <cmath>

class Entity : public GamingObject
{
    Q_OBJECT
public:
    Entity();

    void SetIsMoving(bool new_is_moving);
    bool GetIsMoving();

    double GetSpeed() const;
    void SetSpeed(double new_speed);

    double GetMovingAngle() const;
    void SetMovingAngle(double new_moving_angle);

    void SetMovingOnX(int dx);
    void SetMovingOnY(int dy);

    QPointF NextPosition();

    void MakeMove();

    virtual QJsonObject ToJson() override;
    virtual void FromJson(QJsonObject json) override;

signals:
    void disappear(GamingObject* object);

private:
    bool is_moving = false;

    int dx = 0;
    int dy = 0;

    int last_time_moved = 0;

    double speed = 0;
    double moving_angle = 0;
};

#endif // ENTITY_H
