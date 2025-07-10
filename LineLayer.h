// LineLayer.h
#ifndef LINE_LAYER_H
#define LINE_LAYER_H

#include "Layer.h"

class LineLayer : public Layer {
public:
    LineLayer(const QString& type = "LineLayer", bool status = true);

    void setParameters(const QMap<QString, QVariant>& params) override;
    QMap<QString, QVariant> getParameters() const override;
    void drawLayer(QPainter& painter, bool isThumbnail = false) const override;
    void outputLayerInfo() const override;

private:
    QMap<QString, QVariant> layerParams;  // 用于存储 CRectLayer 的参数
};

#endif // LINE_LAYER_H
