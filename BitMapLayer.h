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
    QMap<QString, QVariant> layerParams;  // ���ڴ洢 BitMapLayer �Ĳ���
    QList<QColor> pixelColors;
};

#endif // BITMAP_LAYER_H
