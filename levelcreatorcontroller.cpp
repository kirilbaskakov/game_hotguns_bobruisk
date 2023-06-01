#include "levelcreatorcontroller.h"

LevelCreatorController::LevelCreatorController(QSize window_size)
{
    this->window_size = window_size;

    menu_items_types.SetFontSize(30);
    menu_items_types.SetPosition(QPointF(50, window_size.height() / 2 - 150));
    menu_items_types.SetSpacing(5);

    menu_items_types.AddMenu("walls");
    connect(menu_items_types.GetMenu(0), &MenuItem::ClickedSignal, this, &LoadWalls);

    menu_items_types.AddMenu("floor");
    connect(menu_items_types.GetMenu(1), &MenuItem::ClickedSignal, this, &LoadFloor);

    menu_items_types.AddMenu("hero");
    connect(menu_items_types.GetMenu(2), &MenuItem::ClickedSignal, this, &LoadHero);

    menu_items_types.AddMenu("decorations");
    connect(menu_items_types.GetMenu(3), &MenuItem::ClickedSignal, this, &LoadDecorations);

    menu_items_types.CreateBackground();

    menu_items_list.SetPosition(QPointF(window_size.width() / 2 - 727, window_size.height() - 190));
    menu_items_list.SetSpacing(10);
    menu_items_list.SetIsHorizontal(true);
    for (int i = 0; i < 7; i++) {
        MenuItem* item = new MenuItem(Pixmap(200, 120));
        connect(item, &MenuItem::ClickedSignal, this, &SelectTexture);
        menu_items_list.AddMenu(item);
    }
    menu_items_list.CreateBackground();

    menu_back.SetFontSize(30);
    menu_back.SetPosition(QPointF(50, window_size.height() - 190));
    menu_back.AddMenu("back");
    connect(menu_back.GetMenu(0), &MenuItem::ClickedSignal, this, &Close);
    menu_back.CreateBackground();

    menu_save.SetFontSize(30);
    menu_save.SetPosition(QPointF(window_size.width() - 180, window_size.height() - 190));
    menu_save.AddMenu("save");
    connect(menu_save.GetMenu(0), &MenuItem::ClickedSignal, this, &Save);
    menu_save.CreateBackground();

    central_object = new Entity();
    central_object->SetPixmap(Pixmap(2, 2));
    central_object->SetPosition(QPointF(window_size.width() / 2, window_size.height() / 2));
    central_object->SetSpeed(500);
}

void LevelCreatorController::KeyPressed(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_S:
       central_object->SetMovingOnY(-1);
       central_object->SetIsMoving(true);
       break;
    case Qt::Key_W:
       central_object->SetMovingOnY(1);
       central_object->SetIsMoving(true);
       break;
    case Qt::Key_A:
       central_object->SetMovingOnX(-1);
       central_object->SetIsMoving(true);
       break;
    case Qt::Key_D:
       central_object->SetMovingOnX(1);
       central_object->SetIsMoving(true);
       break;
    }
}

void LevelCreatorController::KeyReleased(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_S:
        central_object->SetMovingOnY(0);
        break;
    case Qt::Key_W:
        central_object->SetMovingOnY(0);
        break;
    case Qt::Key_A:
        central_object->SetMovingOnX(0);
        break;
    case Qt::Key_D:
        central_object->SetMovingOnX(0);
        break;
    }
}

void LevelCreatorController::MouseClick(int x, int y, Qt::MouseButton button)
{
    if (!selected_item) {
        return;
    }
    if (block_creation) {
        block_creation = false;
        return;
    }
    if (!is_pave_started && (selected_item_type == WALL || selected_item_type == FLOOR)) {
        is_pave_started = true;
        pave_point = QPointF(x, y);
        return;
    }
    is_pave_started = false;
    if (selected_item_type == HERO) {
        if (!hero_created) {
            Hero* hero = new Hero();
            hero->SetPosition(curr_object->GetPosition());
            hero->SetPixmap(curr_object->GetPixmap());
            hero->SetAngle(curr_object->GetAngle());
            hero->SetZValue(curr_object->GetZValue());
            hero->SetIsObstacle(true);
            hero->SetIsVisibleThrough(false);
            hero->SetSpeed(500);
            hero_created = true;
            level.SetMainHero(hero);
        } else {
            Bot* bot = new Bot();
            bot->SetPosition(curr_object->GetPosition());
            bot->SetInitialPoint(curr_object->GetPosition());
            bot->SetPixmap(curr_object->GetPixmap());
            bot->SetAngle(curr_object->GetAngle());
            bot->SetZValue(curr_object->GetZValue());
            bot->SetIsObstacle(true);
            bot->SetIsVisibleThrough(false);
            bot->SetSpeed(500);
            level.AddBot(bot);
        }
    } else {
        level.AddObject(curr_object);
    }
    curr_object = nullptr;
}

