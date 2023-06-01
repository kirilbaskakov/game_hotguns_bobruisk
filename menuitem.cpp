#include "menuitem.h"

MenuItem::MenuItem(QString text, int font_size)
{
    Pixmap pixmap;
    pixmap.FromText(text, font_size, font_size);
    this->SetPixmap(pixmap);
}

MenuItem::MenuItem(Pixmap pixmap)
{
    this->SetPixmap(pixmap);
}

void MenuItem::Clicked()
{
    emit ClickedSignal(this);
}

void MenuItem::Hover()
{

}
