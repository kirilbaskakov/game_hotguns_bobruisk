#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "view.h"
#include <QKeyEvent>
#include "gamingobject.h"
#include <QTimer>
#include <QHash>
#include "level.h"
#include "levelcreatorcontroller.h"
#include "levelplaycontroller.h"
#include "bot.h"
#include "menu.h"

class Controller : public QObject
{
    Q_OBJECT
public:
    Controller(QSize size, QWidget* parent = nullptr);

    void Clear();

    void LoadLevelCreator();
    void LoadMainMenu();
    void Exit();
    void LoadLevelSelection();

    void HandleKeyPressEvent(QKeyEvent* event);
    void HandleKeyReleaseEvent(QKeyEvent* event);

    void CreateObject(GamingObject* object, bool is_static = false);
    void CreateObjects(QList<GamingObject*> objects, bool is_static = false);
    void CreateMovingObject(Entity* entity);
    void CreateBot(Bot* bot);

    View* GetView();

public slots:
    void HandleMouseMoveEvent(int x, int y);

    void HandleMouseClickEvent(int x, int y, Qt::MouseButton button);

    void HandleWheelEvent(int delta, Qt::Orientation orientation);

    void RemoveObject(GamingObject* object);

    void UpdateScene();
private:
    enum Mode {
        SELECTING_LEVEL,
        PLAYING_LEVEL,
        CREATING_LEVEL,
        IN_MENU,
        LOADING
    };

    Mode mode = IN_MENU;

    View* view;

    QTimer* timer;

    QSize window_size;

    QHash<int, Entity*> moving_objects;
    QHash<int, Bot*> bots;
    QHash<int, GamingObject*> all_objects;

    LevelCreatorController* level_creator = nullptr;
    LevelPlayController* level_player = nullptr;

    bool CheckCollisions(Entity* entity, QPointF delta);
};

#endif // CONTROLLER_H
