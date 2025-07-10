// CRectLayer.h
#ifndef CRECT_LAYER_H
#define CRECT_LAYER_H

#include "Layer.h"

class CRectLayer : public Layer {
public:
    CRectLayer(const QString& type = "CRectLayer", bool status = true);

    void setParameters(const QMap<QString, QVariant>& params) override;
    QMap<QString, QVariant> getParameters() const override;
    void drawLayer(QPainter& painter, bool isThumbnail = false) const override;
    void outputLayerInfo() const override;

private:
    QMap<QString, QVariant> layerParams;  // 用于存储 CRectLayer 的参数
};

#endif // CRECT_LAYER_H
