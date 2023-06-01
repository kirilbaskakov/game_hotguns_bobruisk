#ifndef LEVEL_H
#define LEVEL_H

#include <QString>
#include <QList>
#include <QJsonDocument>
#include <QFile>
#include <QHash>
#include "gamingobject.h"
#include "entity.h"
#include "hero.h"
#include "bot.h"

class Level
{
public:
    Level();

    void AddEntity(Entity* entity);
    QList<Entity*> GetEntities();

    void AddObject(GamingObject* object);
    void AddObjects(QList<GamingObject*> objects);
    QList<GamingObject*> GetObjects();

    void SetMainHero(Hero* hero);
    Hero* GetMainHero();

    void AddBot(Bot* hero);
    QList<Bot*> GetBots();

    void LoadFromFile(const QString& filepath);
    void SaveToFile(const QString& filepath);

private:
    QList<Entity*> entities;
    QList<GamingObject*> objects;
    QList<Bot*> bots;
    Hero* main_hero = nullptr;
};

#endif // LEVEL_H
