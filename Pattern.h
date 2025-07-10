// Pattern.h
#ifndef PATTERN_H
#define PATTERN_H

#include <QString>
#include <QVector>
#include <memory>
#include <QPainter>
#include "Layer.h"

class Pattern {
public:
    // 构造函数
    Pattern(QString name, int width, int height, QString description = "");

    // 向Pattern中添加Layer
    void addLayer(std::shared_ptr<Layer> layer);

    // 从Pattern中移除索引项的Layer
    bool removeLayer(int index);

    // 更新layer索引
    void updateLayerOrder(const QList<std::shared_ptr<Layer>>& newLayers);

    // 设置pattern名称/宽度/高度/描述
    void setPatternName(const QString& name);
    void setPatternWidth(int width);
    void setPatternHeight(int height);
    void setPatternDescription(const QString& description);

    // 获取pattern名称/宽度/高度/描述
    QString getPatternName() const;
    int getPatternWidth() const;
    int getPatternHeight() const;
    QString getPatternDescription() const;

    // 获取所有图案的名称
    QStringList getAllPatternNames(const QVector<std::shared_ptr<Pattern>>& patterns);

    // 获取图案包含的所有图层
    QVector<std::shared_ptr<Layer>> getLayers() const;

    // 获取某个Layer的详细信息
    std::shared_ptr<Layer> getLayer(int index);

    // 根据索引修改Layer类型
    void replaceLayer(int index, std::shared_ptr<Layer> newLayer);

    // 绘制图案
    void drawPattern(QPainter& painter, bool isThumbnail = false);

    // 打印图案信息
    void printInfo() const;

    int getWidth() const;
    int getHeight() const;

    QVector<std::shared_ptr<Layer>> layers;  // 存储多个图层

private:
    QString patternName;
    int patternWidth;
    int patternHeight;
    QString patternDescription;
};

#endif // PATTERN_H