// Pattern.cpp
#include "Pattern.h"
#include <QDebug>

// 构造函数实现
Pattern::Pattern(QString name, int width, int height, QString description)
    : patternName(name), patternWidth(width), patternHeight(height), patternDescription(description) {
}

// 向Pattern中添加Layer
void Pattern::addLayer(std::shared_ptr<Layer> layer) {
    layers.append(layer);
}

// 从 Pattern 中移除指定索引的 Layer
bool Pattern::removeLayer(int index) {
    if (index < 0 || index >= layers.size()) {
        return false;
    }

    layers.removeAt(index);
    return true;
}

void Pattern::updateLayerOrder(const QList<std::shared_ptr<Layer>>& newLayers) {
    layers = newLayers;
}

// 设置名称/宽度/高度/描述
void Pattern::setPatternName(const QString& name) { patternName = name; }
void Pattern::setPatternWidth(int width) { patternWidth = width; }
void Pattern::setPatternHeight(int height) { patternHeight = height; }
void Pattern::setPatternDescription(const QString& description) { patternDescription = description; }

// 获取图案的名称/宽度/高度/描述
QString Pattern::getPatternName() const { return patternName; }
int Pattern::getPatternWidth() const { return patternWidth; }
int Pattern::getPatternHeight() const { return patternHeight; }
QString Pattern::getPatternDescription() const { return patternDescription; }

// 获取所有图案的名称
QStringList Pattern::getAllPatternNames(const QVector<std::shared_ptr<Pattern>>& patterns) {
    QStringList patternNames;
    for (const auto& pattern : patterns) {
        if (pattern) {
            patternNames.append(pattern->getPatternName());
        }
    }
    return patternNames;
}

// 获取所有图层
QVector<std::shared_ptr<Layer>> Pattern::getLayers() const {
    return layers; 
}

int Pattern::getWidth() const {
    return patternWidth;
}

int Pattern::getHeight() const {
    return patternHeight;
}

// 按索引查找图层
std::shared_ptr<Layer> Pattern::getLayer(int index) {
    if (index >= 0 && index < layers.size()) {
        return layers[index];
    }
    return nullptr;
}

// 修改Layer类型
void Pattern::replaceLayer(int index, std::shared_ptr<Layer> newLayer) {
    if (index >= 0 && index < layers.size()) {
        layers[index] = newLayer;
    }
}

void Pattern::drawPattern(QPainter& painter, bool isThumbnail) {
    for (const auto& layer : layers) {
        if (layer) {
            if (layer && layer->getLayerStatus()) {
                layer->drawLayer(painter, isThumbnail);
            }
        }
    }
}

// 打印所有图案的详细信息
void Pattern::printInfo() const {
    qDebug() << "Pattern Info:";
    qDebug() << "\tName: " << patternName;
    qDebug() << "\tWidth: " << patternWidth;
    qDebug() << "\tHeight: " << patternHeight;
    qDebug() << "\tDescription: " << patternDescription;
    qDebug() << "\t\tLayer Count: " << layers.size();

    for (const auto& layer : layers) {
        if (layer) {
            layer->outputLayerInfo();
        }
    }
}
