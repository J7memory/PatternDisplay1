#ifndef PATTERNMANAGER_H
#define PATTERNMANAGER_H

#include <QVector>
#include <memory>
#include <QStringList>
#include "Pattern.h"

class PatternManager {
public:
    // ��ȡ��һ�����õ� Pattern ���
    static int getNextPatternNumber();

    // ���� Pattern
    static std::shared_ptr<Pattern> addPattern();

    // ͨ��ָ��ɾ�� Pattern
    static bool removePattern(const std::shared_ptr<Pattern>& pattern);

    // ͨ������ɾ�� Pattern
    static bool removePatternByName(const QString& name);

    // ɾ������ Pattern
    static void clearAllPatterns();

    // �������ƻ�ȡ Pattern ����
    static std::shared_ptr<Pattern> getPatternByName(const QString& name);

    // ��ȡ���� Pattern ������
    static QStringList getAllPatternNames();

    // ��ȡ���е� Pattern ����
    static QVector<std::shared_ptr<Pattern>> getAllPatterns() { return patterns; }

    // ����patternList������
    static void updatePatternOrder(const QStringList& newOrder);

    // pattern ������
    static bool renamePatternName(const QString& oldName, const QString& newName);

    // ��ӡ����Pattern����Ϣ
    void printPatternInfo() const;

    // ����ͼ�����ʹ���ͼ��
    static std::shared_ptr<Layer> createLayerByType(const QString& layerType);

    // ��� Layer ��ָ�� Pattern
    static void addLayerToPattern(const QString& patternName, const std::shared_ptr<Layer>& layer);

    // ��ȡĳ�� Pattern ������ Layers
    static QVector<std::shared_ptr<Layer>> getLayersByPattern(const QString& patternName);

    // ��ȡĳ�� Pattern ������ Layers �� layerType
    static QStringList getLayerTypesByPattern(const QString& patternName);

    // ͨ��layerList������ȡĳ�� Layer ����
    static std::shared_ptr<Layer> getLayerByIndex(const QString& patternName, int index);

    // ͨ��layerList����ɾ�� Pattern �ڵ�ĳ�� Layer
    static bool removeLayerFromPattern(const QString& patternName, int layerIndex);

    // ����Json���͵������ļ���.Json����Json���ݵ�.dat�ļ���
    static std::shared_ptr<Pattern> loadPatternwithJson(const QString& patternName, int patternWidth, int patternHeight, const QString& patternDescription);

    // ���봿���������͵�.dat�����ļ�
    static std::shared_ptr<Pattern> loadPattern(const QString& patternName, int patternWidth, int patternHeight, const QString& patternDescription, int index);

private:
    static QVector<std::shared_ptr<Pattern>> patterns;
};

#endif // PATTERNMANAGER_H