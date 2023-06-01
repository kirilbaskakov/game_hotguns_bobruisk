#ifndef GUN_H
#define GUN_H

#include "bullet.h"

class Gun
{
public:
    Gun();

    void SetOwnerId(int new_owner_id);

    QList<Bullet*> shoot(QPointF pos, double angle);
private:
    int bullets = 0;

    int owner_id = -1;

    int last_time_shoot = 0;

    int cooldown = 200;
};

#endif // GUN_H
