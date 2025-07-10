#include "common.h"
#include "GradientLayerConfigDialog.h"
#include "LayerDrawDialog.h"
#include <QDebug>
#include <QFormLayout>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QScreen>
#include <QVBoxLayout>

GradientLayerConfigDialog::GradientLayerConfigDialog(GradientLayer* layer, QWidget* parent)
    : QDialog(parent), selectedLayer(layer), currentGradientIndex(0) {
    originalParams = layer->getParameters();
    setWindowTitle(tr("Gradient Layer Configuration"));
    resize(900, 600);

    QMap<QString, QVariant> params = selectedLayer->getParameters();

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
    initializeSpinBox(countsSpinBox, "counts", 1, 16);
    initializeSpinBox(stepsSpinBox, "steps", 1, 256);

    initializeComboBox(orientationComboBox, { tr("Horizontal"), tr("Vertical") }, "orientation");

    initializeColorButton(startColorButton, "startColor", currentStartColor, sCAlphaLabel);
    initializeColorButton(endColorButton, "endColor", currentEndColor, eCAlphaLabel);

    initializeAlphaLabel(sCAlphaLabel, currentStartColor);
    initializeAlphaLabel(eCAlphaLabel, currentEndColor);

    gradientList = new QListWidget(this);
    gradientList->setMaximumWidth(160);
    gradientList->setMaximumHeight(280);

    initializeImageLabel(imageLabel, 400, 400);

    initializeButton(applyButton, tr("Apply"), 80, false);
    initializeButton(previewButton, tr("Preview"), 80, false);
    initializeButton(cancelButton, tr("Cancel"), 80, false);

    initializeButton(undoButton, QString::fromUtf8("\u2190"), 40, false);
    
    // ---- 布局 ----
    createLayout();

    // ---- 连接信号与槽 ----
    connectSignals();

    updateGradientList();

    if (params.contains("gradientBars")) {
        gradientBars = params["gradientBars"].value<QList<QMap<QString, QVariant>>>();
        gradientList->setCurrentRow(0);
    }

    updateThumbnail();
}

QMap<QString, QVariant> GradientLayerConfigDialog::getLayerParameters() const {
    QMap<QString, QVariant> params;
    params["startX"] = startXSpinBox->value();
    params["startY"] = startYSpinBox->value();
    params["width"] = widthSpinBox->value();
    params["height"] = heightSpinBox->value();
    params["counts"] = countsSpinBox->value();
    params["orientation"] = orientationComboBox->currentText();
    params["gradientBars"] = QVariant::fromValue(gradientBars);
    return params;
}

// 创建控件
void GradientLayerConfigDialog::initializeSpinBox(QSpinBox*& spinBox, const QString& paramKey, int minValue, int maxValue) {
    spinBox = new QSpinBox(this);
    spinBox->setRange(minValue, maxValue);
    spinBox->setValue(selectedLayer->getParameters().value(paramKey).toInt());
    spinBox->setFixedWidth(120);
}

void GradientLayerConfigDialog::initializeComboBox(QComboBox*& comboBox, const QStringList& items, const QString& paramKey) {
    comboBox = new QComboBox(this);
    comboBox->addItems(items);
    comboBox->setCurrentText(selectedLayer->getParameters().value(paramKey).toString());
    comboBox->setFixedWidth(120);
}

void GradientLayerConfigDialog::initializeColorButton(QPushButton*& button, const QString& paramKey, QColor& color, QLabel* alphaLabel) {
    button = new QPushButton(this);
    color = selectedLayer->getParameters().value(paramKey).value<QColor>();
    button->setStyleSheet("background-color: " + color.name());
    button->setFixedWidth(80);
    button->setAutoDefault(false);
}

void GradientLayerConfigDialog::initializeAlphaLabel(QLabel*& alphaLabel, QColor color) {
    alphaLabel = new QLabel(this);
    QString alphaText = tr("Alpha");
    alphaLabel->setText(QString("%1 : %2").arg(alphaText).arg(color.alpha()));
}

void GradientLayerConfigDialog::initializeButton(QPushButton*& button, const QString& text, int fixedWidth, bool autoDefault) {
    button = new QPushButton(text, this);
    button->setFixedWidth(fixedWidth);
    button->setAutoDefault(autoDefault);
}

