#ifndef MENU_H
#define MENU_H

#include "menuitem.h"

class Menu
{
public:
    Menu();

    void SetFontSize(int new_font_size);

    void SetPosition(QPointF new_position);

    void SetSpacing(int new_spacing);

    void SetIsCentered(bool new_is_centered);

    void SetIsHorizontal(bool new_is_horizontal);

    void SetZValue(int new_z_value);

    void CreateBackground();

    void AddMenu(QString text);
    void AddMenu(MenuItem* menu_item);
    MenuItem* GetMenu(int index);

    void Clear();

    QList<GamingObject*> GetObjects();

private:
    QList<MenuItem*> menu_items;
    GamingObject* background = nullptr;

    bool is_centered = false;

    bool is_horizontal = false;

    int font_size = 50;

    int z_value = 100;

    QPointF position = QPointF(0, 0);
    QPointF curr_position = position;

    int spacing = 0;

};

#endif // MENU_H
