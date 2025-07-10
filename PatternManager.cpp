#include "BitMapLayer.h"
#include "CCircleLayer.h"
#include "CheckerLayer.h"
#include "CRectLayer.h"
#include "GradientLayer.h"
#include "LineLayer.h"
#include "MonoLayer.h"
#include "Pattern.h"
#include "PatternManager.h"
#include "PictureLayer.h"
#include "TextLayer.h"

#include <algorithm>
#include <QDebug>
#include <QGuiApplication>
#include <QScreen>

// ��ʼ����̬����
QVector<std::shared_ptr<Pattern>> PatternManager::patterns;

// ��ȡ��һ�����õ� Pattern ���
int PatternManager::getNextPatternNumber() {
    QVector<int> existingNumbers;
    for (const auto& pattern : patterns) {
        bool ok;
        int num = pattern->getPatternName().split(' ').last().toInt(&ok);
        if (ok) existingNumbers.append(num);
    }

    std::sort(existingNumbers.begin(), existingNumbers.end());

    int nextNum = 1;
    for (int num : existingNumbers) {
        if (num == nextNum) nextNum++;
        else break;
    }
    return nextNum;
}

// ���� Pattern
std::shared_ptr<Pattern> PatternManager::addPattern() {
    int nextPatternNumber = getNextPatternNumber();
    QString patternName = QString("Pattern %1").arg(nextPatternNumber);

    QScreen* screen = QGuiApplication::primaryScreen();
    QRect geometry = screen->geometry();
    qreal pixelRatio = screen->devicePixelRatio();

    int width = geometry.width() * pixelRatio;
    int height = geometry.height() * pixelRatio;

    auto newPattern = std::make_shared<Pattern>(patternName, width, height, "New Pattern Description");
    patterns.append(newPattern);
    return newPattern;
}

// ͨ��ָ��ɾ�� Pattern
bool PatternManager::removePattern(const std::shared_ptr<Pattern>& pattern) {
    return patterns.removeOne(pattern);
}

// ��������ɾ�� Pattern
bool PatternManager::removePatternByName(const QString& name) {
    for (int i = 0; i < patterns.size(); ++i) {
        if (patterns[i]->getPatternName() == name) {
            patterns.remove(i);
            return true;
        }
    }
    return false;
}

// ɾ������ Pattern
void PatternManager::clearAllPatterns() {
    patterns.clear();  // ����б�shared_ptr �Զ��ͷ���Դ
}

// �������ƻ�ȡ Pattern ����
std::shared_ptr<Pattern> PatternManager::getPatternByName(const QString& name) {
    for (const auto& pattern : patterns) {
        if (pattern->getPatternName() == name) {
            return pattern;
        }
    }
    return nullptr;
}

// ��ȡ���� Pattern ����
QStringList PatternManager::getAllPatternNames() {
    QStringList patternNames;
    for (const auto& pattern : patterns) {
        patternNames.append(pattern->getPatternName());
    }
    return patternNames;
}

// ����patternList����
void PatternManager::updatePatternOrder(const QStringList& newOrder) {
    QList<std::shared_ptr<Pattern>> newPatterns;
    for (const QString& name : newOrder) {
        auto ptr = getPatternByName(name);
        if (ptr) newPatterns.append(ptr);
    }
    patterns = newPatterns;
}

// pattern ������
bool PatternManager::renamePatternName(const QString& oldName, const QString& newName) {
    // ����Ƿ�����
    for (const auto& pattern : patterns) {
        if (pattern->getPatternName() == newName) {
            return false;
        }
    }

    // ���Ҳ�����������
    for (auto& pattern : patterns) {
        if (pattern->getPatternName() == oldName) {
            pattern->setPatternName(newName);
            return true;
        }
    }

    return false;
}

// ��ӡ����Pattern����Ϣ
void PatternManager::printPatternInfo() const {
    for (const auto& pattern : patterns) {
        if (pattern) {
            pattern->printInfo();  // ����Pattern��printInfo��������ӡ��Ϣ
        }
    }
}

// ����ͼ�����ʹ���ͼ��
std::shared_ptr<Layer> PatternManager::createLayerByType(const QString& layerType) {
    if (layerType == "Mono") return std::make_shared<MonoLayer>();
    if (layerType == "Gradient") return std::make_shared<GradientLayer>();
    if (layerType == "Checker") return std::make_shared<CheckerLayer>();
    if (layerType == "BitMap") return std::make_shared<BitMapLayer>();
    if (layerType == "CRect") return std::make_shared<CRectLayer>();
    if (layerType == "CCircle") return std::make_shared<CCircleLayer>();
    if (layerType == "Line") return std::make_shared<LineLayer>();
    if (layerType == "Text") return std::make_shared<TextLayer>();
    if (layerType == "Picture") return std::make_shared<PictureLayer>();
    return nullptr;
}

// ��ָ�� Pattern ����� Layer
void PatternManager::addLayerToPattern(const QString& patternName, const std::shared_ptr<Layer>& layer) {
    auto pattern = getPatternByName(patternName);
    if (pattern) {
        pattern->addLayer(layer);
    }
}

// ��ȡĳ�� Pattern ������ Layers
QVector<std::shared_ptr<Layer>> PatternManager::getLayersByPattern(const QString& patternName) {
    auto pattern = getPatternByName(patternName);
    if (pattern) {
        return pattern->getLayers();
    }
    return QVector<std::shared_ptr<Layer>>();
}

// ��ȡ Pattern ������ Layer ����
QStringList PatternManager::getLayerTypesByPattern(const QString& patternName) {
    QStringList layerTypes;
    auto pattern = getPatternByName(patternName);
    if (pattern) {
        for (const auto& layer : pattern->getLayers()) {
            layerTypes.append(layer->getLayerType());
        }
    }
    return layerTypes;
}

// ͨ�� layerIndex ��ȡ layer ����
std::shared_ptr<Layer> PatternManager::getLayerByIndex(const QString& patternName, int index) {
    auto pattern = getPatternByName(patternName);
    if (!pattern) return nullptr;

    return pattern->getLayer(index);
}

// ͨ��layerList����ɾ��ָ�� Pattern �е� Layer
bool PatternManager::removeLayerFromPattern(const QString& patternName, int layerIndex) {
    auto pattern = getPatternByName(patternName);
    if (pattern) {
        return pattern->removeLayer(layerIndex);
    }
    return false;
}

// �������ļ��м��� Pattern
std::shared_ptr<Pattern> PatternManager::loadPatternwithJson(const QString& patternName, int patternWidth, int patternHeight, const QString& patternDescription) {
    auto newPattern = std::make_shared<Pattern>(patternName, patternWidth, patternHeight, patternDescription);
    patterns.append(newPattern);
    return newPattern;
}

// ���봿���������͵�.dat�����ļ�
std::shared_ptr<Pattern> PatternManager::loadPattern(const QString& patternName, int patternWidth, int patternHeight, const QString& patternDescription, int index) {
    auto newPattern = std::make_shared<Pattern>(patternName, patternWidth, patternHeight, patternDescription);
    patterns.insert(index, newPattern);  // ���մ���� index ����
    return newPattern;
}
