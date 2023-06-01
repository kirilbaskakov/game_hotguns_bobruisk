#include "levelplaycontroller.h"

LevelPlayController::LevelPlayController()
{
    menu = new Menu();
    menu->SetFontSize(60);
    menu->SetIsCentered(true);
    menu->SetSpacing(60);

    top_menu = new Menu();
    top_menu->SetFontSize(50);
}

void LevelPlayController::KeyPressed(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_S:
        if (!hero->IsDead() && !in_menu) {
            hero->SetMovingOnY(-1);
            hero->SetIsMoving(true);
        }
        break;
    case Qt::Key_W:
        if (!hero->IsDead() && !in_menu) {
            hero->SetMovingOnY(1);
            hero->SetIsMoving(true);
        }
        break;
    case Qt::Key_A:
        if (!hero->IsDead() && !in_menu) {
            hero->SetMovingOnX(-1);
            hero->SetIsMoving(true);
        }
        break;
    case Qt::Key_D:
        if (!hero->IsDead() && !in_menu) {
            hero->SetMovingOnX(1);
            hero->SetIsMoving(true);
        }
        break;
    case Qt::Key_R:
        if (hero->IsDead() && !in_menu) {
            LoadLevel(level, true);
        }
        break;
    case Qt::Key_Escape:
        if (in_menu) {
            CloseMenu();
        } else {
            ShowMenu();
        }
        break;
    }
}

void LevelPlayController::KeyReleased(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_S:
        hero->SetMovingOnY(0);
        break;
    case Qt::Key_W:
        hero->SetMovingOnY(0);
        break;
    case Qt::Key_A:
        hero->SetMovingOnX(0);
        break;
    case Qt::Key_D:
        hero->SetMovingOnX(0);
        break;
    }
}

void LevelPlayController::MouseClick(int x, int y, Qt::MouseButton button)
{
    if (hero->IsDead() || in_menu) {
        return;
    }
    if (button == Qt::LeftButton) {
        QList<Bullet*> bullets = hero->Shoot();
        for (auto bullet : bullets) {
            emit MovingObjectCreated(bullet);
        }
    }
}

void LevelPlayController::MouseMove(int x, int y)
{
    if (hero->IsDead() || in_menu) {
        return;
    }
    QPointF hero_pos = hero->GetCenter();
    double angle = atan2(y - hero_pos.y(), x - hero_pos.x());
    hero->SetAngle(angle * 180 / 3.14159);
}

bool LevelPlayController::IsInMenu()
{
    return in_menu;
}

QList<GamingObject *> LevelPlayController::GetObjects()
{

}

Entity *LevelPlayController::GetCentralObject()
{
    return hero;
}

void LevelPlayController::BotKilled()
{
    bots_alive--;
    if (bots_alive == 0) {
        top_menu->SetPosition(QPointF(hero->GetPosition().x() - 225, hero->GetPosition().y() - 400));
        top_menu->AddMenu("level clear");
        emit ObjectCreated(top_menu->GetMenu(0), true);
    }
}

void LevelPlayController::HeroKilled()
{
    top_menu->SetPosition(QPointF(hero->GetPosition().x() - 390, hero->GetPosition().y() - 400));
    top_menu->AddMenu("press r to restart");
    emit ObjectCreated(top_menu->GetMenu(0), true);
}

void LevelPlayController::Close()
{
    emit Closed();
}

void LevelPlayController::ShowMenu()
{
    in_menu = true;
    hero->SetIsMoving(false);
    menu->SetPosition(hero->GetPosition() - QPointF(-20, 50));
    menu->AddMenu("resume");
    connect(menu->GetMenu(0), MenuItem::ClickedSignal, this, CloseMenu);
    menu->AddMenu("menu");
    connect(menu->GetMenu(1), MenuItem::ClickedSignal, this, Close);
    menu->CreateBackground();
    emit ObjectCreated(menu->GetObjects()[0], true);
    emit ObjectCreated(menu->GetObjects()[1], true);
    emit ObjectCreated(menu->GetObjects()[2], true);
}

