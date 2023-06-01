#include "controller.h"

Controller::Controller(QSize size, QWidget* parent)
{
    view = new View(parent);

    QObject::connect(view, &View::MouseMovedSignal, this, &HandleMouseMoveEvent);
    QObject::connect(view, &View::MousePressedSignal, this, &HandleMouseClickEvent);
    QObject::connect(view, &View::WheelSignal, this, &HandleWheelEvent);

    window_size = size;

    LoadMainMenu();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &UpdateScene);
    timer->start(20);
}

void Controller::Clear()
{
    view->ClearGamingObjects();
    for (GamingObject* object : all_objects.values()) {
        delete object;
    }
    all_objects.clear();
    moving_objects.clear();
    bots.clear();
}

void Controller::LoadLevelCreator()
{
    Clear();

    level_creator = new LevelCreatorController(window_size);
    connect(level_creator, &LevelCreatorController::ObjectCreated, this, CreateObject);
    connect(level_creator, &LevelCreatorController::ObjectRemoved, this, RemoveObject);
    connect(level_creator, &LevelCreatorController::Closed, this, LoadMainMenu);
    CreateObjects(level_creator->GetObjects(), true);
    CreateMovingObject(level_creator->GetCentralObject());
    view->SetCentralObject(level_creator->GetCentralObject());
    mode = CREATING_LEVEL;
}

void Controller::LoadMainMenu()
{
    Clear();

    mode = IN_MENU;

    GamingObject* background = new GamingObject();
    background->SetPixmap(Pixmap(":/textures/menu_background.jpg", window_size.width(), window_size.height()));
    background->SetPosition(QPointF(0, 0));
    background->SetIsObstacle(false);
    CreateObject(background);
    view->SetCentralObject(background);

    Menu menu;
    menu.SetFontSize(70);
    menu.SetPosition(QPointF(window_size.width() / 2, 450));
    menu.SetIsCentered(true);
    menu.SetSpacing(50);

    menu.AddMenu("Select level");
    connect(menu.GetMenu(0), &MenuItem::ClickedSignal, this, &LoadLevelSelection);

    menu.AddMenu("Create level");
    connect(menu.GetMenu(1), &MenuItem::ClickedSignal, this, &LoadLevelCreator);

    menu.AddMenu("Exit");
    connect(menu.GetMenu(2), &MenuItem::ClickedSignal, this, &Exit);

    CreateObjects(menu.GetObjects());
}

void Controller::Exit()
{
    exit(0);
}

void Controller::LoadLevelSelection()
{
    QString filename = QFileDialog::getOpenFileName(nullptr, "", "", "*.json");
    if (filename.isEmpty()) {
        return;
    }
    mode = LOADING;

    Clear();

    Level level;
    level.LoadFromFile(filename);
    level_player = new LevelPlayController;
    connect(level_player, LevelPlayController::ObjectCreated, this, CreateObject);
    connect(level_player, LevelPlayController::MovingObjectCreated, this, CreateMovingObject);
    connect(level_player, LevelPlayController::ObjectRemoved, this, RemoveObject);
    connect(level_player, LevelPlayController::BotCreated, this, CreateBot);
    connect(level_player, LevelPlayController::Closed, this, LoadMainMenu);
    level_player->LoadLevel(level);
    view->SetCentralObject(level_player->GetCentralObject());
    mode = PLAYING_LEVEL;
}

void Controller::HandleKeyPressEvent(QKeyEvent *event)
{
    switch (mode) {
    case CREATING_LEVEL:
        level_creator->KeyPressed(event);
        break;
    case PLAYING_LEVEL:
        level_player->KeyPressed(event);
        break;
    }
}

void Controller::HandleKeyReleaseEvent(QKeyEvent *event)
{
    switch (mode) {
    case CREATING_LEVEL:
        level_creator->KeyReleased(event);
        break;
    case PLAYING_LEVEL:
        level_player->KeyReleased(event);
        break;
    }
}

void Controller::CreateObject(GamingObject *object, bool is_static)
{
    all_objects[object->GetId()] = object;
    view->AddGamingObject(object, is_static);
}

void Controller::CreateObjects(QList<GamingObject *> objects, bool is_static)
{
    for (auto object : objects) {
        CreateObject(object, is_static);
    }
}

void Controller::CreateMovingObject(Entity *entity)
{
    CreateObject(entity);
    moving_objects[entity->GetId()] = entity;
    connect(entity, &Entity::disappear, this, &RemoveObject);
}

void Controller::CreateBot(Bot *bot)
{
    CreateObject(bot);
    bots[bot->GetId()] = bot;
}

void Controller::HandleMouseMoveEvent(int x, int y)
{
    switch (mode) {
    case PLAYING_LEVEL:
        level_player->MouseMove(x, y);
        break;
    case CREATING_LEVEL:
        level_creator->MouseMove(x, y);
        break;
    }
}

void Controller::HandleMouseClickEvent(int x, int y, Qt::MouseButton button)
{
    switch (mode) {
    case PLAYING_LEVEL:
        level_player->MouseClick(x, y, button);
        break;
    case CREATING_LEVEL:
        level_creator->MouseClick(x, y, button);
        break;
    }
}

void Controller::HandleWheelEvent(int delta, Qt::Orientation orientation)
{
    switch (mode) {
    case CREATING_LEVEL:
        level_creator->Wheel(delta, orientation);
        break;
    }
}

void Controller::RemoveObject(GamingObject *object)
{
    all_objects.erase(all_objects.find(object->GetId()));
    moving_objects.erase(moving_objects.find(object->GetId()));
    view->RemoveGamingObject(object);
    delete object;
}

void Controller::UpdateScene()
{
    if (mode == PLAYING_LEVEL && level_player->IsInMenu()) {
        return;
    }
    // process bots
    for (Bot* bot : bots.values()) {
        if (bot->IsDead() || bot->GetMainHero()->IsDead()) {
            continue;
        }
        if (view->IsObjectVisible(bot->GetMainHero(), bot)) {
            bot->Aim();
            QList<Bullet*> bullets = bot->Shoot();
            for (auto bullet : bullets) {
                CreateMovingObject(bullet);
            }
        }
    }
    // move objects
    for (Entity* entity : moving_objects.values()) {
        if (entity->GetIsMoving()) {
            QPointF next_position = entity->NextPosition();
            if (!CheckCollisions(entity, next_position / 2) && !CheckCollisions(entity, next_position)) {
                entity->MakeMove();
                view->RepaintGamingObject(entity);
            }
        }
    }
    // repaint objects
    for (auto object : all_objects.values()) {
        view->RepaintGamingObject(object);
    }
}

bool Controller::CheckCollisions(Entity* entity, QPointF delta)
{
    QPointF current_pos = entity->GetPosition();
    entity->Move(delta);
    view->RepaintGamingObject(entity);
    QList<GamingObject*> colliding_objects = view->GetCollidingObjects(entity);
    entity->SetPosition(current_pos);
    int entity_id = entity->GetId();
    bool is_colliding = false;
    for (GamingObject* object : colliding_objects) {
        if (!object->GetIsObstacle()) {
            continue;
        }
        is_colliding = true;
        object->Collide(entity);
        view->RepaintGamingObject(object);
        entity->Collide(object);
        if (all_objects.count(entity_id)) {
            view->RepaintGamingObject(entity);
        } else {
            return true;
        }
    }
    return is_colliding;
}

View* Controller::GetView()
{
    return view;
}
