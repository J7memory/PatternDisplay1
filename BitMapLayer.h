// BitMapLayer.h
#ifndef BITMAP_LAYER_H
#define BITMAP_LAYER_H

#include "Layer.h"

class BitMapLayer : public Layer {
public:
    BitMapLayer(const QString& type = "BitMapLayer", bool status = true);

    void setParameters(const QMap<QString, QVariant>& params) override;
    QMap<QString, QVariant> getParameters() const override;
    void drawLayer(QPainter& painter, bool isThumbnail = false) const override;
    void outputLayerInfo() const override;

private:
    QMap<QString, QVariant> layerParams;  // 用于存储 BitMapLayer 的参数
    QList<QColor> pixelColors;
};

#endif // BITMAP_LAYER_H
