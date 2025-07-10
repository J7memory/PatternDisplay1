// Layer.h
#ifndef LAYER_H
#define LAYER_H

#include <QString>
#include <QMap>
#include <QVariant>
#include <QPainter>
#include <QDebug>

class Layer {
public:
    Layer(const QString& type = "", bool status = true);
    virtual ~Layer() = default;  // 虚析构函数，确保派生类的析构函数被调用

    // 纯虚函数，由派生类实现
    virtual void setParameters(const QMap<QString, QVariant>& params) = 0;
    virtual QMap<QString, QVariant> getParameters() const = 0;
    virtual void drawLayer(QPainter& painter, bool isThumbnail = false) const = 0;

    // 输出Layer属性信息
    virtual void outputLayerInfo() const;

    // 获取图层类型/状态
    QString getLayerType() const;
    bool getLayerStatus() const;

    // 设置图层类型/状态
    void setLayerType(const QString& type) const;
    void setLayerStatus(bool status) const;

private:
    mutable QString layerType;   // mutable 允许在const函数中修改
    mutable bool layerStatus;
};

#endif // LAYER_H
