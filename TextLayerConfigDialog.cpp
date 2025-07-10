#include "TextLayerConfigDialog.h"
#include "common.h"
#include "LayerDrawDialog.h"
#include <QFontDatabase>
#include <QGuiApplication>
#include <QScreen>

TextLayerConfigDialog::TextLayerConfigDialog(TextLayer* layer, QWidget* parent)
    : QDialog(parent), selectedLayer(layer) {
    originalParams = layer->getParameters();
    setWindowTitle(tr("Text Layer Configuration"));
    setFixedSize(900, 600); 

    //QScreen* screen = QGuiApplication::primaryScreen();
    //QRect geometry = screen->geometry();
    //qreal pixelRatio = screen->devicePixelRatio();

    //int width = geometry.width() * pixelRatio;
    //int height = geometry.height() * pixelRatio;

    // ---- 创建控件 ----
    initializeSpinBox(startXSpinBox, "startX", 0, 100000);
    initializeSpinBox(startYSpinBox, "startY", 0, 100000);
    initializeSpinBox(sizeSpinBox, "size", 1, 200);

    initializeTextEdit(textTextEdit, "text");

    QStringList availableFonts = QFontDatabase::families();
    initializeComboBox(fontComboBox, { availableFonts }, "font");

    static const QStringList styles = {
        tr("Normal"),
        tr("Bold"),
        tr("Italic"),
        tr("Underline"),
        tr("B & I"),
        tr("B & U"),
        tr("I & U"),
        tr("B & I & U")
    };

    initializeComboBox(styleComboBox, { styles }, "style");

    initializeColorButton(textColorButton, "textColor", currentTextColor, textAlphaLabel);
    initializeAlphaLabel(textAlphaLabel, currentTextColor);

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

QMap<QString, QVariant> TextLayerConfigDialog::getLayerParameters() const {
    QMap<QString, QVariant> params;
    params["startX"] = startXSpinBox->value();
    params["startY"] = startYSpinBox->value();
    params["text"] = textTextEdit->toPlainText();
    params["font"] = fontComboBox->currentText();
    params["size"] = sizeSpinBox->value();
    params["style"] = styleComboBox->currentText();
    params["textColor"] = currentTextColor;
    return params;
}

// 创建控件
void TextLayerConfigDialog::initializeSpinBox(QSpinBox*& spinBox, const QString& paramKey, int minValue, int maxValue) {
    spinBox = new QSpinBox(this);
    spinBox->setRange(minValue, maxValue);
    spinBox->setValue(selectedLayer->getParameters().value(paramKey).toInt());
    spinBox->setFixedWidth(120);
}

void TextLayerConfigDialog::initializeTextEdit(QTextEdit*& textEdit, const QString& paramKey) {
    textEdit = new QTextEdit(this);
    textEdit->setText(selectedLayer->getParameters().value(paramKey).toString());
    textEdit->setFixedWidth(220);
    textEdit->setFixedHeight(80);
}

void TextLayerConfigDialog::initializeColorButton(QPushButton*& button, const QString& paramKey, QColor& color, QLabel* alphaLabel) {
    button = new QPushButton(this);
    color = selectedLayer->getParameters().value(paramKey).value<QColor>();
    button->setStyleSheet("background-color: " + color.name());
    button->setFixedWidth(80);
    button->setAutoDefault(false);
}

void TextLayerConfigDialog::initializeAlphaLabel(QLabel*& alphaLabel, QColor color) {
    alphaLabel = new QLabel(this);
    QString alphaText = tr("Alpha");
    alphaLabel->setText(QString("%1 : %2").arg(alphaText).arg(color.alpha()));
}

void TextLayerConfigDialog::initializeComboBox(QComboBox*& comboBox, const QStringList& items, const QString& paramKey) {
    comboBox = new QComboBox(this);
    comboBox->addItems(items);
    comboBox->setCurrentText(selectedLayer->getParameters().value(paramKey).toString());
    comboBox->setFixedWidth(120);
}

void TextLayerConfigDialog::initializeButton(QPushButton*& button, const QString& text, int fixedWidth, bool autoDefault) {
    button = new QPushButton(text, this);
    button->setFixedWidth(fixedWidth);
    button->setAutoDefault(autoDefault);
}

void TextLayerConfigDialog::initializeImageLabel(QLabel*& lable, int thumbWidth, int thumbHeight) {
    lable = new QLabel;
    lable->setFixedSize(thumbWidth, thumbHeight);
    lable->setAlignment(Qt::AlignCenter);
    QPixmap thumbnail = generateThumbnail(selectedLayer, thumbWidth, thumbHeight);
    lable->setPixmap(thumbnail.scaled(lable->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

// 布局
QVBoxLayout* TextLayerConfigDialog::createLayout() {
    // ---- 主布局 ----
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QHBoxLayout* topButtonLayout = new QHBoxLayout();
    topButtonLayout->addWidget(undoButton);
    //topButtonLayout->addWidget(redoButton);
    topButtonLayout->addStretch();

    QHBoxLayout* contentLayout = new QHBoxLayout;
    contentLayout->setContentsMargins(0, 0, 30, 30);

    // ---- 左侧参数区域 ----
    QVBoxLayout* paramContainer = new QVBoxLayout;
    paramContainer->addStretch();  // 上方留白

    QVBoxLayout* verticalLayout = new QVBoxLayout;
    verticalLayout->setContentsMargins(0, 30, 0, 0);

    QFormLayout* sizeLayout = new QFormLayout;
    sizeLayout->setContentsMargins(60, 0, 0, 0);
    sizeLayout->setSpacing(25);
    sizeLayout->addRow(new QLabel(tr("StartX\t:")), startXSpinBox);
    sizeLayout->addRow(new QLabel(tr("StartY\t:")), startYSpinBox);
    sizeLayout->addRow(new QLabel(tr("Font\t:")), fontComboBox);
    sizeLayout->addRow(new QLabel(tr("Size\t:")), sizeSpinBox);
    sizeLayout->addRow(new QLabel(tr("Style\t:")), styleComboBox);

    QHBoxLayout* colorLayout = new QHBoxLayout;
    colorLayout->addWidget(textColorButton);
    colorLayout->addWidget(textAlphaLabel);
    sizeLayout->addRow(new QLabel(tr("TextColor :")), colorLayout);

    sizeLayout->addRow(new QLabel(tr("Text\t:")), textTextEdit);

    verticalLayout->addLayout(sizeLayout);
    paramContainer->addLayout(verticalLayout);
    paramContainer->addStretch();

    // ---- 右侧区域：imageLabel + 坐标输入 ----
    QVBoxLayout* imageContainer = new QVBoxLayout;
    imageContainer->addStretch();
    imageContainer->addWidget(imageLabel, 0, Qt::AlignCenter);
    imageContainer->addStretch();

    contentLayout->addLayout(paramContainer);
    contentLayout->addLayout(imageContainer);

    // ---- 底部按钮区域 ----
    QHBoxLayout* horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(applyButton);
    horizontalLayout->addWidget(previewButton);
    horizontalLayout->addWidget(cancelButton);

    mainLayout->addLayout(topButtonLayout);
    mainLayout->addSpacing(60);
    mainLayout->addLayout(contentLayout);
    mainLayout->addLayout(horizontalLayout);
    mainLayout->addSpacing(20);

    setLayout(mainLayout);
    return mainLayout;
}

void TextLayerConfigDialog::connectSignals() {
    for (QSpinBox* spinBox : { startXSpinBox , startYSpinBox , sizeSpinBox }) {
        connect(spinBox, &QSpinBox::editingFinished, this, &TextLayerConfigDialog::chooseSpinBox);
        connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &TextLayerConfigDialog::updateThumbnail);
    }

    for (QPushButton* button : { textColorButton }) {
        connect(button, &QPushButton::clicked, this, [this, button]() {
            chooseTextColor();
            updateAlphaLabel();
            });
    }

    for (QComboBox* comboBox : { fontComboBox,  styleComboBox }) {
        connect(comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &TextLayerConfigDialog::chooseComboBox);
        connect(comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &TextLayerConfigDialog::updateThumbnail);
    }

    for (QTextEdit* textEdit : { textTextEdit }) {
        connect(textTextEdit, &QTextEdit::textChanged, this, &TextLayerConfigDialog::chooseText);
        connect(textTextEdit, &QTextEdit::textChanged, this, &TextLayerConfigDialog::updateThumbnail);
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

void TextLayerConfigDialog::chooseSpinBox() {
    int newStartX = startXSpinBox->value();
    int newStartY = startYSpinBox->value();
    int newSize = sizeSpinBox->value();
    pushToUndoStack(selectedLayer->getParameters());
}

void TextLayerConfigDialog::chooseComboBox() {
    QString newFont= fontComboBox->currentText();
    QString newStyle = styleComboBox->currentText();
    pushToUndoStack(selectedLayer->getParameters());
}

void TextLayerConfigDialog::chooseText() {
    QString newText = textTextEdit->toPlainText();
    pushToUndoStack(selectedLayer->getParameters());
}

void TextLayerConfigDialog::chooseTextColor() {
    chooseColor(currentTextColor, textColorButton, this);
    pushToUndoStack(selectedLayer->getParameters());
    updateThumbnail();
}

void TextLayerConfigDialog::updateAlphaLabel() {
    QString alphaText = tr("Alpha");
    textAlphaLabel->setText(QString("%1 : %2").arg(alphaText).arg(currentTextColor.alpha()));
}

void TextLayerConfigDialog::applyChanges() {
    QMap<QString, QVariant> newParams = getLayerParameters();
    selectedLayer->setParameters(newParams);
    accept();
}

void TextLayerConfigDialog::previewChanges() {
    QMap<QString, QVariant> newParams = getLayerParameters();
    selectedLayer->setParameters(newParams);

    QList<std::shared_ptr<Layer>> layersList;
    layersList.append(std::make_shared<TextLayer>(*selectedLayer));

    LayerDrawDialog* drawDialog = new LayerDrawDialog();
    drawDialog->setLayers(layersList);
    drawDialog->exec();
    delete drawDialog;
}

void TextLayerConfigDialog::cancelChanges() {
    selectedLayer->setParameters(originalParams);
    this->close();
}

void TextLayerConfigDialog::undoChanges() {
    if (!undoStack.isEmpty()) {
        QMap<QString, QVariant> lastParams = undoStack.pop();
        selectedLayer->setParameters(lastParams);
    }
    else {
        selectedLayer->setParameters(originalParams);
    }

    startXSpinBox->setValue(selectedLayer->getParameters()["startX"].toInt());
    startYSpinBox->setValue(selectedLayer->getParameters()["startY"].toInt());
    fontComboBox->setCurrentText(selectedLayer->getParameters()["font"].toString());
    sizeSpinBox->setValue(selectedLayer->getParameters()["size"].toInt());
    styleComboBox->setCurrentText(selectedLayer->getParameters()["style"].toString());

    currentTextColor = selectedLayer->getParameters()["textColor"].value<QColor>();
    textColorButton->setStyleSheet("background-color: " + currentTextColor.name());

    updateAlphaLabel();
    updateThumbnail();
}

void TextLayerConfigDialog::pushToUndoStack(const QMap<QString, QVariant>& params) {
    if (undoStack.size() >= maxUndoCount) {
        undoStack.remove(0);
    }
    undoStack.push(params);
}

void TextLayerConfigDialog::updateThumbnail() {
    QMap<QString, QVariant> newParams = getLayerParameters();
    selectedLayer->setParameters(newParams);
    QPixmap thumbnail = generateThumbnail(selectedLayer, thumbWidth, thumbHeight);
    imageLabel->setPixmap(thumbnail);
}