#include "common.h"
#include "LayerDrawDialog.h"
#include "LineLayerConfigDialog.h"
#include <QColorDialog>
#include <QFormLayout>
#include <QLabel>
#include <QGuiApplication>
#include <QScreen>

LineLayerConfigDialog::LineLayerConfigDialog(LineLayer* layer, QWidget* parent)
    : QDialog(parent), selectedLayer(layer) {
    originalParams = layer->getParameters();
    setWindowTitle(tr("Line Layer Configuration"));
    setFixedSize(900, 600);

    //QScreen* screen = QGuiApplication::primaryScreen();
    //QRect geometry = screen->geometry();
    //qreal pixelRatio = screen->devicePixelRatio();

    //int width = geometry.width() * pixelRatio;
    //int height = geometry.height() * pixelRatio;

    // ---- 创建控件 ----
    initializeSpinBox(startXSpinBox, "startX", 0, 100000);
    initializeSpinBox(startYSpinBox, "startY", 0, 100000);
    initializeSpinBox(endXSpinBox, "endX", 0, 100000);
    initializeSpinBox(endYSpinBox, "endY", 0, 100000);
    initializeSpinBox(lineWidthSpinBox, "lineWidth", 0, 50000);

    initializeColorButton(lineColorButton, "lineColor", currentLineColor, lineColorAlphaLabel);
    initializeAlphaLabel(lineColorAlphaLabel, currentLineColor);

    initializeImageLabel(imageLabel, 400, 400);

    initializeButton(applyButton, tr("Apply"), 80, false);
    initializeButton(previewButton, tr("Preview"), 80, false);
    initializeButton(cancelButton, tr("Cancel"), 80, false);

    initializeButton(undoButton, QString::fromUtf8("\u2190"), 40, false);
    //initializeButton(redoButton, QString::fromUtf8("\u2192"), 40, false);

    // ---- 布局 ----
    createLayout();

    // ---- 连接信号与槽 ----
    connectSignals();
}

QMap<QString, QVariant> LineLayerConfigDialog::getLayerParameters() const {
    QMap<QString, QVariant> params;
    params["startX"] = startXSpinBox->value();
    params["startY"] = startYSpinBox->value();
    params["endX"] = endXSpinBox->value();
    params["endY"] = endYSpinBox->value();
    params["lineWidth"] = lineWidthSpinBox->value();
    params["lineColor"] = currentLineColor;
    return params;
}

// 创建控件
void LineLayerConfigDialog::initializeSpinBox(QSpinBox*& spinBox, const QString& paramKey, int minValue, int maxValue) {
    spinBox = new QSpinBox(this);
    spinBox->setRange(minValue, maxValue);
    spinBox->setValue(selectedLayer->getParameters().value(paramKey).toInt());
    spinBox->setFixedWidth(120);
}

void LineLayerConfigDialog::initializeColorButton(QPushButton*& button, const QString& paramKey, QColor& color, QLabel* alphaLabel) {
    button = new QPushButton(this);
    color = selectedLayer->getParameters().value(paramKey).value<QColor>();
    button->setStyleSheet("background-color: " + color.name());
    button->setFixedWidth(80);
    button->setAutoDefault(false);
}

void LineLayerConfigDialog::initializeAlphaLabel(QLabel*& alphaLabel, QColor color) {
    alphaLabel = new QLabel(this);
    QString alphaText = tr("Alpha");
    alphaLabel->setText(QString("%1 : %2").arg(alphaText).arg(color.alpha()));
}

void LineLayerConfigDialog::initializeButton(QPushButton*& button, const QString& text, int fixedWidth, bool autoDefault) {
    button = new QPushButton(text, this);
    button->setFixedWidth(fixedWidth);
    button->setAutoDefault(autoDefault);
}

