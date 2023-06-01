#ifndef LEVELPLAYCONTROLLER_H
#define LEVELPLAYCONTROLLER_H

#include <QObject>
#include <QKeyEvent>
#include <gamingobject.h>
#include <level.h>
#include <entity.h>
#include <hero.h>
#include <menu.h>

class LevelPlayController : public QObject
{
    Q_OBJECT
public:
    LevelPlayController();

    void LoadLevel(Level& level, bool reload = false);

    void KeyPressed(QKeyEvent* event);
    void KeyReleased(QKeyEvent* event);
    void MouseClick(int x, int y, Qt::MouseButton button);
    void MouseMove(int x, int y);

    bool IsInMenu();

    QList<GamingObject*> GetObjects();
    Entity* GetCentralObject();

private slots:
    void BotKilled();

    void HeroKilled();

    void Close();
signals:
    void ObjectCreated(GamingObject* object, bool is_static);
    void MovingObjectCreated(Entity* entity);
    void BotCreated(Bot* bot);

    void ObjectRemoved(GamingObject* object);

    void Closed();

private:
    Hero* hero = nullptr;
    QList<GamingObject*> objects;
    QList<Entity*> entities;
    QList<Bot*> bots;
    Level level;

    int bots_alive = 0;

    Menu* menu;
    Menu* top_menu;

    bool in_menu = false;

    void ShowMenu();
    void CloseMenu();
};

#endif // LEVELPLAYCONTROLLER_H
