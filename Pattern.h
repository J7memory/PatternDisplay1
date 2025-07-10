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
    // ���캯��
    Pattern(QString name, int width, int height, QString description = "");

    // ��Pattern�����Layer
    void addLayer(std::shared_ptr<Layer> layer);

    // ��Pattern���Ƴ��������Layer
    bool removeLayer(int index);

    // ����layer����
    void updateLayerOrder(const QList<std::shared_ptr<Layer>>& newLayers);

    // ����pattern����/���/�߶�/����
    void setPatternName(const QString& name);
    void setPatternWidth(int width);
    void setPatternHeight(int height);
    void setPatternDescription(const QString& description);

    // ��ȡpattern����/���/�߶�/����
    QString getPatternName() const;
    int getPatternWidth() const;
    int getPatternHeight() const;
    QString getPatternDescription() const;

    // ��ȡ����ͼ��������
    QStringList getAllPatternNames(const QVector<std::shared_ptr<Pattern>>& patterns);

    // ��ȡͼ������������ͼ��
    QVector<std::shared_ptr<Layer>> getLayers() const;

    // ��ȡĳ��Layer����ϸ��Ϣ
    std::shared_ptr<Layer> getLayer(int index);

    // ���������޸�Layer����
    void replaceLayer(int index, std::shared_ptr<Layer> newLayer);

    // ����ͼ��
    void drawPattern(QPainter& painter, bool isThumbnail = false);

    // ��ӡͼ����Ϣ
    void printInfo() const;

    int getWidth() const;
    int getHeight() const;

    QVector<std::shared_ptr<Layer>> layers;  // �洢���ͼ��

private:
    QString patternName;
    int patternWidth;
    int patternHeight;
    QString patternDescription;
};

#endif // PATTERN_H