void LevelCreatorController::MouseMove(int x, int y)
{
    if (!selected_item) {
        return;
    }
    if (curr_object == nullptr) {
        curr_object = new GamingObject();
        emit ObjectCreated(curr_object, false);
    }
    Pixmap pixmap;
    int object_width = selected_item->GetPixmap().width();
    int object_height = selected_item->GetPixmap().height();
    pave_point.setX(int(pave_point.x() + (object_width - int(pave_point.x()) % object_width) % object_width));
    pave_point.setY(int(pave_point.y() + (object_height - int(pave_point.y()) % object_height) % object_height));
    switch (selected_item_type) {
    case WALL:
        curr_object->SetIsVisibleThrough(false);
        curr_object->SetIsObstacle(true);
        curr_object->SetZValue(2);
        x += (object_width - x % object_width) % object_width;
        y += (object_height - y % object_height) % object_height;
        if (is_pave_started) {
            int width = qMax(qreal(object_width), qAbs(x - pave_point.x()));
            int height = qMax(qreal(object_height), qAbs(y - pave_point.y()));
            if (x < pave_point.x()) {
                width += object_width;
                curr_object->SetPosition(QPointF(x, curr_object->GetPosition().y()));
            } else {
                curr_object->SetPosition(QPointF(pave_point.x(), curr_object->GetPosition().y()));
            }
            if (y < pave_point.y()) {
                height += object_height;
                curr_object->SetPosition(QPointF(curr_object->GetPosition().x(), y));
            } else {
                curr_object->SetPosition(QPointF(curr_object->GetPosition().x(), pave_point.y()));
            }
            pixmap.PaveByPixmap(selected_item->GetPixmap(), width, height);
        } else {
            pixmap = selected_item->GetPixmap();
            curr_object->SetPosition(QPointF(x, y));
        }
        break;
    case FLOOR:
        curr_object->SetIsVisibleThrough(true);
        curr_object->SetIsObstacle(false);
        curr_object->SetZValue(0);
        x += (object_width - x % object_width) % object_width;
        y += (object_height - y % object_height) % object_height;
        if (is_pave_started) {
            int width = qMax(qreal(object_width), qAbs(x - pave_point.x()));
            int height = qMax(qreal(object_height), qAbs(y - pave_point.y()));
            if (x < pave_point.x()) {
                width += object_width;
                curr_object->SetPosition(QPointF(x, curr_object->GetPosition().y()));
            } else {
                curr_object->SetPosition(QPointF(pave_point.x(), curr_object->GetPosition().y()));
            }
            if (y < pave_point.y()) {
                height += object_height;
                curr_object->SetPosition(QPointF(curr_object->GetPosition().x(), y));
            } else {
                curr_object->SetPosition(QPointF(curr_object->GetPosition().x(), pave_point.y()));
            }
            pixmap.PaveByPixmap(selected_item->GetPixmap(), width, height);
        } else {
            pixmap = selected_item->GetPixmap();
            curr_object->SetPosition(QPointF(x, y));
        }
        break;
    case HERO: case DECORATION:
        curr_object->SetIsVisibleThrough(false);
        curr_object->SetZValue(1);
        curr_object->SetPosition(QPointF(x, y));
        curr_object->SetIsObstacle(true);
        pixmap = selected_item->GetPixmap();
        break;
    }
    curr_object->SetPixmap(pixmap);
}

