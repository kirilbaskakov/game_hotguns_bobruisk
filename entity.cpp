#include "entity.h"

Entity::Entity()
{

}

void Entity::SetIsMoving(bool new_is_moving)
{
    if (!is_moving && new_is_moving) {
        last_time_moved = clock();
    }
    is_moving = new_is_moving;
}

bool Entity::GetIsMoving()
{
    return is_moving;
}

double Entity::GetSpeed() const
{
    return speed;
}

void Entity::SetSpeed(double new_speed)
{
    speed = new_speed;
}

double Entity::GetMovingAngle() const
{
    return moving_angle;
}

void Entity::SetMovingAngle(double new_moving_angle)
{
    moving_angle = new_moving_angle;
}

void Entity::SetMovingOnX(int dx)
{
    this->dx = dx;
    if (dx == -1) {
        moving_angle = 180 + dy * 45;
    }
    if (dx == 0) {
        if (dy == 0) {
            SetIsMoving(false);
        }
        moving_angle = -dy * 90;
    }
    if (dx == 1) {
        moving_angle = -dy * 45;
    }
    if (moving_angle < 0) {
        moving_angle += 360;
    }

}

void Entity::SetMovingOnY(int dy)
{
    this->dy = dy;
    if (dy == -1) {
        moving_angle = 90 - dx * 45;
    }
    if (dy == 0) {
        if (dx == 0) {
            SetIsMoving(false);
        }
        moving_angle = dx == -1 ? 180 : 0;
    }
    if (dy == 1) {
        moving_angle = 270 + dx * 45;
    }
    if (moving_angle < 0) {
        moving_angle += 360;
    }
}

QPointF Entity::NextPosition()
{
    double time = (clock() - last_time_moved) * 1.0 / CLOCKS_PER_SEC;
    return QPointF(speed * time * cos(moving_angle / 180 * 3.14159), speed * time * sin(moving_angle / 180 * 3.14159));
}

void Entity::MakeMove()
{
    if (!is_moving) {
        return;
    }
    Move(NextPosition());
    last_time_moved = clock();
}

QJsonObject Entity::ToJson()
{
    QJsonObject json = GamingObject::ToJson();
    json.insert("speed", speed);
    return json;
}

void Entity::FromJson(QJsonObject json)
{
    GamingObject::FromJson(json);
    SetSpeed(json["speed"].toDouble());
}
