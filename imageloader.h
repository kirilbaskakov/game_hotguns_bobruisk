#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include <QString>
#include <QHash>
#include <QImage>

class ImageLoader
{
public:
    ImageLoader();

    static void LoadImage(const QString& filename);

    static QImage& GetImage(const QString& filename);

    static void Clear();
private:
    static QHash<QString, QImage> images;
};

#endif // IMAGELOADER_H
