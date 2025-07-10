#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include "BitMapLayer.h"
#include "CCircleLayer.h"
#include "CheckerLayer.h"
#include "CRectLayer.h"
#include "GradientLayer.h"
#include "Layer.h"
#include "LineLayer.h"
#include "MonoLayer.h"
#include "TextLayer.h"
#include "PictureLayer.h"
#include <QCheckBox>
#include <QComboBox>
#include <QDataStream>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLineEdit>
#include <QSpinBox>
#include <QTextEdit>
#include <QWidget>
#include <QLabel>
#include <QListWidgetItem>
#include <QGroupBox>
#include <QFormLayout>
#include "PatternDrawDialog.h"

class QListWidget;
class QPushButton;
class QTabWidget;
class Pattern;

class DisplayWidget : public QWidget {
    Q_OBJECT

public:
    explicit DisplayWidget(QWidget* parent = nullptr);
    ~DisplayWidget();

    void retranslateUi();

    void updatePatternListView();
    void updateLayerListView(const std::shared_ptr<Pattern>& patternPtr);
    void clearAllPatterns();

    // I/O
    void saveConfiguration();
    void loadConfiguration();

    bool saveConfigurationBinaryPro(const QString& filePath);
    void loadConfigurationBinaryPro(const QString& filePath);

    QString currentConfigPath; // 配置文件路径
    void updateConfig();  // 例如，这个函数会在配置更改时被调用

    QLabel* patternPreview; // pattern缩略图

    bool eventFilter(QObject* obj, QEvent* event);

signals:
    void configChanged(bool changed);  // 当配置更改时发送信号

private slots:
    void addPattern();
    void removePattern();
    void onPatternSelected();
    void savePatternInformation();
    //void cancelPatternInformation();
    void onPatternSelectedDraw();
    void onPatternSelectPixmapDraw();
    void savePatterAsImage();

    void onPatternListItemChanged();
    void showPatternListContextMenu(const QPoint& pos);

    void renamePattern();
    void onPatternNameChanged(QListWidgetItem* item);

    void movePatternUp();
    void movePatternDown();

    void addLayer();
    void editLayer();
    void removeLayer();
    void onLayerListItemChanged();
    void showLayerListContextMenu(const QPoint& pos);

    void moveLayerUp();
    void moveLayerDown();

    // 输出所有的pattern信息
    void outputAllPatternInfo();

private:
    // 主界面
    void setupUI();

    PatternDrawDialog* privewDialog = nullptr;
    void updatePreviewPattern();

    // Information Tab页
    QWidget* creatInformationTab();

    // 用来保存正在创建但还没正式加入Pattern的新Layer
    std::shared_ptr<Layer> pendingNewLayer;
    QString pendingLayerType;

    // 左侧列表和控件
    QPushButton* addButton;
    QPushButton* delButton;
    QListWidget* patternList;

    QTabWidget* tabWidget;
    QWidget* infoTab;

    QLabel* widthLabel;
    QLabel* heightLabel;

    QGroupBox* patternInfoBox;
    QGroupBox* patternListBox;
    QGroupBox* layerListBox;
    QGroupBox* patternPreviewBox;
    QFormLayout* patternFormLayout;

    // Information选项卡控件
    QPushButton* addLayerButton;
    QPushButton* delLayerButton;
    QListWidget* layerList;

    QLineEdit* patternNameEdit;
    QSpinBox* patternWidthSpinBox;
    QSpinBox* patternHeightSpinBox;
    QTextEdit* patternDescriptionEdit;

    QPushButton* editButton;
    QPushButton* previewButton;
    QPushButton* saveButton;

    // 各Layer类型写入Json文件函数
    void processMonoLayer(Layer* layer, QJsonObject& layerObject);
    void processGradientLayer(Layer* layer, QJsonObject& layerObject);
    void processCheckerLayer(Layer* layer, QJsonObject& layerObject);
    void processBitMapLayer(Layer* layer, QJsonObject& layerObject);
    void processCRectLayer(Layer* layer, QJsonObject& layerObject);
    void processCCircleLayer(Layer* layer, QJsonObject& layerObject);
    void processLineLayer(Layer* layer, QJsonObject& layerObject);
    void processTextLayer(Layer* layer, QJsonObject& layerObject);
    void processPictureLayer(Layer* layer, QJsonObject& layerObject);

    // 各Layer类型写入.dat文件函数(二进制)
    void serializeMonoLayer(QDataStream& out, Layer* layer);
    void serializeGradientLayer(QDataStream& out, Layer* layer);
    void serializeCheckerLayer(QDataStream& out, Layer* layer);
    void serializeBitMapLayer(QDataStream& out, Layer* layer);
    void serializeCRectLayer(QDataStream& out, Layer* layer);
    void serializeCCircleLayer(QDataStream& out, Layer* layer);
    void serializeLineLayer(QDataStream& out, Layer* layer);
    void serializeTextLayer(QDataStream& out, Layer* layer);
    void serializePictureLayer(QDataStream& out, Layer* layer);

    // 各Layer类型载入Json文件函数
    std::shared_ptr<MonoLayer> createMonoLayer(const QJsonObject& layerObject, const QString& layerName);
    std::shared_ptr<GradientLayer> createGradientLayer(const QJsonObject& layerObject, const QString& layerName);
    std::shared_ptr<CheckerLayer> createCheckerLayer(const QJsonObject& layerObject, const QString& layerName);
    std::shared_ptr<BitMapLayer> createBitMapLayer(const QJsonObject& layerObject, const QString& layerName);
    std::shared_ptr<CRectLayer> createCRectLayer(const QJsonObject& layerObject, const QString& layerName);
    std::shared_ptr<CCircleLayer> createCCircleLayer(const QJsonObject& layerObject, const QString& layerName);
    std::shared_ptr<LineLayer> createLineLayer(const QJsonObject& layerObject, const QString& layerName);
    std::shared_ptr<TextLayer> createTextLayer(const QJsonObject& layerObject, const QString& layerName);
    std::shared_ptr<PictureLayer> createPictureLayer(const QJsonObject& layerObject, const QString& layerName);

    // 各Layer类型载入二进制文件函数
    std::shared_ptr<MonoLayer> deserializeMonoLayer(QDataStream& in, const QString& layerName);
    std::shared_ptr<GradientLayer> deserializeGradientLayer(QDataStream& in, const QString& layerName);
    std::shared_ptr<CheckerLayer> deserializeCheckerLayer(QDataStream& in, const QString& layerName);
    std::shared_ptr<BitMapLayer> deserializeBitMapLayer(QDataStream& in, const QString& layerName);
    std::shared_ptr<CRectLayer> deserializeCRectLayer(QDataStream& in, const QString& layerName);
    std::shared_ptr<CCircleLayer> deserializeCCircleLayer(QDataStream& in, const QString& layerName);
    std::shared_ptr<LineLayer> deserializeLineLayer(QDataStream& in, const QString& layerName);
    std::shared_ptr<TextLayer> deserializeTextLayer(QDataStream& in, const QString& layerName);
    std::shared_ptr<PictureLayer> deserializePictureLayer(QDataStream& in, const QString& layerName);
};

#endif // DISPLAYWIDGET_H
