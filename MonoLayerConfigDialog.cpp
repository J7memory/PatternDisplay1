#include "MonoLayerConfigDialog.h"
#include "common.h"
#include "LayerDrawDialog.h"
#include <QGuiApplication>
#include <QScreen>

MonoLayerConfigDialog::MonoLayerConfigDialog(MonoLayer* layer, QWidget* parent)
    : QDialog(parent), selectedLayer(layer) {
    originalParams = layer->getParameters();
    setWindowTitle(tr("Mono Layer Configuration"));
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
    initializeSpinBox(lineWidthSpinBox, "lineWidth", 1, 50000);

    initializeCheckBox(fillCheckBox, tr("Active"), "fill");

    initializeColorButton(colorButton, "color", currentColor, alphaLabel);
    initializeAlphaLabel(alphaLabel, currentColor);

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

QMap<QString, QVariant> MonoLayerConfigDialog::getLayerParameters() const {
    QMap<QString, QVariant> params;
    params["startX"] = startXSpinBox->value();
    params["startY"] = startYSpinBox->value();
    params["width"] = widthSpinBox->value();
    params["height"] = heightSpinBox->value();
    params["lineWidth"] = lineWidthSpinBox->value();
    params["fill"] = fillCheckBox->isChecked();
    params["color"] = currentColor;
    return params;
}

// 创建控件
void MonoLayerConfigDialog::initializeSpinBox(QSpinBox*& spinBox, const QString& paramKey, int minValue, int maxValue) {
    spinBox = new QSpinBox(this);
    spinBox->setRange(minValue, maxValue);
    spinBox->setValue(selectedLayer->getParameters().value(paramKey).toInt());
    spinBox->setFixedWidth(120);
}

void MonoLayerConfigDialog::initializeColorButton(QPushButton*& button, const QString& paramKey, QColor& color, QLabel* alphaLabel) {
    button = new QPushButton(this);
    color = selectedLayer->getParameters().value(paramKey).value<QColor>();
    button->setStyleSheet("background-color: " + color.name());
    button->setFixedWidth(80);
    button->setAutoDefault(false);
}

void MonoLayerConfigDialog::initializeAlphaLabel(QLabel*& alphaLabel, QColor color) {
    alphaLabel = new QLabel(this);
    QString alphaText = tr("Alpha");
    alphaLabel->setText(QString("%1 : %2").arg(alphaText).arg(color.alpha()));
}

void MonoLayerConfigDialog::initializeCheckBox(QCheckBox*& checkBox, const QString& label, const QString& paramKey) {
    checkBox = new QCheckBox(label, this);
    checkBox->setChecked(selectedLayer->getParameters().value(paramKey).toBool());
}

void MonoLayerConfigDialog::initializeButton(QPushButton*& button, const QString& text, int fixedWidth, bool autoDefault) {
    button = new QPushButton(text, this);
    button->setFixedWidth(fixedWidth);
    button->setAutoDefault(autoDefault);
}

void MonoLayerConfigDialog::initializeImageLabel(QLabel*& lable, int thumbWidth, int thumbHeight) {
    lable = new QLabel;
    lable->setFixedSize(thumbWidth, thumbHeight);
    lable->setAlignment(Qt::AlignCenter);
    QPixmap thumbnail = generateThumbnail(selectedLayer, thumbWidth, thumbHeight);
    lable->setPixmap(thumbnail.scaled(lable->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

// 布局
QVBoxLayout* MonoLayerConfigDialog::createLayout() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QHBoxLayout* topButtonLayout = new QHBoxLayout();
    topButtonLayout->addWidget(undoButton);
    //topButtonLayout->addWidget(redoButton);
    topButtonLayout->addStretch();

    QHBoxLayout* contentLayout = new QHBoxLayout;
    contentLayout->setContentsMargins(0, 0, 80, 30);

    // -------- 左侧参数布局 --------
    QVBoxLayout* paramContainer = new QVBoxLayout;
    paramContainer->addStretch();  // 上方留白
    QVBoxLayout* verticalLayout = new QVBoxLayout;
    verticalLayout->setContentsMargins(0, 30, 0, 0);

    QFormLayout* sizeLayout = new QFormLayout;
    sizeLayout->setContentsMargins(60, 0, 0, 0);
    sizeLayout->setSpacing(25);
    sizeLayout->addRow(new QLabel(tr("StartX\t:")), startXSpinBox);
    sizeLayout->addRow(new QLabel(tr("StartY\t:")), startYSpinBox);
    sizeLayout->addRow(new QLabel(tr("Width\t:")), widthSpinBox);
    sizeLayout->addRow(new QLabel(tr("Height\t:")), heightSpinBox);
    sizeLayout->addRow(new QLabel(tr("LineWidth :")), lineWidthSpinBox);
    sizeLayout->addRow(new QLabel(tr("Fill\t:")), fillCheckBox);

    QHBoxLayout* colorLayout = new QHBoxLayout;
    colorLayout->addWidget(colorButton);
    colorLayout->addWidget(alphaLabel);
    sizeLayout->addRow(new QLabel(tr("Color\t:")), colorLayout);

    verticalLayout->addLayout(sizeLayout);
    paramContainer->addLayout(verticalLayout);
    paramContainer->addStretch();

    // -------- 右侧图像布局 --------
    QVBoxLayout* imageContainer = new QVBoxLayout;
    imageContainer->addStretch();
    imageContainer->addWidget(imageLabel, 0, Qt::AlignCenter);
    imageContainer->addStretch();

    contentLayout->addLayout(paramContainer);
    contentLayout->addLayout(imageContainer);

    QHBoxLayout* horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(applyButton);
    horizontalLayout->addWidget(previewButton);
    horizontalLayout->addWidget(cancelButton);

    mainLayout->addLayout(topButtonLayout);
    mainLayout->addSpacing(40);
    mainLayout->addLayout(contentLayout);
    mainLayout->addLayout(horizontalLayout);
    mainLayout->addSpacing(20);

    setLayout(mainLayout);

    return mainLayout;
}


// 信号与槽
void MonoLayerConfigDialog::connectSignals() {
    for (QSpinBox* spinBox : { startXSpinBox, startYSpinBox, widthSpinBox, heightSpinBox, lineWidthSpinBox}) {
        connect(spinBox, &QSpinBox::editingFinished, this, &MonoLayerConfigDialog::chooseSpinBox);
        connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MonoLayerConfigDialog::updateThumbnail);
    }

    for (QPushButton* button : { colorButton }) {
        connect(button, &QPushButton::clicked, this, [this, button]() {
            chooseRectColor();
            updateAlphaLabel();
        });
    }

    for (QCheckBox* checkBox : { fillCheckBox }) {
        connect(checkBox, &QCheckBox::stateChanged, this, &MonoLayerConfigDialog::updateCheckBox);
        connect(checkBox, &QCheckBox::stateChanged, this, &MonoLayerConfigDialog::updateThumbnail);
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

void MonoLayerConfigDialog::chooseSpinBox() {
    int newStartX = startXSpinBox->value();
    int newStartY = startYSpinBox->value();
    int newWidth = widthSpinBox->value();
    int newHeight = heightSpinBox->value();
    int newLineWidth = lineWidthSpinBox->value();
    pushToUndoStack(selectedLayer->getParameters());
}

void MonoLayerConfigDialog::chooseRectColor() {
    chooseColor(currentColor, colorButton, this);
    pushToUndoStack(selectedLayer->getParameters());
    updateThumbnail();
}

void MonoLayerConfigDialog::updateAlphaLabel() {
    QString alphaText = tr("Alpha");
    alphaLabel->setText(QString("%1 : %2").arg(alphaText).arg(currentColor.alpha()));
}

void MonoLayerConfigDialog::updateCheckBox(int state) {
    pushToUndoStack(selectedLayer->getParameters());
    bool isChecked = (state == Qt::Checked);

    QMap<QString, QVariant> newParams = selectedLayer->getParameters();
    newParams["fill"] = isChecked;
    selectedLayer->setParameters(newParams);
}

void MonoLayerConfigDialog::applyChanges() {
    QMap<QString, QVariant> newParams = getLayerParameters();
    selectedLayer->setParameters(newParams);
    accept();
}

void MonoLayerConfigDialog::previewChanges() {
    QMap<QString, QVariant> newParams = getLayerParameters();
    selectedLayer->setParameters(newParams);

    QList<std::shared_ptr<Layer>> layersList;
    layersList.append(std::make_shared<MonoLayer>(*selectedLayer));

    LayerDrawDialog* drawDialog = new LayerDrawDialog();
    drawDialog->setLayers(layersList);
    drawDialog->exec();
    delete drawDialog;
}

void MonoLayerConfigDialog::cancelChanges() {
    selectedLayer->setParameters(originalParams);
    this->close();
}

void MonoLayerConfigDialog::undoChanges() {
    if (!undoStack.isEmpty()) {
        QMap<QString, QVariant> lastParams = undoStack.pop();
        selectedLayer->setParameters(lastParams);
    }
    else {
        selectedLayer->setParameters(originalParams);
    }

    // 更新 UI
    startXSpinBox->setValue(selectedLayer->getParameters()["startX"].toInt());
    startYSpinBox->setValue(selectedLayer->getParameters()["startY"].toInt());
    widthSpinBox->setValue(selectedLayer->getParameters()["width"].toInt());
    heightSpinBox->setValue(selectedLayer->getParameters()["height"].toInt());
    lineWidthSpinBox->setValue(selectedLayer->getParameters()["lineWidth"].toInt());
    fillCheckBox->setChecked(selectedLayer->getParameters()["fill"].toBool());

    currentColor = selectedLayer->getParameters()["color"].value<QColor>();
    colorButton->setStyleSheet("background-color: " + currentColor.name());

    updateAlphaLabel();
    updateThumbnail();
}

void MonoLayerConfigDialog::pushToUndoStack(const QMap<QString, QVariant>& params) {
    if (undoStack.size() >= maxUndoCount) {
        undoStack.remove(0);
    }
    undoStack.push(params);
}

void MonoLayerConfigDialog::updateThumbnail() {
    QMap<QString, QVariant> newParams = getLayerParameters();
    selectedLayer->setParameters(newParams);
    QPixmap thumbnail = generateThumbnail(selectedLayer, thumbWidth, thumbHeight);
    imageLabel->setPixmap(thumbnail);
}