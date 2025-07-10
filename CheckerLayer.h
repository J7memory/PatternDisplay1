// CheckerLayer.h
#ifndef CHECKER_LAYER_H
#define CHECKER_LAYER_H

#include "Layer.h"

class CheckerLayer : public Layer {
public:
    CheckerLayer(const QString& type = "CheckerLayer", bool status = true);

    void setParameters(const QMap<QString, QVariant>& params) override;
    QMap<QString, QVariant> getParameters() const override;
    void drawLayer(QPainter& painter, bool isThumbnail = false) const override;
    void outputLayerInfo() const override;

private:
    QMap<QString, QVariant> layerParams;  // 用于存储 CheckerLayer 的参数
};

#endif // CHECKER_LAYER_H
