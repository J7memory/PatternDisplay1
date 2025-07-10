#include "BitMapLayerConfigDialog.h"
#include "ColorGridWidget.h"
#include "common.h"
#include "LayerDrawDialog.h"
#include <QGuiApplication>
#include <QScreen>

BitMapLayerConfigDialog::BitMapLayerConfigDialog(BitMapLayer* layer, QWidget* parent)
    : QDialog(parent), selectedLayer(layer) {
    originalParams = selectedLayer->getParameters();
    setWindowTitle(tr("BitMap Layer Configuration"));
    setFixedSize(900, 600);

    //QScreen* screen = QGuiApplication::primaryScreen();
    //QRect geometry = screen->geometry();
    //qreal pixelRatio = screen->devicePixelRatio();

    //int width = geometry.width() * pixelRatio;
    //int height = geometry.height() * pixelRatio;

    // ---- 创建控件 ----
    initializeSpinBox(startXSpinBox, "startX", 0, 100000);
    initializeSpinBox(startYSpinBox, "startY", 0, 100000);
    initializeSpinBox(widthSpinBox, "width", 0, 100000);
    initializeSpinBox(heightSpinBox, "height", 0, 100000);
    initializeSpinBox(bitMapWidthSpinBox, "bitMapWidth", 1, 8);
    initializeSpinBox(bitMapHeightSpinBox, "bitMapHeight", 1, 8);

    initializeColorButton(colorButton, "color", currentColor, alphaLabel);
    initializeAlphaLabel(alphaLabel, currentColor);

    // 创建网格控件并设置初始尺寸
    colorGrid = new ColorGridWidget(this);
    int bitMapWidth = bitMapWidthSpinBox->value();
    int bitMapHeight = bitMapHeightSpinBox->value();
    colorGrid->setGridSize(bitMapWidth, bitMapHeight);

    initializeButton(applyButton, tr("Apply"), 80, false);
    initializeButton(previewButton, tr("Preview"), 80, false);
    initializeButton(cancelButton, tr("Cancel"), 80, false);

    initializeButton(undoButton, QString::fromUtf8("\u2190"), 40, false);
    //initializeButton(redoButton, QString::fromUtf8("\u2192"), 40, false);

    // ---- 布局 ----
    createLayout();

    // ---- 连接信号与槽 ----
    connectSignals();

    updateControlsFromParameters();

}

QMap<QString, QVariant> BitMapLayerConfigDialog::getLayerParameters() const {
    QMap<QString, QVariant> params;
    params["startX"] = startXSpinBox->value();
    params["startY"] = startYSpinBox->value();
    params["width"] = widthSpinBox->value();
    params["height"] = heightSpinBox->value();
    params["bitMapWidth"] = bitMapWidthSpinBox->value();
    params["bitMapHeight"] = bitMapHeightSpinBox->value();
    params["color"] = currentColor;
    QList<QColor> pixelColorsList = colorGrid->getPixelColors();
    params["pixelColors"] = QVariant::fromValue(pixelColorsList);
    return params;
}

// 创建控件
void BitMapLayerConfigDialog::initializeSpinBox(QSpinBox*& spinBox, const QString& paramKey, int minValue, int maxValue) {
    spinBox = new QSpinBox(this);
    spinBox->setRange(minValue, maxValue);
    spinBox->setValue(selectedLayer->getParameters().value(paramKey).toInt());
    spinBox->setFixedWidth(120);
}

void BitMapLayerConfigDialog::initializeColorButton(QPushButton*& button, const QString& paramKey, QColor& color, QLabel* alphaLabel) {
    button = new QPushButton(this);
    color = selectedLayer->getParameters().value(paramKey).value<QColor>();
    button->setStyleSheet("background-color: " + color.name());
    button->setFixedWidth(80);
    button->setAutoDefault(false);
}

void BitMapLayerConfigDialog::initializeAlphaLabel(QLabel*& alphaLabel, QColor color) {
    alphaLabel = new QLabel(this);
    QString alphaText = tr("Alpha");
    alphaLabel->setText(QString("%1 : %2").arg(alphaText).arg(color.alpha()));
}

void BitMapLayerConfigDialog::initializeButton(QPushButton*& button, const QString& text, int fixedWidth, bool autoDefault) {
    button = new QPushButton(text, this);
    button->setFixedWidth(fixedWidth);
    button->setAutoDefault(autoDefault);
}