void LineLayerConfigDialog::initializeImageLabel(QLabel*& lable, int thumbWidth, int thumbHeight) {
    lable = new QLabel;
    lable->setFixedSize(thumbWidth, thumbHeight);
    lable->setAlignment(Qt::AlignCenter);
    QPixmap thumbnail = generateThumbnail(selectedLayer, thumbWidth, thumbHeight);
    lable->setPixmap(thumbnail.scaled(lable->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

// 布局
QVBoxLayout* LineLayerConfigDialog::createLayout() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QHBoxLayout* topButtonLayout = new QHBoxLayout();
    topButtonLayout->addWidget(undoButton);
    //topButtonLayout->addWidget(redoButton);
    topButtonLayout->addStretch();

    QHBoxLayout* contentLayout = new QHBoxLayout;
    contentLayout->setContentsMargins(0, 0, 80, 80);

    QVBoxLayout* verticalLayout = new QVBoxLayout;
    verticalLayout->setContentsMargins(0, 80, 0, 0);
    QFormLayout* sizeLayout = new QFormLayout;
    sizeLayout->setContentsMargins(60, 0, 0, 0);
    sizeLayout->setSpacing(25);
    sizeLayout->addRow(new QLabel(tr("StartX\t:")), startXSpinBox);
    sizeLayout->addRow(new QLabel(tr("StartY\t:")), startYSpinBox);
    sizeLayout->addRow(new QLabel(tr("EndX\t:")), endXSpinBox);
    sizeLayout->addRow(new QLabel(tr("EndY\t:")), endYSpinBox);
    sizeLayout->addRow(new QLabel(tr("LineWidth :")), lineWidthSpinBox);

    QHBoxLayout* lineColorLayout = new QHBoxLayout;
    lineColorLayout->addWidget(lineColorButton);
    lineColorLayout->addWidget(lineColorAlphaLabel);
    sizeLayout->addRow(new QLabel(tr("LineColor :")), lineColorLayout);

    verticalLayout->addLayout(sizeLayout);

    // ---- 将左侧参数和右侧图片加入水平布局 ----
    contentLayout->addLayout(verticalLayout);
    contentLayout->addWidget(imageLabel);

    QHBoxLayout* horizontalLayout2 = new QHBoxLayout;
    horizontalLayout2->addWidget(applyButton);
    horizontalLayout2->addWidget(previewButton);
    horizontalLayout2->addWidget(cancelButton);

    mainLayout->addLayout(topButtonLayout);
    mainLayout->addSpacing(30);
    mainLayout->addLayout(contentLayout);
    mainLayout->addLayout(horizontalLayout2);
    mainLayout->addSpacing(20);

    setLayout(mainLayout);

    return mainLayout;
}

// 信号与槽
void LineLayerConfigDialog::connectSignals() {
    for (QSpinBox* spinBox : { startXSpinBox, startYSpinBox, endXSpinBox, endYSpinBox, lineWidthSpinBox }) {
        connect(spinBox, &QSpinBox::editingFinished, this, &LineLayerConfigDialog::chooseSpinBox);
        connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &LineLayerConfigDialog::updateThumbnail);
    }

    for (QPushButton* button : { /*backColorButton, */lineColorButton }) {
        connect(button, &QPushButton::clicked, this, [this, button]() {
            chooseLineColor();
            updateAlphaLabel();
            });
    }

    for (QPushButton* button : { applyButton, previewButton, cancelButton, undoButton }) {
        connect(button, &QPushButton::clicked, this, [this, button]() {
            if (button == applyButton) applyChanges();
            else if (button == previewButton) previewChanges();
            else if (button == cancelButton) cancelChanges();
            else if (button == undoButton) undoChanges();
            });
    }
}

void LineLayerConfigDialog::chooseSpinBox() {
    int newStartX = startXSpinBox->value();
    int newStartY = startYSpinBox->value();
    int newEndX = endXSpinBox->value();
    int newEndY = endYSpinBox->value();
    int newLineWidth = lineWidthSpinBox->value();
    pushToUndoStack(selectedLayer->getParameters());
}

void LineLayerConfigDialog::chooseLineColor() {
    chooseColor(currentLineColor, lineColorButton, this);
    pushToUndoStack(selectedLayer->getParameters());
    updateThumbnail();
}

void LineLayerConfigDialog::updateAlphaLabel() {
    QString alphaText = tr("Alpha");
    lineColorAlphaLabel->setText(QString("%1 : %2").arg(alphaText).arg(currentLineColor.alpha()));
}

void LineLayerConfigDialog::applyChanges() {
    QMap<QString, QVariant> newParams = getLayerParameters();
    selectedLayer->setParameters(newParams);
    accept();
}

void LineLayerConfigDialog::previewChanges() {
    QMap<QString, QVariant> newParams = getLayerParameters();
    selectedLayer->setParameters(newParams);

    QList<std::shared_ptr<Layer>> layersList;
    layersList.append(std::make_shared<LineLayer>(*selectedLayer));

    LayerDrawDialog* drawDialog = new LayerDrawDialog();
    drawDialog->setLayers(layersList);
    drawDialog->exec();
    delete drawDialog;
}

void LineLayerConfigDialog::cancelChanges() {
    selectedLayer->setParameters(originalParams);
    this->close();
}

void LineLayerConfigDialog::undoChanges() {
    if (!undoStack.isEmpty()) {
        QMap<QString, QVariant> lastParams = undoStack.pop();
        selectedLayer->setParameters(lastParams);
    }
    else {
        selectedLayer->setParameters(originalParams);
    }

    startXSpinBox->setValue(selectedLayer->getParameters()["startX"].toInt());
    startYSpinBox->setValue(selectedLayer->getParameters()["startY"].toInt());
    endXSpinBox->setValue(selectedLayer->getParameters()["endX"].toInt());
    endYSpinBox->setValue(selectedLayer->getParameters()["endY"].toInt());
    lineWidthSpinBox->setValue(selectedLayer->getParameters()["lineWidth"].toInt());

    currentLineColor = selectedLayer->getParameters()["lineColor"].value<QColor>();
    lineColorButton->setStyleSheet("background-color: " + currentLineColor.name());

    updateAlphaLabel();
    updateThumbnail();
}

void LineLayerConfigDialog::pushToUndoStack(const QMap<QString, QVariant>& params) {
    if (undoStack.size() >= maxUndoCount) {
        undoStack.remove(0);
    }
    undoStack.push(params);
}

void LineLayerConfigDialog::updateThumbnail() {
    QMap<QString, QVariant> newParams = getLayerParameters();
    selectedLayer->setParameters(newParams);
    QPixmap thumbnail = generateThumbnail(selectedLayer, thumbWidth, thumbHeight);
    imageLabel->setPixmap(thumbnail);
}