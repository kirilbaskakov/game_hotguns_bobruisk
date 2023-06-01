#ifndef BOT_H
#define BOT_H

#include "hero.h"

class Bot : public Hero
{
public:
    Bot();

    void SetMainHero(Hero* hero);
    Hero* GetMainHero();

    void Aim();

    void SetInitialPoint(QPointF point);

private:
    QPointF initial_point;

    Hero* main_hero = nullptr;
};

#endif // BOT_H
