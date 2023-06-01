#ifndef BULLET_H
#define BULLET_H

#include "entity.h"

class Bullet : public Entity
{
    Q_OBJECT
public:
    Bullet(int owner_id);

    int GetOwnerId() const;

    virtual void Collide(const GamingObject* object) override;

private:
    int owner_id = 0;
};

#endif // BULLET_H
