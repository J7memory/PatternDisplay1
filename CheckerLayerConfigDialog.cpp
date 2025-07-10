#include "CheckerLayerConfigDialog.h"
#include "common.h"
#include <QFormLayout>
#include <QLabel>
#include <QColorDialog>
#include "LayerDrawDialog.h"
#include <QGuiApplication>
#include <QScreen>


CheckerLayerConfigDialog::CheckerLayerConfigDialog(CheckerLayer* layer, QWidget* parent)
    : QDialog(parent), selectedLayer(layer) {
    originalParams = layer->getParameters();
    setWindowTitle(tr("Checker Layer Configuration"));
    setFixedSize(900, 600);

    //QScreen* screen = QGuiApplication::primaryScreen();
    //QRect geometry = screen->geometry();
    //qreal pixelRatio = screen->devicePixelRatio();

    //int width = geometry.width() * pixelRatio;
    //int height = geometry.height() * pixelRatio;

    // ---- �����ؼ� ----
    initializeSpinBox(startXSpinBox, "startX", 0, 100000);
    initializeSpinBox(startYSpinBox, "startY", 0, 100000);
    initializeSpinBox(widthSpinBox, "width", 0, 100000);
    initializeSpinBox(heightSpinBox, "height", 0, 100000);
    initializeSpinBox(rowsSpinBox, "rows", 1, 5000);
    initializeSpinBox(colsSpinBox, "cols", 1, 5000);

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

    // ---- ���� ----
    createLayout();

    // ---- �����ź���� ----
    connectSignals();
}

QMap<QString, QVariant> CheckerLayerConfigDialog::getLayerParameters() const {
    QMap<QString, QVariant> params;
    params["startX"] = startXSpinBox->value();
    params["startY"] = startYSpinBox->value();
    params["width"] = widthSpinBox->value();
    params["height"] = heightSpinBox->value();
    params["rows"] = rowsSpinBox->value();
    params["cols"] = colsSpinBox->value();
    params["color1"] = currentColor1;
    params["color2"] = currentColor2;
    return params;
}

// �����ؼ�
void CheckerLayerConfigDialog::initializeSpinBox(QSpinBox*& spinBox, const QString& paramKey, int minValue, int maxValue) {
    spinBox = new QSpinBox(this);
    spinBox->setRange(minValue, maxValue);
    spinBox->setValue(selectedLayer->getParameters().value(paramKey).toInt());
    spinBox->setFixedWidth(120);
}

void CheckerLayerConfigDialog::initializeColorButton(QPushButton*& button, const QString& paramKey, QColor& color, QLabel* alphaLabel) {
    button = new QPushButton(this);
    color = selectedLayer->getParameters().value(paramKey).value<QColor>();
    button->setStyleSheet("background-color: " + color.name());
    button->setFixedWidth(80);
    button->setAutoDefault(false);
}

void CheckerLayerConfigDialog::initializeAlphaLabel(QLabel*& alphaLabel, QColor color) {
    alphaLabel = new QLabel(this);
    QString alphaText = tr("Alpha");
    alphaLabel->setText(QString("%1 : %2").arg(alphaText).arg(color.alpha()));
}

void CheckerLayerConfigDialog::initializeButton(QPushButton*& button, const QString& text, int fixedWidth, bool autoDefault) {
    button = new QPushButton(text, this);
    button->setFixedWidth(fixedWidth);
    button->setAutoDefault(autoDefault);
}

