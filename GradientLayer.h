// GradientLayer.h
#ifndef GRADIENT_LAYER_H
#define GRADIENT_LAYER_H

#include "Layer.h"
#include <QList>
#include <QMap>
#include <QVariant>

class GradientLayer : public Layer {
public:
    GradientLayer(const QString& type = "GradientLayer", bool status = true);

    void setParameters(const QMap<QString, QVariant>& params) override;
    QMap<QString, QVariant> getParameters() const override;
    void drawLayer(QPainter& painter, bool isThumbnail = false) const override;
    void outputLayerInfo() const override;

private:
    QMap<QString, QVariant> layerParams;
    QList<QMap<QString, QVariant>> gradientBars;

    void initializeGradientBars(int count);

    QList<QRect> divideGradientRegion(int width, int height, int counts,
        const QList<int>& stepsPerRegion,
        const QString& orientation,
        int startX, int startY) const;
};

#endif // GRADIENT_LAYER_H
