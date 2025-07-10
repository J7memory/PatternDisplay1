// Layer.cpp
#include "Layer.h"
#include "Pattern.h"

// Layer 基类构造函数实现
Layer::Layer(const QString& type, bool status)
    : layerType(type), layerStatus(status) {
}

// Layer 基类成员函数实现
QString Layer::getLayerType() const { return layerType; }
bool Layer::getLayerStatus() const { return layerStatus; }

void Layer::setLayerType(const QString& type) const { layerType = type; }
void Layer::setLayerStatus(bool status) const { layerStatus = status; }

void Layer::outputLayerInfo() const {
    qDebug() << "========================";
    qDebug() << "Layer Information:";
    qDebug() << "Layer Type:" << layerType;
    qDebug() << "Layer Status:" << (layerStatus ? "Visible" : "Hidden");

    // 获取并输出参数
    QMap<QString, QVariant> params = getParameters();
    qDebug() << "Layer Parameters:";
    if (params.isEmpty()) {
        qDebug() << "\t(No parameters set)";
    }
    else {
        for (auto it = params.begin(); it != params.end(); ++it) {
            qDebug() << "\t-" << it.key() << ": " << it.value().toString();
        }
    }
    qDebug() << "========================";
}