void CheckerLayerConfigDialog::initializeImageLabel(QLabel*& lable, int thumbWidth, int thumbHeight) {
    lable = new QLabel;
    lable->setFixedSize(thumbWidth, thumbHeight);
    lable->setAlignment(Qt::AlignCenter);
    QPixmap thumbnail = generateThumbnail(selectedLayer, thumbWidth, thumbHeight);
    lable->setPixmap(thumbnail.scaled(lable->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

// ����
QVBoxLayout* CheckerLayerConfigDialog::createLayout() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QHBoxLayout* topButtonLayout = new QHBoxLayout();
    topButtonLayout->addWidget(undoButton);
    //topButtonLayout->addWidget(redoButton);
    topButtonLayout->addStretch();

    // ---- ������ ----
    QHBoxLayout* contentLayout = new QHBoxLayout;
    contentLayout->setContentsMargins(0, 0, 80, 80);

    // ��ഹֱ����
    QVBoxLayout* leftSideLayout = new QVBoxLayout;
    leftSideLayout->addStretch(); 

    QFormLayout* sizeLayout = new QFormLayout;
    sizeLayout->setContentsMargins(60, 0, 0, 0);
    sizeLayout->setSpacing(25);
    sizeLayout->addRow(new QLabel(tr("StartX\t:")), startXSpinBox);
    sizeLayout->addRow(new QLabel(tr("StartY\t:")), startYSpinBox);
    sizeLayout->addRow(new QLabel(tr("Width\t:")), widthSpinBox);
    sizeLayout->addRow(new QLabel(tr("Height\t:")), heightSpinBox);
    sizeLayout->addRow(new QLabel(tr("Rows\t:")), rowsSpinBox);
    sizeLayout->addRow(new QLabel(tr("Cols\t:")), colsSpinBox);

    QHBoxLayout* color1Layout = new QHBoxLayout;
    color1Layout->addWidget(color1Button);
    color1Layout->addWidget(color1AlphaLabel);
    sizeLayout->addRow(new QLabel(tr("Color 1\t:")), color1Layout);

    QHBoxLayout* color2Layout = new QHBoxLayout;
    color2Layout->addWidget(color2Button);
    color2Layout->addWidget(color2AlphaLabel);
    sizeLayout->addRow(new QLabel(tr("Color 2\t:")), color2Layout);

    leftSideLayout->addLayout(sizeLayout);
    leftSideLayout->addStretch();

    // �Ҳഹֱ����
    QVBoxLayout* rightSideLayout = new QVBoxLayout;
    rightSideLayout->addStretch();
    rightSideLayout->addWidget(imageLabel, 0, Qt::AlignHCenter);
    rightSideLayout->addStretch();

    contentLayout->addLayout(leftSideLayout);
    contentLayout->addLayout(rightSideLayout);

    // ---- ��ť ----
    QHBoxLayout* horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(applyButton);
    horizontalLayout->addWidget(previewButton);
    horizontalLayout->addWidget(cancelButton);

    // ---- ��������� ----
    mainLayout->addLayout(topButtonLayout);
    mainLayout->addSpacing(40);
    mainLayout->addLayout(contentLayout);
    mainLayout->addLayout(horizontalLayout);
    mainLayout->addSpacing(20);

    return mainLayout;
}

// �ź����
void CheckerLayerConfigDialog::connectSignals() {
    for (QSpinBox* spinBox : { startXSpinBox, startYSpinBox, widthSpinBox, heightSpinBox, rowsSpinBox, colsSpinBox }) {
        connect(spinBox, &QSpinBox::editingFinished, this, &CheckerLayerConfigDialog::chooseSpinBox);
        connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &CheckerLayerConfigDialog::updateThumbnail);
    }

    for (QPushButton* button : { color1Button, color2Button }) {
        connect(button, &QPushButton::clicked, this, [this, button]() {
            (button == color1Button ? chooseColor1() : chooseColor2());
            updateAlphaLabel();
            });
    }

    for (QPushButton* button : { applyButton, previewButton, cancelButton, undoButton}) {
        connect(button, &QPushButton::clicked, this, [this, button]() {
            if (button == applyButton) applyChanges();
            else if (button == previewButton) previewChanges();
            else if (button == cancelButton) cancelChanges();
            else if (button == undoButton) undoChanges();
            });
    }
}

void CheckerLayerConfigDialog::chooseSpinBox() {
    int newStartX = startXSpinBox->value();
    int newStartY = startYSpinBox->value();
    int newWidth = widthSpinBox->value();
    int newHeight = heightSpinBox->value();
    int newRows = rowsSpinBox->value();
    int newCols = colsSpinBox->value();
    pushToUndoStack(selectedLayer->getParameters());
}

void CheckerLayerConfigDialog::chooseColor1() {
    chooseColor(currentColor1, color1Button, this);
    pushToUndoStack(selectedLayer->getParameters());
    updateThumbnail();
}

void CheckerLayerConfigDialog::chooseColor2() {
    chooseColor(currentColor2, color2Button, this);
    pushToUndoStack(selectedLayer->getParameters());
    updateThumbnail();
}

void CheckerLayerConfigDialog::updateAlphaLabel() {
    QString alphaText = tr("Alpha"); 
    color1AlphaLabel->setText(QString("%1 : %2").arg(alphaText).arg(currentColor1.alpha()));
    color2AlphaLabel->setText(QString("%1 : %2").arg(alphaText).arg(currentColor2.alpha()));
}

// Ӧ�ø���
void CheckerLayerConfigDialog::applyChanges() {
    QMap<QString, QVariant> newParams = getLayerParameters();
    selectedLayer->setParameters(newParams);
    accept();
}

void CheckerLayerConfigDialog::previewChanges() {
    QMap<QString, QVariant> newParams = getLayerParameters();
    selectedLayer->setParameters(newParams);

    QList<std::shared_ptr<Layer>> layersList;
    layersList.append(std::make_shared<CheckerLayer>(*selectedLayer));

    LayerDrawDialog* drawDialog = new LayerDrawDialog();
    drawDialog->setLayers(layersList);
    drawDialog->exec();
    delete drawDialog;
}

void CheckerLayerConfigDialog::cancelChanges() {
    selectedLayer->setParameters(originalParams);
    this->close();
}

void CheckerLayerConfigDialog::undoChanges() {
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
    rowsSpinBox->setValue(selectedLayer->getParameters()["rows"].toInt());
    colsSpinBox->setValue(selectedLayer->getParameters()["cols"].toInt());

    currentColor1 = selectedLayer->getParameters()["color1"].value<QColor>();
    currentColor2 = selectedLayer->getParameters()["color2"].value<QColor>();
    color1Button->setStyleSheet("background-color: " + currentColor1.name());
    color2Button->setStyleSheet("background-color: " + currentColor2.name());

    updateAlphaLabel();
    updateThumbnail();
}

void CheckerLayerConfigDialog::pushToUndoStack(const QMap<QString, QVariant>& params) {
    if (undoStack.size() >= maxUndoCount) {
        undoStack.remove(0);
    }
    undoStack.push(params);
}

void CheckerLayerConfigDialog::updateThumbnail() {
    QMap<QString, QVariant> newParams = getLayerParameters();
    selectedLayer->setParameters(newParams);
    QPixmap thumbnail = generateThumbnail(selectedLayer, thumbWidth, thumbHeight);
    imageLabel->setPixmap(thumbnail);
}