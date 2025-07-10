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
    virtual ~Layer() = default;  // ������������ȷ�����������������������

    // ���麯������������ʵ��
    virtual void setParameters(const QMap<QString, QVariant>& params) = 0;
    virtual QMap<QString, QVariant> getParameters() const = 0;
    virtual void drawLayer(QPainter& painter, bool isThumbnail = false) const = 0;

    // ���Layer������Ϣ
    virtual void outputLayerInfo() const;

    // ��ȡͼ������/״̬
    QString getLayerType() const;
    bool getLayerStatus() const;

    // ����ͼ������/״̬
    void setLayerType(const QString& type) const;
    void setLayerStatus(bool status) const;

private:
    mutable QString layerType;   // mutable ������const�������޸�
    mutable bool layerStatus;
};

#endif // LAYER_H