void LevelCreatorController::Wheel(int delta, Qt::Orientation orientation)
{
    if (!selected_item || selected_item_type == WALL || selected_item_type == FLOOR || !curr_object) {
        return;
    }
    curr_object->Rotate(delta / 10);
}

QList<GamingObject *> LevelCreatorController::GetObjects()
{
    QList<GamingObject *> objects;
    for (auto object : menu_items_types.GetObjects()) {
        objects.push_back(object);
    }
    for (auto object : menu_items_list.GetObjects()) {
        objects.push_back(object);
    }
    for (auto object : menu_back.GetObjects()) {
        objects.push_back(object);
    }
    for (auto object : menu_save.GetObjects()) {
        objects.push_back(object);
    }
    return objects;
}

Entity *LevelCreatorController::GetCentralObject()
{
    return central_object;
}

void LevelCreatorController::LoadWalls()
{
    ClearItemsList();
    selected_item_type = WALL;
    menu_items_list.GetMenu(0)->SetPixmap(Pixmap(":/textures/walls/wall_brick.png", 25, 25));
    menu_items_list.GetMenu(1)->SetPixmap(Pixmap(":/textures/walls/wall_black.png", 25, 25));
    menu_items_list.GetMenu(2)->SetPixmap(Pixmap(":/textures/walls/wall_yellow.png", 25, 25));
}

void LevelCreatorController::LoadFloor()
{
    ClearItemsList();
    selected_item_type = FLOOR;
    menu_items_list.GetMenu(0)->SetPixmap(Pixmap(":/textures/floor/floor_wood_light.png", 100, 25));
    menu_items_list.GetMenu(1)->SetPixmap(Pixmap(":/textures/floor/floor_wood_brown.png", 100, 25));
    menu_items_list.GetMenu(2)->SetPixmap(Pixmap(":/textures/floor/floor_wood_white.png", 100, 25));
    menu_items_list.GetMenu(3)->SetPixmap(Pixmap(":/textures/floor/floor_tile_white.png", 25, 25));
    menu_items_list.GetMenu(4)->SetPixmap(Pixmap(":/textures/floor/floor_tile_gold.png", 25, 25));
}

void LevelCreatorController::LoadHero()
{
    ClearItemsList();
    selected_item_type = HERO;
    menu_items_list.GetMenu(0)->SetPixmap(Pixmap(":/textures/hero/hero.png", 80, 40));
}

void LevelCreatorController::LoadDecorations()
{
    ClearItemsList();
    selected_item_type = DECORATION;
    menu_items_list.CreateBackground();
    menu_items_list.GetMenu(0)->SetPixmap(Pixmap(":/textures/decorations/bed_table.png", 40, 40));
    menu_items_list.GetMenu(1)->SetPixmap(Pixmap(":/textures/decorations/table.png", 200, 120));
    menu_items_list.GetMenu(2)->SetPixmap(Pixmap(":/textures/decorations/sink.png", 70, 70));
    menu_items_list.GetMenu(3)->SetPixmap(Pixmap(":/textures/decorations/toilet.png", 70, 70));
    menu_items_list.GetMenu(4)->SetPixmap(Pixmap(":/textures/decorations/trash.png", 50, 50));
  //  menu_items_list.GetMenu(5)->SetPixmap(Pixmap(":/textures/decorations/floor_wood_brown2.png", 100, 25));
}

void LevelCreatorController::SelectTexture(MenuItem *item)
{
    selected_item = item;
    block_creation = true;
}

void LevelCreatorController::Close()
{
    emit Closed();
}

void LevelCreatorController::Save()
{
    block_creation = true;
    QString filepath = QFileDialog::getSaveFileName(nullptr, "", "", "*.json");
    if (!filepath.isEmpty()) {
        level.SaveToFile(filepath);
    }
}

void LevelCreatorController::ClearItemsList()
{
    if (curr_object) {
        emit ObjectRemoved(curr_object);
        curr_object = nullptr;
    }
    selected_item = nullptr;
    is_pave_started = false;
    for (int i = 0; i < 7; i++) {
        menu_items_list.GetMenu(i)->SetPixmap(Pixmap(200, 120));
    }
}
