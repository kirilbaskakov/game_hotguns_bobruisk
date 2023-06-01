#include "pixmap.h"

Pixmap::Pixmap()
{

}

Pixmap::Pixmap(int width, int height) : QPixmap(width, height)
{
    fill(QColor(0, 0, 0, 0));
}

Pixmap::Pixmap(QString filename, int width, int height, double rotation)
{
    LoadFromFile(filename, width, height, rotation);
}

Pixmap::Pixmap(const Pixmap &pixmap, int width, int height)
{
    PaveByPixmap(pixmap, width, height);
}

void Pixmap::LoadFromFile(QString filename, int width, int height, double rotation)
{
    QImage image = ImageLoader::GetImage(filename);
    image = image.scaled(width, height);
    QTransform rotate_transform;
    rotate_transform = rotate_transform.rotate(rotation);
    image = image.transformed(rotate_transform);
    QPixmap::convertFromImage(image);

    QJsonObject json;
    json.insert("filename", filename);
    json.insert("width", width);
    json.insert("height", height);
    pixmaps.append(json);
}

void Pixmap::PaveByPixmap(const Pixmap& pixmap, int width, int height)
{
    is_tiled = true;
    pixmaps.append(pixmap.ToJson());

    QImage image = pixmap.toImage();
    QImage image_total(width, height, image.format());

    int bytes_width = image.byteCount() / (image.width() * image.height()) * image.width();
    int bytes_total_width = image_total.byteCount() / (width * height) * width;
    int rows = height / pixmap.height();
    int cols = width / pixmap.width();
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            for (int i1 = 0; i1 < bytes_width; i1++) {
                for (int j1 = 0; j1 < image.height(); j1++) {
                    int pos_x = i * image.height() + j1;
                    int pos_y = j * bytes_width + i1;
                    image_total.bits()[pos_x * bytes_total_width + pos_y] = image.bits()[j1 * bytes_width + i1];
                }

            }
        }
    }

    QPixmap::convertFromImage(image_total);
}

void Pixmap::PaveByPixmaps(QList<Pixmap> pixmaps)
{
    int total_width = 0;
    int total_height = 0;
    for (const Pixmap& pixmap : pixmaps) {
        this->pixmaps.append(pixmap.ToJson());
        total_width += pixmap.width();
        total_height = pixmap.height();
    }
    QImage image_total(total_width, total_height, pixmaps[0].toImage().format());
    int total_bytes_width = image_total.byteCount() / (total_width * total_height) * total_width;
    int x = 0;
    int y = 0;
    for (const Pixmap& pixmap : pixmaps) {
        QImage image = pixmap.toImage();
        int bytes_width = image.byteCount() / (image.width() * image.height()) * image.width();
        for (int i = 0; i < bytes_width; i++) {
            for (int j = 0; j < image.height(); j++) {
                image_total.bits()[(y + j) * total_bytes_width + x + i] = image.bits()[j * bytes_width + i];
            }
        }
        x += bytes_width;
    }

    QPixmap::convertFromImage(image_total);
}

void Pixmap::FromText(QString text, int width, int height)
{
    QList<Pixmap> pixmaps;
    for (int i = 0; i < text.length(); i++) {
        if (text[i] == ' ') {
            Pixmap pixmap(width, height);
            pixmap.fill(QColor(0, 0, 0, 0));
            pixmaps.append(pixmap);
            continue;
        }
        QString filename = ":/textures/font/";
        filename += text[i].toLower();
        filename += ".png";
        pixmaps.append(Pixmap(filename, width, height));
    }
    PaveByPixmaps(pixmaps);
}

QJsonObject Pixmap::ToJson() const
{
    QJsonObject json;
    json.insert("is_tiled", is_tiled);
    json.insert("width", width());
    json.insert("height", height());
    json.insert("pixmaps_amount", pixmaps.length());
    for (int i = 0; i < pixmaps.length(); i++) {
        json.insert("pixmap" + QString::number(i), pixmaps[i]);
    }
    return json;
}

void Pixmap::FromJson(const QJsonObject &json)
{
    if (json.find("filename") != json.end()) {
        LoadFromFile(json["filename"].toString(), json["width"].toInt(), json["height"].toInt());
        return;
    }
    int pixmaps_amount = json["pixmaps_amount"].toInt();
    QList<Pixmap> pixmaps;
    for (int i = 0; i < pixmaps_amount; i++) {
        Pixmap pixmap;
        pixmap.FromJson(json["pixmap" + QString::number(i)].toObject());
        pixmaps.append(pixmap);
    }
    if (json["is_tiled"].toBool()) {
        PaveByPixmap(pixmaps[0], json["width"].toInt(), json["height"].toInt());
    } else {
        PaveByPixmaps(pixmaps);
    }
}
