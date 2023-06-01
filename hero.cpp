#include "hero.h"

Hero::Hero()
{
    gun = new Gun();
}

void Hero::Collide(const GamingObject *object)
{
    const Bullet* bullet_ptr = dynamic_cast<const Bullet*>(object);
    if (bullet_ptr) {
        if (bullet_ptr->GetOwnerId() != GetId()) {
            Die();
        }
    } else {
        SetIsMoving(false);
    }
}

void Hero::SetIsDead(bool new_is_dead)
{
    is_dead = new_is_dead;
}

bool Hero::IsDead()
{
    return is_dead;
}

QList<Bullet *>Hero::Shoot()
{
    gun->SetOwnerId(GetId());
    return gun->shoot(GetCenter(), GetAngle());
}

void Hero::Die()
{
    is_dead = true;
    SetPixmap(Pixmap(":textures/hero/hero_dead_front1.png", 100, 100, GetAngle()));
    SetIsObstacle(false);
    SetZValue(0);
    SetIsVisibleThrough(true);
    emit Dead();
}
