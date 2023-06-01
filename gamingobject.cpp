#include "gamingobject.h"

int GamingObject::objects_amount = 0;

GamingObject::GamingObject()
{
    id = objects_amount++;
}

QPointF GamingObject::GetPosition() const
{
    return position;
}

void GamingObject::SetPosition(QPointF new_position)
{
    is_updated = false;
    position = new_position;
}

QPointF GamingObject::GetCenter() const
{
    return position + QPointF(pixmap.width() / 2, pixmap.height() / 2);
}

double GamingObject::GetAngle() const
{
    return angle;
}

void GamingObject::SetAngle(double new_angle)
{
    is_updated = false;
    if (new_angle < 0) {
        new_angle += 360;
    }
    if (new_angle > 360) {
        new_angle -= 360;
    }
    angle = new_angle;
}

int GamingObject::GetId() const
{
    return id;
}

Pixmap GamingObject::GetPixmap() const
{
    return pixmap;
}

void GamingObject::SetPixmap(Pixmap new_pixmap)
{
    is_updated = false;
    pixmap = new_pixmap;
}

void GamingObject::SetIsObstacle(bool is_obstacle)
{
    is_updated = false;
    this->is_obstacle = is_obstacle;
}

bool GamingObject::GetIsObstacle()
{
    return is_obstacle;
}

void GamingObject::SetIsVisibleThrough(bool is_visible_through)
{
    this->is_visible_through = is_visible_through;
}

bool GamingObject::GetIsVisibleThrough()
{
    return is_visible_through;
}

void GamingObject::SetZValue(int new_z_value)
{
    is_updated = false;
    z_value = new_z_value;
}

int GamingObject::GetZValue()
{
    return z_value;
}

void GamingObject::Rotate(double angle)
{
    is_updated = false;
    SetAngle(this->angle + angle);
}

void GamingObject::Move(QPointF offset)
{
    is_updated = false;
    SetPosition(position + offset);
}

bool GamingObject::IsUpdated()
{
    return is_updated;
}

void GamingObject::Updated()
{
    is_updated = true;
}

QJsonObject GamingObject::ToJson()
{
    QJsonObject json;
    json.insert("pixmap", pixmap.ToJson());
    json.insert("angle", angle);
    json.insert("position_x", position.x());
    json.insert("position_y", position.y());
    json.insert("is_obstacle", is_obstacle);
    json.insert("is_visible_through", is_visible_through);
    json.insert("z_value", z_value);
    return json;
}

void GamingObject::FromJson(QJsonObject json)
{
    pixmap.FromJson(json["pixmap"].toObject());
    SetPosition(QPointF(json["position_x"].toInt(), json["position_y"].toInt()));
    SetAngle(json["angle"].toDouble());
    SetIsObstacle(json["is_obstacle"].toBool());
    SetIsVisibleThrough(json["is_visible_through"].toBool());
    SetZValue(json["z_value"].toInt());
}

void GamingObject::Collide(const GamingObject *object)
{

}

void GamingObject::Clicked()
{

}

void GamingObject::Hover()
{
}






