// CCircleLayer.h
#ifndef CCIRCLE_LAYER_H
#define CCIRCLE_LAYER_H

#include "Layer.h"

class CCircleLayer : public Layer {
public:
    CCircleLayer(const QString& type = "CCircleLayer", bool status = true);

    void setParameters(const QMap<QString, QVariant>& params) override;
    QMap<QString, QVariant> getParameters() const override;
    void drawLayer(QPainter& painter, bool isThumbnail = false) const override;
    void outputLayerInfo() const override;

private:
    QMap<QString, QVariant> layerParams;  // 用于存储 CRectLayer 的参数
};

#endif // CCIRCLE_LAYER_H