// 布局
QVBoxLayout* BitMapLayerConfigDialog::createLayout() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QHBoxLayout* topButtonLayout = new QHBoxLayout();
    topButtonLayout->addWidget(undoButton);
    //topButtonLayout->addWidget(redoButton);
    topButtonLayout->addStretch();

    QVBoxLayout* verticalLayout = new QVBoxLayout;

    QFormLayout* sizeLayout = new QFormLayout;
    sizeLayout->setContentsMargins(80, 0, 0, 0);
    sizeLayout->setSpacing(25);
    sizeLayout->addRow(new QLabel(tr("StartX\t\t:")), startXSpinBox);
    sizeLayout->addRow(new QLabel(tr("StartY\t\t:")), startYSpinBox);
    sizeLayout->addRow(new QLabel(tr("Width\t\t:")), widthSpinBox);
    sizeLayout->addRow(new QLabel(tr("Height\t\t:")), heightSpinBox);
    sizeLayout->addRow(new QLabel(tr("BitMap Width\t:")), bitMapWidthSpinBox);
    sizeLayout->addRow(new QLabel(tr("BitMap Height\t:")), bitMapHeightSpinBox);

    QHBoxLayout* bitColorLayout = new QHBoxLayout;
    bitColorLayout->addWidget(colorButton);
    bitColorLayout->addWidget(alphaLabel);
    sizeLayout->addRow(new QLabel(tr("SubPixel Color\t:")), bitColorLayout);

    verticalLayout->addLayout(sizeLayout);

    // ---- 水平布局 1：将 colorGrid 放在右侧 ----
    QHBoxLayout* horizontalLayout1 = new QHBoxLayout;
    horizontalLayout1->addLayout(verticalLayout);
    horizontalLayout1->addWidget(colorGrid);

    // ---- 水平布局 2：应用和取消按钮 ----
    QHBoxLayout* horizontalLayout2 = new QHBoxLayout;
    horizontalLayout2->addWidget(applyButton);
    horizontalLayout2->addWidget(previewButton);
    horizontalLayout2->addWidget(cancelButton);

    // ---- 组装主布局 ----
    mainLayout->addLayout(topButtonLayout);
    mainLayout->addSpacing(70);
    mainLayout->addLayout(horizontalLayout1);
    mainLayout->addLayout(horizontalLayout2);
    mainLayout->addSpacing(20);

    setLayout(mainLayout);

    return mainLayout;
}

// 信号与槽
void BitMapLayerConfigDialog::connectSignals() {
    for (QSpinBox* spinBox : { startXSpinBox, startYSpinBox, widthSpinBox, heightSpinBox }) {
        connect(spinBox, &QSpinBox::editingFinished, this, &BitMapLayerConfigDialog::chooseSpinBox);
        connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &BitMapLayerConfigDialog::updateThumbnail);
    }

    for (QSpinBox* spinBox : { bitMapWidthSpinBox, bitMapHeightSpinBox }) {
        connect(spinBox, &QSpinBox::editingFinished, this, &BitMapLayerConfigDialog::chooseSpinBox);
        connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &BitMapLayerConfigDialog::updateThumbnail);
        connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &BitMapLayerConfigDialog::updateGridSize);
    }

    for (QPushButton* button : { colorButton }) {
        connect(button, &QPushButton::clicked, this, &BitMapLayerConfigDialog::chooseBitMapColor);
        connect(button, &QPushButton::clicked, this, &BitMapLayerConfigDialog::updateAlphaLabel);
    }

    connect(colorGrid, &ColorGridWidget::pixelClicked, this, &BitMapLayerConfigDialog::handlePixelClicked);

    for (QPushButton* button : { applyButton, previewButton, cancelButton, undoButton }) {
        connect(button, &QPushButton::clicked, this, [this, button]() {
            if (button == applyButton) applyChanges();
            else if (button == previewButton) previewChanges();
            else if (button == cancelButton) cancelChanges();
            else if (button == undoButton) undoChanges();
            });
    }
}

void BitMapLayerConfigDialog::chooseSpinBox() {
    int newStartX = startXSpinBox->value();
    int newStartY = startYSpinBox->value();
    int newWidth = widthSpinBox->value();
    int newHeight = heightSpinBox->value();
    int newBitMapWidth = bitMapWidthSpinBox->value();
    int newBitMapHeight = bitMapHeightSpinBox->value();

    pushToUndoStack(selectedLayer->getParameters(), colorGrid->getPixelColors());
}

void BitMapLayerConfigDialog::chooseBitMapColor() {
    chooseColor(currentColor, colorButton, this);
    pushToUndoStack(selectedLayer->getParameters(), colorGrid->getPixelColors());
    updateThumbnail();
}

