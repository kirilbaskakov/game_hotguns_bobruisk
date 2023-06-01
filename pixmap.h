#ifndef PIXMAP_H
#define PIXMAP_H

#include <QPixmap>
#include <QJsonObject>
#include <QString>
#include <imageloader.h>

class Pixmap : public QPixmap
{
public:
    Pixmap();
    Pixmap(int width, int height);
    Pixmap(QString filename, int width, int height, double rotation = 0);
    Pixmap(const Pixmap& pixmap, int width, int height);

    void LoadFromFile(QString filename, int width, int height, double rotation = 0);
    void PaveByPixmap(const Pixmap& pixmap, int width, int height);
    void PaveByPixmaps(QList<Pixmap> pixmaps);
    void FromText(QString text, int width, int height);

    QJsonObject ToJson() const;
    void FromJson(const QJsonObject& json);
private:
    QList<QJsonObject> pixmaps;
    bool is_tiled;
};

#endif // PIXMAP_H