void GradientLayerConfigDialog::initializeImageLabel(QLabel*& lable, int thumbWidth, int thumbHeight) {
    lable = new QLabel;
    lable->setFixedSize(thumbWidth, thumbHeight);
    lable->setAlignment(Qt::AlignCenter);
    QPixmap thumbnail = generateThumbnail(selectedLayer, thumbWidth, thumbHeight);
    lable->setPixmap(thumbnail.scaled(lable->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

// 布局
QVBoxLayout* GradientLayerConfigDialog::createLayout() {
    // 布局设置
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QHBoxLayout* topButtonLayout = new QHBoxLayout();
    topButtonLayout->addWidget(undoButton);
    //topButtonLayout->addWidget(redoButton);
    topButtonLayout->addStretch();

    // 创建第一个垂直布局 (sizeLayout1)
    QVBoxLayout* verticalLayout1 = new QVBoxLayout;
    QFormLayout* sizeLayout1 = new QFormLayout();
    sizeLayout1->setSpacing(25);
    sizeLayout1->addRow(new QLabel(tr("StartX\t:")), startXSpinBox);
    sizeLayout1->addRow(new QLabel(tr("StartY\t:")), startYSpinBox);
    sizeLayout1->addRow(new QLabel(tr("Width\t:")), widthSpinBox);
    sizeLayout1->addRow(new QLabel(tr("Height\t:")), heightSpinBox);

    verticalLayout1->addLayout(sizeLayout1);

    // 创建第二个垂直布局 (sizeLayout2)
    QVBoxLayout* verticalLayout2 = new QVBoxLayout;
    QFormLayout* sizeLayout2 = new QFormLayout();
    sizeLayout2->setSpacing(25);
    sizeLayout2->addRow(new QLabel(tr("Orientation:")), orientationComboBox);
    sizeLayout2->addRow(new QLabel(tr("Counts\t:")), countsSpinBox);
    sizeLayout2->addRow(new QLabel(tr("Steps\t:")), stepsSpinBox);

    QHBoxLayout* startColorLayout = new QHBoxLayout;
    startColorLayout->addWidget(startColorButton);
    startColorLayout->addWidget(sCAlphaLabel);
    sizeLayout2->addRow(new QLabel(tr("Start Color:")), startColorLayout);

    QHBoxLayout* endColorLayout = new QHBoxLayout;
    endColorLayout->addWidget(endColorButton);
    endColorLayout->addWidget(eCAlphaLabel);
    sizeLayout2->addRow(new QLabel(tr("End Color:")), endColorLayout);

    verticalLayout2->addLayout(sizeLayout2);

    // 水平布局1，包含两个垂直布局
    QHBoxLayout* horizontalLayout1 = new QHBoxLayout;
    horizontalLayout1->addLayout(verticalLayout1);
    QSpacerItem* spacer = new QSpacerItem(120, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);
    horizontalLayout1->addItem(spacer);
    horizontalLayout1->addLayout(verticalLayout2);
    horizontalLayout1->setAlignment(Qt::AlignHCenter);

    QHBoxLayout* horizontalLayout2 = new QHBoxLayout;
    horizontalLayout2->addWidget(gradientList);
    horizontalLayout2->addWidget(imageLabel);

    // 水平布局3，按钮
    QHBoxLayout* horizontalLayout3 = new QHBoxLayout;
    horizontalLayout3->addWidget(applyButton);
    horizontalLayout3->addWidget(previewButton);
    horizontalLayout3->addWidget(cancelButton);

    // 添加到主布局
    mainLayout->addLayout(topButtonLayout);
    mainLayout->addSpacing(20);
    mainLayout->addLayout(horizontalLayout1);
    mainLayout->addLayout(horizontalLayout2);
    mainLayout->addLayout(horizontalLayout3);
    mainLayout->addSpacing(20);
    // 设置主布局
    setLayout(mainLayout);

    return mainLayout;
}

// 信号与槽
void GradientLayerConfigDialog::connectSignals() {
    for (QSpinBox* spinBox : { startXSpinBox, startYSpinBox, widthSpinBox, heightSpinBox, countsSpinBox, stepsSpinBox }) {
        connect(spinBox, &QSpinBox::editingFinished, this, &GradientLayerConfigDialog::chooseSpinBox);
        connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &GradientLayerConfigDialog::updateThumbnail);

        if (spinBox == countsSpinBox) {
            connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &GradientLayerConfigDialog::updateGradientList);
            connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &GradientLayerConfigDialog::updateThumbnail);
        }
        if (spinBox == stepsSpinBox) {
            connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &GradientLayerConfigDialog::saveCurrentGradient);
            connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &GradientLayerConfigDialog::updateThumbnail);
        }
    }

    for (QComboBox* comboBox : { orientationComboBox }) {
        connect(comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &GradientLayerConfigDialog::chooseComboBox);
    }

    for (QPushButton* button : { startColorButton, endColorButton }) {
        connect(button, &QPushButton::clicked, this, [this, button]() {
            (button == startColorButton ? chooseStartColor() : chooseEndColor());
            updateAlphaLabel();
        });

    }

    connect(gradientList, &QListWidget::currentRowChanged, this, &GradientLayerConfigDialog::loadSelectedGradient);
    connect(gradientList, &QListWidget::currentRowChanged, this, &GradientLayerConfigDialog::updateAlphaLabel);

    for (QPushButton* button : { applyButton, previewButton, cancelButton, undoButton }) {
        connect(button, &QPushButton::clicked, this, [this, button]() {
            if (button == applyButton) applyChanges();
            else if (button == previewButton) previewChanges();
            else if (button == cancelButton) cancelChanges();
            else if (button == undoButton) undoChanges();
            });
    }
}

