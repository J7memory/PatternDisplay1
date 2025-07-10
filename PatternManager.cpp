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

// 初始化静态变量
QVector<std::shared_ptr<Pattern>> PatternManager::patterns;

// 获取下一个可用的 Pattern 编号
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

// 新增 Pattern
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

// 通过指针删除 Pattern
bool PatternManager::removePattern(const std::shared_ptr<Pattern>& pattern) {
    return patterns.removeOne(pattern);
}

// 根据名称删除 Pattern
bool PatternManager::removePatternByName(const QString& name) {
    for (int i = 0; i < patterns.size(); ++i) {
        if (patterns[i]->getPatternName() == name) {
            patterns.remove(i);
            return true;
        }
    }
    return false;
}

// 删除所有 Pattern
void PatternManager::clearAllPatterns() {
    patterns.clear();  // 清空列表，shared_ptr 自动释放资源
}

// 根据名称获取 Pattern 对象
std::shared_ptr<Pattern> PatternManager::getPatternByName(const QString& name) {
    for (const auto& pattern : patterns) {
        if (pattern->getPatternName() == name) {
            return pattern;
        }
    }
    return nullptr;
}

// 获取所有 Pattern 名称
QStringList PatternManager::getAllPatternNames() {
    QStringList patternNames;
    for (const auto& pattern : patterns) {
        patternNames.append(pattern->getPatternName());
    }
    return patternNames;
}

// 更新patternList索引
void PatternManager::updatePatternOrder(const QStringList& newOrder) {
    QList<std::shared_ptr<Pattern>> newPatterns;
    for (const QString& name : newOrder) {
        auto ptr = getPatternByName(name);
        if (ptr) newPatterns.append(ptr);
    }
    patterns = newPatterns;
}

// pattern 重命名
bool PatternManager::renamePatternName(const QString& oldName, const QString& newName) {
    // 检查是否重名
    for (const auto& pattern : patterns) {
        if (pattern->getPatternName() == newName) {
            return false;
        }
    }

    // 查找并设置新名字
    for (auto& pattern : patterns) {
        if (pattern->getPatternName() == oldName) {
            pattern->setPatternName(newName);
            return true;
        }
    }

    return false;
}

// 打印所有Pattern的信息
void PatternManager::printPatternInfo() const {
    for (const auto& pattern : patterns) {
        if (pattern) {
            pattern->printInfo();  // 调用Pattern的printInfo函数来打印信息
        }
    }
}

// 根据图层类型创建图层
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

// 向指定 Pattern 中添加 Layer
void PatternManager::addLayerToPattern(const QString& patternName, const std::shared_ptr<Layer>& layer) {
    auto pattern = getPatternByName(patternName);
    if (pattern) {
        pattern->addLayer(layer);
    }
}

// 获取某个 Pattern 的所有 Layers
QVector<std::shared_ptr<Layer>> PatternManager::getLayersByPattern(const QString& patternName) {
    auto pattern = getPatternByName(patternName);
    if (pattern) {
        return pattern->getLayers();
    }
    return QVector<std::shared_ptr<Layer>>();
}

// 获取 Pattern 的所有 Layer 类型
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

// 通过 layerIndex 获取 layer 对象
std::shared_ptr<Layer> PatternManager::getLayerByIndex(const QString& patternName, int index) {
    auto pattern = getPatternByName(patternName);
    if (!pattern) return nullptr;

    return pattern->getLayer(index);
}

// 通过layerList索引删除指定 Pattern 中的 Layer
bool PatternManager::removeLayerFromPattern(const QString& patternName, int layerIndex) {
    auto pattern = getPatternByName(patternName);
    if (pattern) {
        return pattern->removeLayer(layerIndex);
    }
    return false;
}

// 从配置文件中加载 Pattern
std::shared_ptr<Pattern> PatternManager::loadPatternwithJson(const QString& patternName, int patternWidth, int patternHeight, const QString& patternDescription) {
    auto newPattern = std::make_shared<Pattern>(patternName, patternWidth, patternHeight, patternDescription);
    patterns.append(newPattern);
    return newPattern;
}

// 载入纯二进制类型的.dat配置文件
std::shared_ptr<Pattern> PatternManager::loadPattern(const QString& patternName, int patternWidth, int patternHeight, const QString& patternDescription, int index) {
    auto newPattern = std::make_shared<Pattern>(patternName, patternWidth, patternHeight, patternDescription);
    patterns.insert(index, newPattern);  // 按照传入的 index 插入
    return newPattern;
}
