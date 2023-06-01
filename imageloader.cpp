#include "imageloader.h"

QHash<QString, QImage> ImageLoader::images;

ImageLoader::ImageLoader()
{

}

void ImageLoader::LoadImage(const QString &filename)
{
    if (images.count(filename)) {
        return;
    }
    images[filename] = QImage(filename);
}

QImage& ImageLoader::GetImage(const QString &filename)
{
    if (!images.count(filename)) {
        LoadImage(filename);
    }
    return images[filename];
}

void ImageLoader::Clear()
{
    images.clear();
}