void GradientLayerConfigDialog::chooseSpinBox() {
    int newStartX = startXSpinBox->value();
    int newStartY = startYSpinBox->value();
    int newWidth = widthSpinBox->value();
    int newHeight = heightSpinBox->value();
    int newCounts = countsSpinBox->value();
    int newSteps = stepsSpinBox->value();
    pushToUndoStack(selectedLayer->getParameters());
}

void GradientLayerConfigDialog::chooseComboBox() {
    QString newOrientation = orientationComboBox->currentText();
    pushToUndoStack(selectedLayer->getParameters());
    updateThumbnail();
}

void GradientLayerConfigDialog::chooseStartColor() {
    chooseColor(currentStartColor, startColorButton, this);
    pushToUndoStack(selectedLayer->getParameters());
    saveCurrentGradient();
    updateThumbnail();
}

void GradientLayerConfigDialog::chooseEndColor() {
    chooseColor(currentEndColor, endColorButton, this);
    pushToUndoStack(selectedLayer->getParameters());
    saveCurrentGradient();
    updateThumbnail();
}

void GradientLayerConfigDialog::updateAlphaLabel() {
    QString alphaText = tr("Alpha");  // 翻译 "Alpha" 为 "透明度"
    sCAlphaLabel->setText(QString("%1 : %2").arg(alphaText).arg(currentStartColor.alpha()));
    eCAlphaLabel->setText(QString("%1 : %2").arg(alphaText).arg(currentEndColor.alpha()));
}

void GradientLayerConfigDialog::loadSelectedGradient() {
    int index = gradientList->currentRow();
    if (index < 0 || index >= gradientBars.size())
        return;

    currentGradientIndex = index;
    QMap<QString, QVariant> gradient = gradientBars[index];

    if (gradient["startColor"].canConvert<QColor>()) {
        currentStartColor = gradient["startColor"].value<QColor>();
    }
    if (gradient["endColor"].canConvert<QColor>()) {
        currentEndColor = gradient["endColor"].value<QColor>();
    }

    stepsSpinBox->setValue(gradient["steps"].toInt());
    startColorButton->setStyleSheet(QString("background-color: %1").arg(currentStartColor.name(QColor::HexArgb)));
    endColorButton->setStyleSheet(QString("background-color: %1").arg(currentEndColor.name(QColor::HexArgb)));
}

