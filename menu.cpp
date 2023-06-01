#include "menu.h"

Menu::Menu()
{

}

void Menu::SetFontSize(int new_font_size)
{
    font_size = new_font_size;
}

void Menu::SetPosition(QPointF new_position)
{
    curr_position = position = new_position;
}

void Menu::SetSpacing(int new_spacing)
{
    spacing = new_spacing;
}

void Menu::SetIsCentered(bool new_is_centered)
{
    is_centered = new_is_centered;
}

void Menu::SetIsHorizontal(bool new_is_horizontal)
{
    is_horizontal = new_is_horizontal;
}

void Menu::SetZValue(int new_z_value)
{
    z_value = new_z_value;
}

void Menu::CreateBackground()
{
    background = new GamingObject();
    background->SetZValue(-1);
    QPointF start = QPointF(1e9, 1e9);
    QPointF end = QPointF(-1e9, -1e9);
    for (auto menu : menu_items) {
        start.setX(qMin(start.x(), menu->GetPosition().x() - 15));
        start.setY(qMin(start.y(), menu->GetPosition().y() - 15));
        end.setX(qMax(end.x(), menu->GetPosition().x() + menu->GetPixmap().width() + 15));
        end.setY(qMax(end.y(), menu->GetPosition().y() + menu->GetPixmap().height() + 15));
    }
    background->SetPosition(start);
    background->SetPixmap(Pixmap(":/textures/background.png", end.x() - start.x(), end.y() - start.y()));
    background->SetZValue(z_value - 1);
}

void Menu::AddMenu(QString text)
{
    AddMenu(new MenuItem(text, font_size));
}

void Menu::AddMenu(MenuItem *menu_item)
{
    menu_item->SetPosition((is_centered ? QPointF(curr_position.x() - menu_item->GetPixmap().width() / 2, curr_position.y()) : curr_position));
    menu_item->SetZValue(z_value);
    if (is_horizontal) {
        curr_position.setX(curr_position.x() + menu_item->GetPixmap().width() + spacing);
    } else {
        curr_position.setY(curr_position.y() + menu_item->GetPixmap().height() + spacing);
    }
    menu_items.append(menu_item);
}

MenuItem *Menu::GetMenu(int index)
{
    return menu_items[index];
}

void Menu::Clear()
{
    curr_position = position;
    menu_items.clear();
}

QList<GamingObject *> Menu::GetObjects()
{
    QList<GamingObject*> objects;
    for (auto item : menu_items) {
        objects.append(dynamic_cast<GamingObject*> (item));
    }
    if (background) {
        objects.append(background);
    }
    return objects;
}
