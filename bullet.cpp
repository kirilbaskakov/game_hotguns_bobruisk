#include "bullet.h"

Bullet::Bullet(int owner_id)
{
    this->owner_id = owner_id;
}

int Bullet::GetOwnerId() const
{
    return owner_id;
}

void Bullet::Collide(const GamingObject* object)
{
    if (object->GetId() != owner_id && dynamic_cast<const Bullet*>(object) == nullptr) {
        emit disappear(this);
    }
}