void GradientLayerConfigDialog::updateGradientList() {
    int newCount = countsSpinBox->value();
    int currentCount = gradientBars.size();

    if (currentCount == newCount) return;
    // 记录是否新增了条目
    bool addedNewItems = (newCount > currentCount);
    bool removedItems = (newCount < currentCount);

    if (currentCount < newCount) {
        QList<QColor> defaultColors = { QColor(255, 255, 255, 255), QColor(255, 0, 0, 255), QColor(0, 255, 0, 255), QColor(0, 0, 255, 255) };
        for (int i = currentCount; i < newCount; ++i) {
            QMap<QString, QVariant> gradient;
            gradient["startColor"] = QColor(0, 0, 0, 255);
            gradient["endColor"] = (i < defaultColors.size()) ? defaultColors[i] : defaultColors.last();
            gradient["steps"] = 9;
            gradientBars.append(gradient);
        }
    }else {
        gradientBars.resize(newCount);
    }

    gradientList->blockSignals(true);
    gradientList->clear();
    for (int i = 0; i < gradientBars.size(); ++i) {
        gradientList->addItem(QString("Gradient %1").arg(i + 1));
    }
    gradientList->blockSignals(false);

    if (addedNewItems) {
        int lastIndex = newCount - 1;
        gradientList->setCurrentRow(lastIndex);
        gradientList->scrollToItem(gradientList->item(lastIndex));
        loadSelectedGradient();
    }
    else if (removedItems) {
        int newLastIndex = newCount - 1;
        if (newLastIndex >= 0) {
            gradientList->setCurrentRow(newLastIndex);
            loadSelectedGradient();
        }
        else {
            startColorButton->setStyleSheet("");
            endColorButton->setStyleSheet("");
        }
    }
    else if (gradientList->currentRow() == -1 && !gradientBars.isEmpty()) {
        gradientList->setCurrentRow(0);
        loadSelectedGradient();
    }
}

void GradientLayerConfigDialog::saveCurrentGradient() {
    if (currentGradientIndex < 0 || currentGradientIndex >= gradientBars.size())
        return;

    QMap<QString, QVariant> gradient;
    gradient["startColor"] = currentStartColor;
    gradient["endColor"] = currentEndColor;
    gradient["steps"] = stepsSpinBox->value();

    gradientBars[currentGradientIndex] = gradient;
}

void GradientLayerConfigDialog::applyChanges() {
    saveCurrentGradient();

     QMap<QString, QVariant> newParams = getLayerParameters();
     selectedLayer->setParameters(newParams);
     accept();
}

void GradientLayerConfigDialog::previewChanges() {
    QMap<QString, QVariant> newParams = getLayerParameters();
    selectedLayer->setParameters(newParams);

    QList<std::shared_ptr<Layer>> layersList;
    layersList.append(std::make_shared<GradientLayer>(*selectedLayer));

    LayerDrawDialog* drawDialog = new LayerDrawDialog();
    drawDialog->setLayers(layersList);
    drawDialog->exec();
    delete drawDialog;
}

void GradientLayerConfigDialog::cancelChanges() {
    selectedLayer->setParameters(originalParams);
    this->close();
}

void GradientLayerConfigDialog::undoChanges() {
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
    orientationComboBox->setCurrentText(selectedLayer->getParameters()["orientation"].toString());
    gradientBars = selectedLayer->getParameters()["gradientBars"].value<QList<QMap<QString, QVariant>>>();

    updateAlphaLabel();

    gradientList->clear();
    for (int i = 0; i < gradientBars.size(); ++i) {
        gradientList->addItem(QString("Gradient %1").arg(i + 1));
    }

    if (!gradientBars.isEmpty()) {
        loadSelectedGradient();
    }
    else {
        startColorButton->setStyleSheet("");
        endColorButton->setStyleSheet("");
    }

    updateThumbnail();
}

void GradientLayerConfigDialog::pushToUndoStack(const QMap<QString, QVariant>& params) {
    if (undoStack.size() >= maxUndoCount) {
        undoStack.remove(0);
    }
    undoStack.push(params);
}

void GradientLayerConfigDialog::updateThumbnail() {
    QMap<QString, QVariant> newParams = getLayerParameters();
    selectedLayer->setParameters(newParams);
    QPixmap thumbnail = generateThumbnail(selectedLayer, thumbWidth, thumbHeight);
    imageLabel->setPixmap(thumbnail);
}