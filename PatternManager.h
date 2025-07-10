#ifndef PATTERNMANAGER_H
#define PATTERNMANAGER_H

#include <QVector>
#include <memory>
#include <QStringList>
#include "Pattern.h"

class PatternManager {
public:
    // 获取下一个可用的 Pattern 编号
    static int getNextPatternNumber();

    // 新增 Pattern
    static std::shared_ptr<Pattern> addPattern();

    // 通过指针删除 Pattern
    static bool removePattern(const std::shared_ptr<Pattern>& pattern);

    // 通过名称删除 Pattern
    static bool removePatternByName(const QString& name);

    // 删除所有 Pattern
    static void clearAllPatterns();

    // 根据名称获取 Pattern 对象
    static std::shared_ptr<Pattern> getPatternByName(const QString& name);

    // 获取所有 Pattern 的名称
    static QStringList getAllPatternNames();

    // 获取所有的 Pattern 对象
    static QVector<std::shared_ptr<Pattern>> getAllPatterns() { return patterns; }

    // 更新patternList的索引
    static void updatePatternOrder(const QStringList& newOrder);

    // pattern 重命名
    static bool renamePatternName(const QString& oldName, const QString& newName);

    // 打印所有Pattern的信息
    void printPatternInfo() const;

    // 根据图层类型创建图层
    static std::shared_ptr<Layer> createLayerByType(const QString& layerType);

    // 添加 Layer 到指定 Pattern
    static void addLayerToPattern(const QString& patternName, const std::shared_ptr<Layer>& layer);

    // 获取某个 Pattern 的所有 Layers
    static QVector<std::shared_ptr<Layer>> getLayersByPattern(const QString& patternName);

    // 获取某个 Pattern 的所有 Layers 的 layerType
    static QStringList getLayerTypesByPattern(const QString& patternName);

    // 通过layerList索引获取某个 Layer 对象
    static std::shared_ptr<Layer> getLayerByIndex(const QString& patternName, int index);

    // 通过layerList索引删除 Pattern 内的某个 Layer
    static bool removeLayerFromPattern(const QString& patternName, int layerIndex);

    // 载入Json类型的配置文件（.Json或者Json内容的.dat文件）
    static std::shared_ptr<Pattern> loadPatternwithJson(const QString& patternName, int patternWidth, int patternHeight, const QString& patternDescription);

    // 载入纯二进制类型的.dat配置文件
    static std::shared_ptr<Pattern> loadPattern(const QString& patternName, int patternWidth, int patternHeight, const QString& patternDescription, int index);

private:
    static QVector<std::shared_ptr<Pattern>> patterns;
};

#endif // PATTERNMANAGER_H