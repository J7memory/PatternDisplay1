#include "PictureLayerConfigDialog.h"
#include "common.h"
#include "LayerDrawDialog.h"
#include <QFormLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QImageReader>
#include <QMessageBox>
#include <QFontDatabase>
#include <QGuiApplication>
#include <QScreen>

PictureLayerConfigDialog::PictureLayerConfigDialog(PictureLayer* layer, QWidget* parent)
    : QDialog(parent), selectedLayer(layer) {
    originalParams = layer->getParameters();
    setWindowTitle(tr("Picture Layer Configuration"));
    setFixedSize(900, 600);

    //QScreen* screen = QGuiApplication::primaryScreen();
    //QRect geometry = screen->geometry();
    //qreal pixelRatio = screen->devicePixelRatio();

    //int width = geometry.width() * pixelRatio;
    //int height = geometry.height() * pixelRatio;

    // ---- 创建控件 ----
    initializeLineEdit(imagePathLineEdit, originalParams["imagePath"].toString(), true);
    initializeButton(browseButton, tr("Browse..."), 80, false);

    initializeSpinBox(startXSpinBox, "startX", 0, 100000);
    initializeSpinBox(startYSpinBox, "startY", 0, 100000);
    initializeSpinBox(widthSpinBox, "width", 0, 100000);
    initializeSpinBox(heightSpinBox, "height", 0, 100000);

    static const QStringList styles = {
        tr("Original"),
        tr("Maintain Ratio"),
        tr("Ignore Ratio"),
        tr("Expand Ratio")
    };
    initializeComboBox(modeComboBox, styles, "mode");

    initializeImageLabel(imageLabel, 400, 400);

    //initializeButton(saveButton, tr("Save"), 80, false);
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

QMap<QString, QVariant> PictureLayerConfigDialog::getLayerParameters() const {
    QMap<QString, QVariant> params;
    params["startX"] = startXSpinBox->value();
    params["startY"] = startYSpinBox->value();
    params["width"] = widthSpinBox->value();
    params["height"] = heightSpinBox->value();
    params["mode"] = modeComboBox->currentText();
    params["imagePath"] = imagePathLineEdit->text();
    return params;
}

// 创建控件
void PictureLayerConfigDialog::initializeLineEdit(QLineEdit*& lineEdit, const QString& initialText, bool readOnly) {
    lineEdit = new QLineEdit(initialText);
    lineEdit->setReadOnly(readOnly);
    lineEdit->setFixedWidth(400);
}

void PictureLayerConfigDialog::initializeSpinBox(QSpinBox*& spinBox, const QString& paramKey, int minValue, int maxValue) {
    spinBox = new QSpinBox(this);
    spinBox->setRange(minValue, maxValue);
    spinBox->setValue(selectedLayer->getParameters().value(paramKey).toInt());
    spinBox->setFixedWidth(120);
}

void PictureLayerConfigDialog::initializeComboBox(QComboBox*& comboBox, const QStringList& items, const QString& paramKey) {
    comboBox = new QComboBox(this);
    comboBox->addItems(items);
    comboBox->setCurrentText(selectedLayer->getParameters().value(paramKey).toString());
    comboBox->setFixedWidth(140);
}

void PictureLayerConfigDialog::initializeImageLabel(QLabel*& lable, int thumbWidth, int thumbHeight) {
    lable = new QLabel;
    lable->setFixedSize(thumbWidth, thumbHeight);
    lable->setAlignment(Qt::AlignCenter);
    QPixmap thumbnail = generateThumbnail(selectedLayer, thumbWidth, thumbHeight);
    lable->setPixmap(thumbnail.scaled(lable->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void PictureLayerConfigDialog::initializeButton(QPushButton*& button, const QString& text, int fixedWidth, bool autoDefault) {
    button = new QPushButton(text, this);
    button->setFixedWidth(fixedWidth);
    button->setAutoDefault(autoDefault);
}

// ----布局----
QVBoxLayout* PictureLayerConfigDialog::createLayout() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QHBoxLayout* topButtonLayout = new QHBoxLayout();
    topButtonLayout->addWidget(undoButton);
    //topButtonLayout->addWidget(redoButton);
    topButtonLayout->addStretch();

    // ==== 顶部 Image Path 区域 ====
    QHBoxLayout* topPathLayout = new QHBoxLayout;
    QLabel* pathLabel = new QLabel(tr("Image Path :"));
    pathLabel->setFixedWidth(90);

    topPathLayout->addStretch();
    topPathLayout->addSpacing(26);
    topPathLayout->addWidget(pathLabel);
    topPathLayout->addWidget(imagePathLineEdit);
    topPathLayout->addWidget(browseButton);
    topPathLayout->addStretch();

    mainLayout->addLayout(topButtonLayout);
    mainLayout->addSpacing(20);
    mainLayout->addLayout(topPathLayout);
    mainLayout->addSpacing(20);  // Path 和内容之间留白

    // ==== 中间内容区域 ====
    QHBoxLayout* contentLayout = new QHBoxLayout;
    contentLayout->setContentsMargins(0, 0, 80, 80);

    // 左边参数
    QVBoxLayout* paramContainer = new QVBoxLayout;
    paramContainer->addStretch();

    QVBoxLayout* verticalLayout = new QVBoxLayout;
    verticalLayout->setContentsMargins(0, 30, 0, 0);

    QFormLayout* formLayout = new QFormLayout;
    formLayout->setContentsMargins(80, 0, 0, 0);
    formLayout->setSpacing(25);
    formLayout->addRow(new QLabel(tr("Start X\t:")), startXSpinBox);
    formLayout->addRow(new QLabel(tr("Start Y\t:")), startYSpinBox);
    formLayout->addRow(new QLabel(tr("Width\t:")), widthSpinBox);
    formLayout->addRow(new QLabel(tr("Height\t:")), heightSpinBox);
    formLayout->addRow(new QLabel(tr("Mode\t:")), modeComboBox);

    verticalLayout->addLayout(formLayout);
    paramContainer->addLayout(verticalLayout);
    paramContainer->addStretch();

    // 右边图像
    QVBoxLayout* imageContainer = new QVBoxLayout;
    imageContainer->addStretch();
    imageContainer->addWidget(imageLabel, 0, Qt::AlignCenter);
    imageContainer->addStretch();

    contentLayout->addLayout(paramContainer);
    contentLayout->addLayout(imageContainer);

    // ==== 底部按钮 ====
    QHBoxLayout* horizontalLayout = new QHBoxLayout;
    //horizontalLayout->addWidget(saveButton);
    horizontalLayout->addWidget(applyButton);
    horizontalLayout->addWidget(previewButton);
    horizontalLayout->addWidget(cancelButton);

    mainLayout->addLayout(contentLayout);
    mainLayout->addLayout(horizontalLayout);
    mainLayout->addSpacing(20); // 底部留白

    setLayout(mainLayout);

    return mainLayout;
}

void PictureLayerConfigDialog::connectSignals() {
    for (QSpinBox* spinBox : { startXSpinBox, startYSpinBox, widthSpinBox, heightSpinBox }) {
        connect(spinBox, &QSpinBox::editingFinished, this, &PictureLayerConfigDialog::chooseSpinBox);
        connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &PictureLayerConfigDialog::updateThumbnail);
    }

    for (QComboBox* comboBox : { modeComboBox }) {
        connect(comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &PictureLayerConfigDialog::chooseComboBox);
        connect(comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &PictureLayerConfigDialog::updateThumbnail);
    }

    for (QPushButton* button : { browseButton, /*saveButton*/ applyButton, previewButton, cancelButton, undoButton }) {
        connect(button, &QPushButton::clicked, this, [this, button]() {
            if (button == browseButton) chooseImage();
            //else if (button == saveButton) savePicture();
            else if (button == applyButton) applyChanges();
            else if (button == previewButton) previewChanges();
            else if (button == cancelButton) cancelChanges();
            else if (button == undoButton) undoChanges();
            });
    }
}

void PictureLayerConfigDialog::chooseImage() {
    QString filePath = QFileDialog::getOpenFileName(this, tr("Select Image"), "", "Images (*.png *.jpg *.bmp)");
    if (!filePath.isEmpty()) {
        imagePathLineEdit->setText(filePath);
        pushToUndoStack(selectedLayer->getParameters());
        updateThumbnail();
    }
}

void PictureLayerConfigDialog::chooseSpinBox() {
    int newStartX = startXSpinBox->value();
    int newStartY = startYSpinBox->value();
    int newWidth = widthSpinBox->value();
    int newHeight = heightSpinBox->value();
    pushToUndoStack(selectedLayer->getParameters());
}

void PictureLayerConfigDialog::chooseComboBox() {
    QString newdrawMode = modeComboBox->currentText();
    pushToUndoStack(selectedLayer->getParameters());
}

//void PictureLayerConfigDialog::savePicture() {
//    // 获取 PictureLayer 中的 QPixmap
//    QPixmap pix = selectedLayer->getPixmap();
//    if (pix.isNull()) {
//        QMessageBox::warning(this, "No image", "There's no image to save.");
//        return;
//    }
//
//    QString filePath = QFileDialog::getSaveFileName(this, "Save Image", "", "PNG (*.png);;JPG (*.jpg);;BMP (*.bmp)");
//
//    if (!filePath.isEmpty()) {
//        if (pix.save(filePath)) {
//            QMessageBox::information(this, "Saved", "Image saved successfully.");
//        }
//        else {
//            QMessageBox::critical(this, "Error", "Failed to save the image.");
//        }
//    }
//}

void PictureLayerConfigDialog::applyChanges() {
    QMap<QString, QVariant> newParams = getLayerParameters();
    selectedLayer->setParameters(newParams);
    accept();
}

void PictureLayerConfigDialog::previewChanges() {
    QMap<QString, QVariant> newParams = getLayerParameters();
    selectedLayer->setParameters(newParams);

    QList<std::shared_ptr<Layer>> layersList;
    layersList.append(std::make_shared<PictureLayer>(*selectedLayer));

    LayerDrawDialog* drawDialog = new LayerDrawDialog();
    drawDialog->setLayers(layersList);
    drawDialog->exec();
    delete drawDialog;
}

void PictureLayerConfigDialog::cancelChanges() {
    selectedLayer->setParameters(originalParams);
    this->close();
}

void PictureLayerConfigDialog::undoChanges() {
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
    modeComboBox->setCurrentText(selectedLayer->getParameters()["mode"].toString());

    updateThumbnail();
}

void PictureLayerConfigDialog::pushToUndoStack(const QMap<QString, QVariant>& params) {
    if (undoStack.size() >= maxUndoCount) {
        undoStack.remove(0);
    }
    undoStack.push(params);
}

void PictureLayerConfigDialog::updateThumbnail() {
    QMap<QString, QVariant> newParams = getLayerParameters();
    selectedLayer->setParameters(newParams);
    QPixmap thumbnail = generateThumbnail(selectedLayer, thumbWidth, thumbHeight);
    imageLabel->setPixmap(thumbnail);
}
