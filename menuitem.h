#ifndef MENUITEM_H
#define MENUITEM_H

#include "gamingobject.h"

class MenuItem : public GamingObject
{
    Q_OBJECT
public:
    MenuItem(QString text, int font_size);
    MenuItem(Pixmap pixmap);

    virtual void Clicked() override;
    virtual void Hover() override;

signals:
    void ClickedSignal(MenuItem* item);
};

#endif // MENUITEM_H
