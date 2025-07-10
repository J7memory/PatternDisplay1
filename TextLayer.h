// TextLayer.h
#ifndef TEXT_LAYER_H
#define TEXT_LAYER_H

#include "Layer.h"

class TextLayer : public Layer {
public:
    TextLayer(const QString& type = "Textlayer", bool status = true);

    void setParameters(const QMap<QString, QVariant>& params) override;
    QMap<QString, QVariant> getParameters() const override;
    void drawLayer(QPainter& painter, bool isThumbnail = false) const override;
    void outputLayerInfo() const override;

private:
    QMap<QString, QVariant> layerParams; 
};

#endif // TEXT_LAYER_H