void BitMapLayerConfigDialog::updateAlphaLabel() {
    QString alphaText = tr("Alpha");
    alphaLabel->setText(QString("%1 : %2").arg(alphaText).arg(currentColor.alpha()));
}

void BitMapLayerConfigDialog::updateControlsFromParameters() {
    widthSpinBox->setValue(originalParams["width"].toInt());
    heightSpinBox->setValue(originalParams["height"].toInt());
    bitMapWidthSpinBox->setValue(originalParams["bitMapWidth"].toInt());
    bitMapHeightSpinBox->setValue(originalParams["bitMapHeight"].toInt());
    currentColor = originalParams["color"].value<QColor>();
    colorButton->setStyleSheet("background-color: " + currentColor.name());
    colorGrid->setPixelColors(originalParams["pixelColors"].value<QList<QColor>>());

    updateGridSize();
}

void BitMapLayerConfigDialog::applyChanges() {
    QMap<QString, QVariant> newParams = getLayerParameters();
    selectedLayer->setParameters(newParams);
    accept();
}

void BitMapLayerConfigDialog::previewChanges() {
    QMap<QString, QVariant> newParams = getLayerParameters();
    selectedLayer->setParameters(newParams);

    QList<std::shared_ptr<Layer>> layersList;
    layersList.append(std::make_shared<BitMapLayer>(*selectedLayer));

    LayerDrawDialog* drawDialog = new LayerDrawDialog();
    drawDialog->setLayers(layersList);
    drawDialog->exec();
    delete drawDialog;
}

void BitMapLayerConfigDialog::cancelChanges() {
    selectedLayer->setParameters(originalParams);
    this->close();
}

void BitMapLayerConfigDialog::undoChanges() {
    if (!undoStack.isEmpty()) {
        QMap<QString, QVariant> lastParams = undoStack.pop();
        selectedLayer->setParameters(lastParams);
        QList<QColor> pixelColors = lastParams["pixelColors"].value<QList<QColor>>();
        colorGrid->setPixelColors(pixelColors);
    }
    else {
        selectedLayer->setParameters(originalParams);
        colorGrid->setPixelColors(originalParams["pixelColors"].value<QList<QColor>>());
    }

    startXSpinBox->setValue(selectedLayer->getParameters()["startX"].toInt());
    startYSpinBox->setValue(selectedLayer->getParameters()["startY"].toInt());
    widthSpinBox->setValue(selectedLayer->getParameters()["width"].toInt());
    heightSpinBox->setValue(selectedLayer->getParameters()["height"].toInt());
    bitMapWidthSpinBox->setValue(selectedLayer->getParameters()["bitMapWidth"].toInt());
    bitMapHeightSpinBox->setValue(selectedLayer->getParameters()["bitMapHeight"].toInt());
    
    currentColor = selectedLayer->getParameters()["color"].value<QColor>();
    colorButton->setStyleSheet("background-color: " + currentColor.name());
    
    QList<QColor> savedPixelColors = selectedLayer->getParameters()["pixelColors"].value<QList<QColor>>();
    colorGrid->setPixelColors(savedPixelColors);

    updateAlphaLabel();
    updateThumbnail();
    updateGridSize();
}

void BitMapLayerConfigDialog::pushToUndoStack(const QMap<QString, QVariant>& params, const QList<QColor>& pixelColors) {
    if (undoStack.size() >= maxUndoCount) {
        undoStack.remove(0);
    }
    QMap<QString, QVariant> snapshot = params;
    snapshot["pixelColors"] = QVariant::fromValue(pixelColors);  // 保存网格颜色
    undoStack.push(snapshot);
}


void BitMapLayerConfigDialog::handlePixelClicked(int index) {
    // 获取当前图层的参数
    QMap<QString, QVariant> currentParams = selectedLayer->getParameters();
    pushToUndoStack(currentParams, colorGrid->getPixelColors());

    // 获取当前网格的像素颜色列表
    QList<QColor> colors = colorGrid->getPixelColors();
    if (index >= 0 && index < colors.size()) {
        colors[index] = currentColor;
        colorGrid->setPixelColors(colors);  // 更新网格显示的颜色
    }
}

void BitMapLayerConfigDialog::updateGridSize() {
    int width = bitMapWidthSpinBox->value();
    int height = bitMapHeightSpinBox->value();
    colorGrid->setGridSize(width, height);
    colorGrid->update();
}

void BitMapLayerConfigDialog::updateThumbnail() {
    QMap<QString, QVariant> newParams = getLayerParameters();
    selectedLayer->setParameters(newParams);
}