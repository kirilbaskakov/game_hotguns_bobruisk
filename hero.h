#ifndef HERO_H
#define HERO_H

#include "entity.h"
#include "gun.h"

class Hero : public Entity
{
    Q_OBJECT
public:
    Hero();

    virtual void Collide(const GamingObject* object) override;

    void SetIsDead(bool new_is_dead);
    bool IsDead();

    QList<Bullet*> Shoot();

signals:
    void Dead();

private:

    bool is_dead = false;

    void Die();

    Gun* gun = nullptr;
};

#endif // HERO_H
