#include "level.h"

Level::Level()
{

}

void Level::AddEntity(Entity *entity)
{
    entities.push_back(entity);
}

QList<Entity *> Level::GetEntities()
{
    return entities;
}

void Level::AddObject(GamingObject *object)
{
    objects.push_back(object);
}

void Level::AddObjects(QList<GamingObject *> objects)
{
    for (GamingObject* object : objects) {
        AddObject(object);
    }
}

QList<GamingObject *> Level::GetObjects()
{
    return objects;
}

void Level::SetMainHero(Hero *hero)
{
    main_hero = hero;
}

Hero *Level::GetMainHero()
{
    return main_hero;
}

void Level::AddBot(Bot *bot)
{
    bots.append(bot);
}

QList<Bot*> Level::GetBots()
{
    return bots;
}

void Level::LoadFromFile(const QString &filepath)
{
    QFile file(filepath);
    file.open(QIODevice::ReadOnly);
    QJsonObject json = QJsonDocument().fromJson(file.readAll()).object();

    main_hero = new Hero();
    main_hero->FromJson(json["main_hero"].toObject());

    int objects_amount = json["objects_amount"].toInt();
    for (int i = 0; i < objects_amount; i++) {
        GamingObject* object = new GamingObject();
        object->FromJson(json["objects" + QString::number(i)].toObject());
        AddObject(object);
    }

    int entities_amount = json["entities_amount"].toInt();
    for (int i = 0; i < entities_amount; i++) {
        Entity* entity = new Entity();
        entity->FromJson(json["entities" + QString::number(i)].toObject());
        AddEntity(entity);
    }

    int bots_amount = json["bots_amount"].toInt();
    for (int i = 0; i < bots_amount; i++) {
        Bot* bot = new Bot();
        bot->FromJson(json["bot" + QString::number(i)].toObject());
        AddBot(bot);
    }
}

void Level::SaveToFile(const QString &filepath)
{
    QJsonObject json;

    if (main_hero)
        json.insert("main_hero", main_hero->ToJson());

    json.insert("objects_amount", objects.length());
    for (int i = 0; i < objects.length(); i++) {
        json.insert("objects" + QString::number(i), objects[i]->ToJson());
    }

    json.insert("entities_amount", entities.length());
    for (int i = 0; i < entities.length(); i++) {
        json.insert("entities" + QString::number(i), entities[i]->ToJson());
    }

    json.insert("bots_amount", bots.length());
    for (int i = 0; i < bots.length(); i++) {
        json.insert("bot" + QString::number(i), bots[i]->ToJson());
    }
    QFile file(filepath);
    file.open(QIODevice::WriteOnly);
    file.write(QJsonDocument(json).toJson());
    file.close();
}

