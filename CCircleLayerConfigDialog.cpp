#include "CCircleLayerConfigDialog.h"
#include "common.h"
#include <QFormLayout>
#include <QLabel>
#include <QColorDialog>
#include "LayerDrawDialog.h"
#include <QGuiApplication>
#include <QScreen>

CCircleLayerConfigDialog::CCircleLayerConfigDialog(CCircleLayer* layer, QWidget* parent)
    : QDialog(parent), selectedLayer(layer) {
    originalParams = layer->getParameters();
    setWindowTitle(tr("CCircle Layer Configuration"));
    setFixedSize(900, 600);

    //QScreen* screen = QGuiApplication::primaryScreen();
    //QRect geometry = screen->geometry();
    //qreal pixelRatio = screen->devicePixelRatio();

    //int width = geometry.width() * pixelRatio;
    //int height = geometry.height() * pixelRatio;

    initializeSpinBox(centerXSpinBox, "centerX", 0, 100000);
    initializeSpinBox(centerYSpinBox, "centerY", 0, 100000);
    initializeSpinBox(radiusXSpinBox, "radiusX", 0, 50000);
    initializeSpinBox(radiusYSpinBox, "radiusY", 0, 50000);
    initializeSpinBox(countsSpinBox, "counts", 1, 256);
    initializeSpinBox(lineWidthSpinBox, "lineWidth", 1, 200);

    initializeCheckBox(concentricCheckBox, tr("Active"), "borderFillColor");

    initializeColorButton(colorButton, "color", currentColor, colorAlphaLabel);
    initializeAlphaLabel(colorAlphaLabel, currentColor);

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

QMap<QString, QVariant> CCircleLayerConfigDialog::getLayerParameters() const {
    QMap<QString, QVariant> params;
    params["centerX"] = centerXSpinBox->value();
    params["centerY"] = centerYSpinBox->value();
    params["radiusX"] = radiusXSpinBox->value();
    params["radiusY"] = radiusYSpinBox->value();
    params["counts"] = countsSpinBox->value();
    params["concentric"] = concentricCheckBox->isChecked();
    params["lineWidth"] = lineWidthSpinBox->value();
    params["color"] = currentColor;
    return params;
}

// 创建控件
void CCircleLayerConfigDialog::initializeSpinBox(QSpinBox*& spinBox, const QString& paramKey, int minValue, int maxValue) {
    spinBox = new QSpinBox(this);
    spinBox->setRange(minValue, maxValue);
    spinBox->setValue(selectedLayer->getParameters().value(paramKey).toInt());
    spinBox->setFixedWidth(120);
}

void CCircleLayerConfigDialog::initializeColorButton(QPushButton*& button, const QString& paramKey, QColor& color, QLabel* alphaLabel) {
    button = new QPushButton(this);
    color = selectedLayer->getParameters().value(paramKey).value<QColor>();
    button->setStyleSheet("background-color: " + color.name());
    button->setFixedWidth(80);
    button->setAutoDefault(false);
}

void CCircleLayerConfigDialog::initializeAlphaLabel(QLabel*& alphaLabel, QColor color) {
    alphaLabel = new QLabel(this);
    QString alphaText = tr("Alpha");
    alphaLabel->setText(QString("%1 : %2").arg(alphaText).arg(color.alpha()));
}

void CCircleLayerConfigDialog::initializeCheckBox(QCheckBox*& checkBox, const QString& label, const QString& paramKey) {
    checkBox = new QCheckBox(label, this);
    checkBox->setChecked(selectedLayer->getParameters().value(paramKey).toBool());
    concentricCheckBox->setChecked(selectedLayer->getParameters().value("concentric").toBool());
}

void CCircleLayerConfigDialog::initializeButton(QPushButton*& button, const QString& text, int fixedWidth, bool autoDefault) {
    button = new QPushButton(text, this);
    button->setFixedWidth(fixedWidth);
    button->setAutoDefault(autoDefault);
}

void CCircleLayerConfigDialog::initializeImageLabel(QLabel*& lable, int thumbWidth, int thumbHeight) {
    lable = new QLabel;
    lable->setFixedSize(thumbWidth, thumbHeight);
    lable->setAlignment(Qt::AlignCenter);
    QPixmap thumbnail = generateThumbnail(selectedLayer, thumbWidth, thumbHeight);
    lable->setPixmap(thumbnail.scaled(lable->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

// 布局
QVBoxLayout* CCircleLayerConfigDialog::createLayout() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QHBoxLayout* topButtonLayout = new QHBoxLayout();
    topButtonLayout->addWidget(undoButton);
    //topButtonLayout->addWidget(redoButton);
    topButtonLayout->addStretch();

    QHBoxLayout* contentLayout = new QHBoxLayout;
    contentLayout->setContentsMargins(0, 0, 80, 80);

    // -------- 左侧参数布局 --------
    QVBoxLayout* paramContainer = new QVBoxLayout;
    paramContainer->addStretch();  // 上方留白
    QVBoxLayout* verticalLayout = new QVBoxLayout;
    verticalLayout->setContentsMargins(0, 30, 0, 0);
    QFormLayout* sizeLayout = new QFormLayout;
    sizeLayout->setContentsMargins(60, 0, 0, 0);
    sizeLayout->setSpacing(25);
    sizeLayout->addRow(new QLabel(tr("CenterX\t:")), centerXSpinBox);
    sizeLayout->addRow(new QLabel(tr("CenterY\t:")), centerYSpinBox);
    sizeLayout->addRow(new QLabel(tr("RadiusX\t:")), radiusXSpinBox);
    sizeLayout->addRow(new QLabel(tr("RadiusY\t:")), radiusYSpinBox);

    QHBoxLayout* horizontalLayout1 = new QHBoxLayout;
    horizontalLayout1->addWidget(countsSpinBox);
    horizontalLayout1->addWidget(concentricCheckBox);
    sizeLayout->addRow(new QLabel(tr("Counts\t:")), horizontalLayout1);
    sizeLayout->addRow(new QLabel(tr("LineWidth :")), lineWidthSpinBox);

    QHBoxLayout* colorLayout = new QHBoxLayout;
    colorLayout->addWidget(colorButton);
    colorLayout->addWidget(colorAlphaLabel);
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
    contentLayout->addSpacing(20);
    contentLayout->addLayout(imageContainer);

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
void CCircleLayerConfigDialog::connectSignals() {
    for (QSpinBox* spinBox : { centerXSpinBox, centerYSpinBox, radiusXSpinBox, radiusYSpinBox, countsSpinBox, lineWidthSpinBox }) {
        connect(spinBox, &QSpinBox::editingFinished, this, &CCircleLayerConfigDialog::chooseSpinBox);
        connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &CCircleLayerConfigDialog::updateThumbnail);
    }

    for (QPushButton* button : { colorButton }) {
        connect(button, &QPushButton::clicked, this, [this, button]() {
            chooseCircleColor();
            updateAlphaLabel();
            });
    }

    for (QCheckBox* checkBox : { concentricCheckBox }) {
        connect(checkBox, &QCheckBox::stateChanged, this, &CCircleLayerConfigDialog::updateCheckBox);
        connect(checkBox, &QCheckBox::stateChanged, this, &CCircleLayerConfigDialog::updateThumbnail);
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

void CCircleLayerConfigDialog::chooseSpinBox() {
    int newCenterX = centerXSpinBox->value();
    int newCenterY = centerYSpinBox->value();
    int newRadiusX = radiusXSpinBox->value();
    int newRadiusY = radiusYSpinBox->value();
    int newCounts = countsSpinBox->value();
    int newLineWidth = lineWidthSpinBox->value();
    pushToUndoStack(selectedLayer->getParameters());
}

void CCircleLayerConfigDialog::chooseCircleColor() {
    chooseColor(currentColor, colorButton, this);
    pushToUndoStack(selectedLayer->getParameters());
    updateThumbnail();
}

void CCircleLayerConfigDialog::updateAlphaLabel() {
    QString alphaText = tr("Alpha");
    colorAlphaLabel->setText(QString("%1 : %2").arg(alphaText).arg(currentColor.alpha()));
}

void CCircleLayerConfigDialog::updateCheckBox(int state) {
    pushToUndoStack(selectedLayer->getParameters());
    bool isChecked = (state == Qt::Checked);

    QMap<QString, QVariant> newParams = selectedLayer->getParameters();
    newParams["concentric"] = isChecked;
    selectedLayer->setParameters(newParams);
}

void CCircleLayerConfigDialog::applyChanges() {
    QMap<QString, QVariant> newParams = getLayerParameters();
    selectedLayer->setParameters(newParams);
    accept();
}

void CCircleLayerConfigDialog::previewChanges() {
    QMap<QString, QVariant> newParams = getLayerParameters();
    selectedLayer->setParameters(newParams);

    QList<std::shared_ptr<Layer>> layersList;
    layersList.append(std::make_shared<CCircleLayer>(*selectedLayer));

    LayerDrawDialog* drawDialog = new LayerDrawDialog();
    drawDialog->setLayers(layersList);
    drawDialog->exec();
    delete drawDialog;
}

void CCircleLayerConfigDialog::cancelChanges() {
    selectedLayer->setParameters(originalParams);
    this->close();
}

void CCircleLayerConfigDialog::undoChanges() {
    if (!undoStack.isEmpty()) {
        QMap<QString, QVariant> lastParams = undoStack.pop();
        selectedLayer->setParameters(lastParams);
    }
    else {
        selectedLayer->setParameters(originalParams);
    }

    centerXSpinBox->setValue(selectedLayer->getParameters()["centerX"].toInt());
    centerYSpinBox->setValue(selectedLayer->getParameters()["centerY"].toInt());
    radiusXSpinBox->setValue(selectedLayer->getParameters()["radiusX"].toInt());
    radiusYSpinBox->setValue(selectedLayer->getParameters()["radiusY"].toInt());
    countsSpinBox->setValue(selectedLayer->getParameters()["counts"].toInt());
    concentricCheckBox->setChecked(selectedLayer->getParameters()["concentric"].toBool());
    lineWidthSpinBox->setValue(selectedLayer->getParameters()["lineWidth"].toInt());

    currentColor = selectedLayer->getParameters()["color"].value<QColor>();
    colorButton->setStyleSheet("background-color: " + currentColor.name());

    updateAlphaLabel();
    updateThumbnail();
}

void CCircleLayerConfigDialog::pushToUndoStack(const QMap<QString, QVariant>& params) {
    if (undoStack.size() >= maxUndoCount) {
        undoStack.remove(0);
    }
    undoStack.push(params);
}

void CCircleLayerConfigDialog::updateThumbnail() {
    QMap<QString, QVariant> newParams = getLayerParameters();
    selectedLayer->setParameters(newParams);
    QPixmap thumbnail = generateThumbnail(selectedLayer, thumbWidth, thumbHeight);
    imageLabel->setPixmap(thumbnail);
}