#include "gun.h"

Gun::Gun()
{

}

void Gun::SetOwnerId(int new_owner_id)
{
    owner_id = new_owner_id;
}

QList<Bullet*> Gun::shoot(QPointF pos, double angle)
{
    QList<Bullet*> list;
    int curr_time = clock();
    if (curr_time - last_time_shoot < cooldown) {
        return list;
    }
    last_time_shoot = curr_time;
    Bullet* bullet = new Bullet(owner_id);
    bullet->SetPixmap(Pixmap(":/textures/bullet.png", 15, 15));
    bullet->SetPosition(QPointF(pos.x() + 50 * cos(angle / 180 * 3.14159), pos.y() + 50 * sin(angle / 180 * 3.14159)));
    bullet->SetIsObstacle(false);
    bullet->SetIsVisibleThrough(true);
    bullet->SetAngle(angle);
    bullet->SetMovingAngle(angle);
    bullet->SetSpeed(1350);
    bullet->SetIsMoving(true);
    list.append(bullet);
    return list;
}
