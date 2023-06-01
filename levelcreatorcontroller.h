#ifndef LEVELCREATORCONTROLLER_H
#define LEVELCREATORCONTROLLER_H

#include <menu.h>
#include <level.h>
#include <QKeyEvent>
#include <QFileDialog>

class LevelCreatorController : public QObject
{
    Q_OBJECT
public:
    LevelCreatorController(QSize window_size);

    void KeyPressed(QKeyEvent* event);
    void KeyReleased(QKeyEvent* event);
    void MouseClick(int x, int y, Qt::MouseButton button);
    void MouseMove(int x, int y);
    void Wheel(int delta, Qt::Orientation orientation);

    QList<GamingObject*> GetObjects();
    Entity* GetCentralObject();

private slots:
    void LoadWalls();
    void LoadFloor();
    void LoadHero();
    void LoadDecorations();

    void SelectTexture(MenuItem* item);

    void Close();
    void Save();

signals:
    void ObjectCreated(GamingObject* object, bool is_static);
    void ObjectRemoved(GamingObject* object);
    void Closed();
private:
    enum SelectedItemType {
        WALL,
        FLOOR,
        HERO,
        DECORATION
    };

    QSize window_size;

    Level level;

    Menu menu_items_types;
    Menu menu_items_list;
    Menu menu_back;
    Menu menu_save;

    MenuItem* selected_item = nullptr;
    SelectedItemType selected_item_type;

    Entity* central_object = nullptr;

    GamingObject* curr_object = nullptr;
    bool is_pave_started = false;
    QPointF pave_point;

    bool block_creation = false;

    bool hero_created = false;

    void ClearItemsList();
};

#endif // LEVELCREATORCONTROLLER_H
