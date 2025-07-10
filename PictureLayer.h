// PictureLayer.h
#ifndef PICTURE_LAYER_H
#define PICTURE_LAYER_H

#include "Layer.h"
#include <QPixmap>
#include <QMap>
#include <QVariant>
#include <QPainter>
#include <QString>

class PictureLayer : public Layer {
public:
    PictureLayer(const QString& type = "PictureLayer", bool status = true);

    void setParameters(const QMap<QString, QVariant>& params) override;
    QMap<QString, QVariant> getParameters() const override;
    void drawLayer(QPainter& painter, bool isThumbnail = false) const override;
    void outputLayerInfo() const override;

    //void setPixmap(const QPixmap& newPixmap);
    QPixmap getPixmap() const;

private:
    QPixmap pixmap;
    QMap<QString, QVariant> layerParams; 
    void loadImage(const QString& path);
};

#endif // PICTURE_LAYER_H