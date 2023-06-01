#include "bot.h"

Bot::Bot()
{

}

void Bot::SetMainHero(Hero *hero)
{
    main_hero = hero;
}

Hero *Bot::GetMainHero()
{
    return main_hero;
}

void Bot::Aim()
{
    double angle = atan2(main_hero->GetCenter().y() - GetCenter().y(), main_hero->GetCenter().x() - GetCenter().x()) / 3.14159 * 180;
    SetAngle(angle);
}

void Bot::SetInitialPoint(QPointF point)
{
    initial_point = point;
}
