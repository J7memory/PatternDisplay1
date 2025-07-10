// MonoLayer.h
#ifndef MONO_LAYER_H
#define MONO_LAYER_H

#include "Layer.h"

class MonoLayer : public Layer {
public:
    MonoLayer(const QString& type = "MonoLayer", bool status = true);

    void setParameters(const QMap<QString, QVariant>& params) override;
    QMap<QString, QVariant> getParameters() const override;
    void drawLayer(QPainter& painter, bool isThumbnail = false) const override;
    void outputLayerInfo() const override;

private:
    QMap<QString, QVariant> layerParams;  // ���ڴ洢 MonoLayer �Ĳ���
};

#endif // MONO_LAYER_H