void LevelPlayController::CloseMenu()
{
    in_menu = false;
    emit ObjectRemoved(menu->GetObjects()[0]);
    emit ObjectRemoved(menu->GetObjects()[1]);
    emit ObjectRemoved(menu->GetObjects()[2]);
    menu->Clear();
}

void LevelPlayController::LoadLevel(Level &level, bool reload)
{
    if (!reload) {
        this->level = level;
    } else {
        emit ObjectRemoved(top_menu->GetMenu(0));
        top_menu->Clear();
    }

    QList<GamingObject*> level_objects = level.GetObjects();
    for (int i = 0; i < level_objects.length(); i++) {
        GamingObject* object;
        if (reload) {
            object = objects[i];
        } else {
            object = new GamingObject();
        }
        object->SetPixmap(level_objects[i]->GetPixmap());
        object->SetPosition(level_objects[i]->GetPosition());
        object->SetAngle(level_objects[i]->GetAngle());
        object->SetZValue(level_objects[i]->GetZValue());
        object->SetIsObstacle(level_objects[i]->GetIsObstacle());
        object->SetIsVisibleThrough(level_objects[i]->GetIsVisibleThrough());
        if (!reload) {
            objects.append(object);
            emit ObjectCreated(object, false);
        }
    }

    QList<Entity*> level_entities = level.GetEntities();
    for (int i = 0; i < level_entities.length(); i++) {
        Entity* entity;
        if (reload) {
            entity = entities[i];
        } else {
            entity = new Entity();
        }
        entity->SetPixmap(level_entities[i]->GetPixmap());
        entity->SetPosition(level_entities[i]->GetPosition());
        entity->SetAngle(level_entities[i]->GetAngle());
        entity->SetZValue(level_entities[i]->GetZValue());
        entity->SetIsObstacle(level_entities[i]->GetIsObstacle());
        entity->SetIsVisibleThrough(level_entities[i]->GetIsVisibleThrough());
        entity->SetSpeed(level_entities[i]->GetSpeed());
        if (!reload) {
            entities.append(entity);
            emit MovingObjectCreated(entity);
        }
    }

    Hero* level_hero = level.GetMainHero();
    if (!reload) {
        hero = new Hero;
        connect(hero, Hero::Dead, this, HeroKilled);
    }
    hero->SetPixmap(level_hero->GetPixmap());
    hero->SetPosition(level_hero->GetPosition());
    hero->SetAngle(level_hero->GetAngle());
    hero->SetZValue(level_hero->GetZValue());
    hero->SetIsObstacle(level_hero->GetIsObstacle());
    hero->SetIsVisibleThrough(level_hero->GetIsVisibleThrough());
    hero->SetSpeed(level_hero->GetSpeed());
    hero->SetIsDead(false);
    if (!reload) {
        emit MovingObjectCreated(hero);
    }

    QList<Bot*> level_bots = level.GetBots();
    bots_alive = level_bots.length();
    for (int i = 0; i < level_bots.length(); i++) {
        Bot* bot;
        if (reload) {
            bot = bots[i];
        } else {
            bot = new Bot();
            connect(bot, &Bot::Dead, this, BotKilled);
            bot->SetMainHero(hero);
        }
        bot->SetPixmap(level_bots[i]->GetPixmap());
        bot->SetPosition(level_bots[i]->GetPosition());
        bot->SetAngle(level_bots[i]->GetAngle());
        bot->SetZValue(level_bots[i]->GetZValue());
        bot->SetIsObstacle(level_bots[i]->GetIsObstacle());
        bot->SetIsVisibleThrough(level_bots[i]->GetIsVisibleThrough());
        bot->SetSpeed(level_bots[i]->GetSpeed());
        bot->SetIsDead(false);
        if (!reload) {
            bots.append(bot);
            emit BotCreated(bot);
        }
    }
}
