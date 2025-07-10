#include "CRectLayerConfigDialog.h"
#include "common.h"
#include "LayerDrawDialog.h"
#include <QGuiApplication>
#include <QScreen>

CRectLayerConfigDialog::CRectLayerConfigDialog(CRectLayer* layer, QWidget* parent)
    : QDialog(parent), selectedLayer(layer) {
    originalParams = layer->getParameters();
    setWindowTitle(tr("CRect Layer Configuration"));
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
    initializeSpinBox(countsSpinBox, "counts", 1, 256);

    initializeCheckBox(fillCheckBox, tr("Active"), "fill");

    initializeColorButton(color1Button, "color1", currentColor1, color1AlphaLabel);
    initializeColorButton(color2Button, "color2", currentColor2, color2AlphaLabel);

    initializeAlphaLabel(color1AlphaLabel, currentColor1);
    initializeAlphaLabel(color2AlphaLabel, currentColor2);

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

QMap<QString, QVariant> CRectLayerConfigDialog::getLayerParameters() const {
    QMap<QString, QVariant> params;
    params["startX"] = startXSpinBox->value();
    params["startY"] = startYSpinBox->value();
    params["width"] = widthSpinBox->value();
    params["height"] = heightSpinBox->value();
    params["counts"] = countsSpinBox->value();
    params["fill"] = fillCheckBox->isChecked();
    params["color1"] = currentColor1;
    params["color2"] = currentColor2;
    return params;
}

// 创建控件
void CRectLayerConfigDialog::initializeSpinBox(QSpinBox*& spinBox, const QString& paramKey, int minValue, int maxValue) {
    spinBox = new QSpinBox(this);
    spinBox->setRange(minValue, maxValue);
    spinBox->setValue(selectedLayer->getParameters().value(paramKey).toInt());
    spinBox->setFixedWidth(120);
}

void CRectLayerConfigDialog::initializeColorButton(QPushButton*& button, const QString& paramKey, QColor& color, QLabel* alphaLabel) {
    button = new QPushButton(this);
    color = selectedLayer->getParameters().value(paramKey).value<QColor>();
    button->setStyleSheet("background-color: " + color.name());
    button->setFixedWidth(80);
    button->setAutoDefault(false);
}

void CRectLayerConfigDialog::initializeAlphaLabel(QLabel*& alphaLabel, QColor color) {
    alphaLabel = new QLabel(this);
    QString alphaText = tr("Alpha");
    alphaLabel->setText(QString("%1 : %2").arg(alphaText).arg(color.alpha()));
}

void CRectLayerConfigDialog::initializeCheckBox(QCheckBox*& checkBox, const QString& label, const QString& paramKey) {
    checkBox = new QCheckBox(label, this);
    checkBox->setChecked(selectedLayer->getParameters().value(paramKey).toBool());
}

void CRectLayerConfigDialog::initializeButton(QPushButton*& button, const QString& text, int fixedWidth, bool autoDefault) {
    button = new QPushButton(text, this);
    button->setFixedWidth(fixedWidth);
    button->setAutoDefault(autoDefault);
}

void CRectLayerConfigDialog::initializeImageLabel(QLabel*& lable, int thumbWidth, int thumbHeight) {
    lable = new QLabel;
    lable->setFixedSize(thumbWidth, thumbHeight);
    lable->setAlignment(Qt::AlignCenter);
    QPixmap thumbnail = generateThumbnail(selectedLayer, thumbWidth, thumbHeight);
    lable->setPixmap(thumbnail.scaled(lable->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

// 布局
QVBoxLayout* CRectLayerConfigDialog::createLayout() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QHBoxLayout* topButtonLayout = new QHBoxLayout();
    topButtonLayout->addWidget(undoButton);
    //topButtonLayout->addWidget(redoButton);
    topButtonLayout->addStretch();

    // ---- 创建水平布局，包含左侧参数设置和右侧图片 ----
    QHBoxLayout* contentLayout = new QHBoxLayout;
    contentLayout->setContentsMargins(0, 0, 80, 80);

    QVBoxLayout* verticalLayout = new QVBoxLayout;
    verticalLayout->setContentsMargins(0, 30, 0, 0);
    QFormLayout* sizeLayout = new QFormLayout;
    sizeLayout->setContentsMargins(60, 0, 0, 0);
    sizeLayout->setSpacing(25);
    sizeLayout->addRow(new QLabel(tr("StartX\t:")), startXSpinBox);
    sizeLayout->addRow(new QLabel(tr("StartY\t:")), startYSpinBox);
    sizeLayout->addRow(new QLabel(tr("Width\t:")), widthSpinBox);
    sizeLayout->addRow(new QLabel(tr("Height\t:")), heightSpinBox);
    sizeLayout->addRow(new QLabel(tr("Counts\t:")), countsSpinBox);
    sizeLayout->addRow(new QLabel(tr("Fill\t:")), fillCheckBox);

    QHBoxLayout* color1Layout = new QHBoxLayout;
    color1Layout->addWidget(color1Button);
    color1Layout->addWidget(color1AlphaLabel);
    sizeLayout->addRow(new QLabel(tr("Color 1\t:")), color1Layout);

    QHBoxLayout* color2Layout = new QHBoxLayout;
    color2Layout->addWidget(color2Button);
    color2Layout->addWidget(color2AlphaLabel);
    sizeLayout->addRow(new QLabel(tr("Color 2\t:")), color2Layout);

    verticalLayout->addLayout(sizeLayout);

    contentLayout->addLayout(verticalLayout);
    contentLayout->addWidget(imageLabel);

    QHBoxLayout* horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(applyButton);
    horizontalLayout->addWidget(previewButton);
    horizontalLayout->addWidget(cancelButton);

    // ---- 主布局 ----
    mainLayout->addLayout(topButtonLayout);
    mainLayout->addSpacing(40);
    mainLayout->addLayout(contentLayout);
    mainLayout->addLayout(horizontalLayout);
    mainLayout->addSpacing(20);

    setLayout(mainLayout);

    return mainLayout;
}

// 信号与槽
void CRectLayerConfigDialog::connectSignals() {
    for (QSpinBox* spinBox : { startXSpinBox, startYSpinBox, widthSpinBox, heightSpinBox, countsSpinBox }) {
        connect(spinBox, &QSpinBox::editingFinished, this, &CRectLayerConfigDialog::chooseSpinBox);
        connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &CRectLayerConfigDialog::updateThumbnail);
    }

    for (QCheckBox* checkBox : { fillCheckBox }) {
        connect(checkBox, &QCheckBox::stateChanged, this, &CRectLayerConfigDialog::updateCheckBox);
        connect(checkBox, &QCheckBox::stateChanged, this, &CRectLayerConfigDialog::updateThumbnail);
    }

    for (QPushButton* button : { color1Button, color2Button }) {
        connect(button, &QPushButton::clicked, this, [this, button]() {
            (button == color1Button ? chooseColor1() : chooseColor2());
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

void CRectLayerConfigDialog::chooseSpinBox() {
    int newStartX = startXSpinBox->value();
    int newStartY = startYSpinBox->value();
    int newWidth = widthSpinBox->value();
    int newHeight = heightSpinBox->value();
    int newCounts = countsSpinBox->value();
    pushToUndoStack(selectedLayer->getParameters());
}

void CRectLayerConfigDialog::chooseColor1() {
    chooseColor(currentColor1, color1Button, this);
    pushToUndoStack(selectedLayer->getParameters());
    updateThumbnail();
}

void CRectLayerConfigDialog::chooseColor2() {
    chooseColor(currentColor2, color2Button, this);
    pushToUndoStack(selectedLayer->getParameters());
    updateThumbnail();
}

void CRectLayerConfigDialog::updateCheckBox(int state) {
    pushToUndoStack(selectedLayer->getParameters());
    bool isChecked = (state == Qt::Checked);

    QMap<QString, QVariant> newParams = selectedLayer->getParameters();
    newParams["fill"] = isChecked;
    selectedLayer->setParameters(newParams);
}

void CRectLayerConfigDialog::updateAlphaLabel() {
    QString alphaText = tr("Alpha");  
    color1AlphaLabel->setText(QString("%1 : %2").arg(alphaText).arg(currentColor1.alpha()));
    color2AlphaLabel->setText(QString("%1 : %2").arg(alphaText).arg(currentColor2.alpha()));
}

void CRectLayerConfigDialog::applyChanges() {
    QMap<QString, QVariant> newParams = getLayerParameters();
    selectedLayer->setParameters(newParams);
    accept();
}

void CRectLayerConfigDialog::previewChanges() {
    QMap<QString, QVariant> newParams = getLayerParameters();
    selectedLayer->setParameters(newParams);

    QList<std::shared_ptr<Layer>> layersList;
    layersList.append(std::make_shared<CRectLayer>(*selectedLayer));

    LayerDrawDialog* drawDialog = new LayerDrawDialog();
    drawDialog->setLayers(layersList);
    drawDialog->exec();
    delete drawDialog;
}

void CRectLayerConfigDialog::cancelChanges() {
    selectedLayer->setParameters(originalParams);
    this->close();
}

void CRectLayerConfigDialog::undoChanges() {
    if (!undoStack.isEmpty()) {
        QMap<QString, QVariant> lastParams = undoStack.pop();
        selectedLayer->setParameters(lastParams);
    }
    else {
        selectedLayer->setParameters(originalParams);
    }

    startXSpinBox->setValue(selectedLayer->getParameters()["startX"].toInt());
    startYSpinBox->setValue(selectedLayer->getParameters()["startY"].toInt());
    widthSpinBox->setValue(selectedLayer->getParameters()["width"].toInt());
    heightSpinBox->setValue(selectedLayer->getParameters()["height"].toInt());
    countsSpinBox->setValue(selectedLayer->getParameters()["counts"].toInt());
    fillCheckBox->setChecked(selectedLayer->getParameters()["fill"].toBool());

    currentColor1 = selectedLayer->getParameters()["color1"].value<QColor>();
    currentColor2 = selectedLayer->getParameters()["color2"].value<QColor>();
    color1Button->setStyleSheet("background-color: " + currentColor1.name());
    color2Button->setStyleSheet("background-color: " + currentColor2.name());

    updateAlphaLabel();
    updateThumbnail();
}

void CRectLayerConfigDialog::pushToUndoStack(const QMap<QString, QVariant>& params) {
    if (undoStack.size() >= maxUndoCount) {
        undoStack.remove(0);
    }
    undoStack.push(params);
}

void CRectLayerConfigDialog::updateThumbnail() {
    QMap<QString, QVariant> newParams = getLayerParameters();
    selectedLayer->setParameters(newParams);
    QPixmap thumbnail = generateThumbnail(selectedLayer, thumbWidth, thumbHeight);
    imageLabel->setPixmap(thumbnail);
}