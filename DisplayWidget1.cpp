//#include "BitMapLayer.h"
//#include "BitMapLayerConfigDialog.h"
//#include "CCircleLayer.h"
//#include "CCircleLayerConfigDialog.h"
//#include "CheckerLayer.h"
//#include "CheckerLayerConfigDialog.h"
//#include "CRectLayer.h"
//#include "CRectLayerConfigDialog.h"
//#include "DisplayWidget.h"
//#include "GradientLayer.h"
//#include "GradientLayerConfigDialog.h"
//#include "Layer.h"
//#include "LayerType1.h"
//#include "LayerType1ConfigDialog.h"
//#include "LayerType2.h"
//#include "LayerType2ConfigDialog.h"
//#include "LineLayer.h"
//#include "LineLayerConfigDialog.h"
//#include "MonoLayer.h"
//#include "MonoLayerConfigDialog.h"
//#include "Pattern.h"
//#include "PatternDrawDialog.h"
//#include "PatternManager.h"
//#include "TextLayer.h"
//#include "TextLayerConfigDialog.h"
//#include <memory>
//#include <QFont>
//#include <QFormLayout>
//#include <QGuiApplication>
//#include <QHBoxLayout>
//#include <QInputDialog>
//#include <QLabel>
//#include <QLineEdit>
//#include <QListWidget>
//#include <QMessageBox>
//#include <QPushButton>
//#include <QScreen>
//#include <QSpinBox>
//#include <QSplitter>
//#include <QTabWidget>
//#include <QTextEdit>
//#include <QVBoxLayout>
//
//#include <QDebug>
//#include <QFile>
//#include <QJsonArray>
//#include <QJsonDocument>
//#include <QJsonObject>
//
//DisplayWidget::DisplayWidget(QWidget* parent)
//    : QWidget(parent)
//{
//    setupUI();
//}
//
//DisplayWidget::~DisplayWidget()
//{
//}
//
//void DisplayWidget::setupUI()
//{
//    // 创建 QSplitter
//    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
//
//    // 左侧（图案列表 + 按钮）
//    QWidget* leftWidget = new QWidget(this);
//    QVBoxLayout* leftLayout = new QVBoxLayout(leftWidget);
//
//    // Pattern列表按钮
//    QHBoxLayout* buttonLayout = new QHBoxLayout();
//    addButton = new QPushButton("Add", this);
//    delButton = new QPushButton("Del", this);
//    buttonLayout->addWidget(addButton);
//    buttonLayout->addWidget(delButton);
//
//    // Pattern列表组件
//    patternList = new QListWidget(this);
//    patternList->setSelectionMode(QAbstractItemView::ExtendedSelection); // 多选模式
//    leftLayout->addLayout(buttonLayout);
//    leftLayout->addWidget(patternList);
//    leftWidget->setLayout(leftLayout);
//
//    // 右侧（Tab 选项卡）
//    tabWidget = new QTabWidget(this);
//    QWidget* tab1 = creatInformationTab();
//    QWidget* tab2 = creatLayerTab();
//    QWidget* tab3 = creatThumbnailTab();
//    tabWidget->addTab(tab1, "Information");
//    tabWidget->addTab(tab2, "Layer");
//    tabWidget->addTab(tab3, "Thumbnail");
//
//    // 添加到 QSplitter
//    splitter->addWidget(leftWidget);
//    splitter->addWidget(tabWidget);
//    splitter->setSizes({ 250, 700 });
//    leftWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
//    tabWidget->setMinimumWidth(1);
//    splitter->setStretchFactor(0, 1);
//    splitter->setStretchFactor(1, 1);
//
//    // 设置主布局
//    QVBoxLayout* mainLayout = new QVBoxLayout(this);
//    mainLayout->addWidget(splitter);
//    setLayout(mainLayout);
//
//    // 连接信号槽
//    connect(addButton, &QPushButton::clicked, this, &DisplayWidget::addPattern);
//    connect(delButton, &QPushButton::clicked, this, &DisplayWidget::removePattern);
//    connect(patternList, &QListWidget::itemSelectionChanged, this, &DisplayWidget::onPatternSelected);
//}
//
//QWidget* DisplayWidget::creatInformationTab()
//{
//    QWidget* tab1 = new QWidget(this);
//    QVBoxLayout* tab1Layout = new QVBoxLayout(tab1);
//
//    // 创建表单布局
//    QFormLayout* formLayout = new QFormLayout();
//
//    // 初始化控件
//    patternNameEdit = new QLineEdit(tab1);
//    patternWidthSpinBox = new QSpinBox(tab1);
//    patternHeightSpinBox = new QSpinBox(tab1);
//    patternDescriptionEdit = new QTextEdit(tab1);
//
//    // 设置框体宽度和高度
//    patternNameEdit->setFixedWidth(180);
//    patternWidthSpinBox->setFixedWidth(100);
//    patternHeightSpinBox->setFixedWidth(100);
//    patternWidthSpinBox->setRange(1, 10000);
//    patternHeightSpinBox->setRange(1, 10000);
//    patternDescriptionEdit->setFixedSize(300, 80); // 文本框大小
//
//    QScreen* screen = QGuiApplication::primaryScreen();
//    if (screen) {
//        QRect geometry = screen->geometry();
//        qreal pixelRatio = screen->devicePixelRatio();
//
//        int width = geometry.width() * pixelRatio;
//        int height = geometry.height() * pixelRatio;
//
//        patternWidthSpinBox->setValue(width);
//        patternHeightSpinBox->setValue(height);
//    }
//
//    // 添加到表单布局
//    formLayout->addRow("Pattern Name:", patternNameEdit);
//    formLayout->addRow("Pattern Width:", patternWidthSpinBox);
//    formLayout->addRow("Pattern Height:", patternHeightSpinBox);
//    formLayout->addRow("Pattern Description:", patternDescriptionEdit);
//
//    // 整体垂直布局，并水平居中
//    QHBoxLayout* formCenterLayout = new QHBoxLayout();
//    formCenterLayout->addStretch();
//    formCenterLayout->addLayout(formLayout);
//    formCenterLayout->addStretch();
//
//    // 按钮水平布局
//    QHBoxLayout* buttonLayout = new QHBoxLayout();
//    editButton = new QPushButton("Edit", this);
//    saveButton = new QPushButton("Save", this);
//    cancelButton = new QPushButton("Cancel", this);
//    drawButton = new QPushButton("Draw", this);
//    buttonLayout->addStretch();
//    buttonLayout->addWidget(editButton);
//    buttonLayout->addSpacing(20);
//    buttonLayout->addWidget(saveButton);
//    buttonLayout->addSpacing(20);
//    buttonLayout->addWidget(cancelButton);
//    buttonLayout->addSpacing(20);
//    buttonLayout->addWidget(drawButton);
//    buttonLayout->addStretch();
//
//    // 主布局
//    tab1Layout->addLayout(formCenterLayout);
//    tab1Layout->addLayout(buttonLayout);
//    tab1->setLayout(tab1Layout);
//
//    // 连接信号槽
// /*   connect(editButton, &QPushButton::clicked, this, &DisplayWidget::editPatternInformation);*/
//    connect(saveButton, &QPushButton::clicked, this, &DisplayWidget::savePatternInformation);
//    connect(cancelButton, &QPushButton::clicked, this, &DisplayWidget::cancelPatternInformation);
//    connect(drawButton, &QPushButton::clicked, this, &DisplayWidget::onPatternSelectedDraw);
//
//    return tab1;
//}
//
//QWidget* DisplayWidget::creatLayerTab()
//{
//    QWidget* tab2 = new QWidget(this);
//    QHBoxLayout* tab2Layout = new QHBoxLayout(tab2);
//
//    // 创建 QSplitter
//    QSplitter* splitter = new QSplitter(Qt::Horizontal, tab2);
//
//    // 左侧（图层列表 + 按钮)
//    QWidget* leftWidget = new QWidget(tab2);
//    QVBoxLayout* leftLayout = new QVBoxLayout(leftWidget);
//
//    // 按钮水平布局
//    QHBoxLayout* buttonLayout = new QHBoxLayout();
//    patternComboBox = new QComboBox();
//    addLayerButton = new QPushButton("Add", this);
//    delLayerButton = new QPushButton("Del", this);
//    patternComboBox->setMinimumWidth(80);
//    addLayerButton->setFixedWidth(40);
//    delLayerButton->setFixedWidth(40);
//    buttonLayout->setSpacing(5);
//    buttonLayout->addWidget(patternComboBox);
//    buttonLayout->addWidget(addLayerButton);
//    buttonLayout->addWidget(delLayerButton);
//
//    // 添加Layer列表组件
//    layerList = new QListWidget(tab2);
//    layerList->setSelectionMode(QAbstractItemView::ExtendedSelection);
//    leftLayout->addLayout(buttonLayout);
//    leftLayout->addWidget(layerList);
//    leftWidget->setLayout(leftLayout);
//
//    // 右侧Layer表单布局
//    QWidget* rightWidget = new QWidget(tab2);
//    QVBoxLayout* rightLayout = new QVBoxLayout(rightWidget);
//
//    QFormLayout* formLayout = new QFormLayout();
//
//    // 初始化控件
//    layerNameEdit = new QLineEdit(tab2);
//    layerTypeComboBox = new QComboBox(tab2);
//    layerStatusCheckBox = new QCheckBox("Active", tab2);
//    layerDescriptionEdit = new QTextEdit(tab2);
//
//    // 设置框体宽度和高度
//    layerNameEdit->setFixedWidth(180);
//    layerTypeComboBox->setFixedWidth(100);
//    layerStatusCheckBox->setFixedWidth(100);
//    layerDescriptionEdit->setFixedSize(300, 80);
//
//    // 设置控件的默认值
//    layerTypeComboBox->addItem("Mono");
//    layerTypeComboBox->addItem("Gradient");
//    layerTypeComboBox->addItem("Checker");
//    layerTypeComboBox->addItem("BitMap");
//    layerTypeComboBox->addItem("CRect");
//    layerTypeComboBox->addItem("CCircle");
//    layerTypeComboBox->addItem("Line");
//    layerTypeComboBox->addItem("Text");
//    layerTypeComboBox->setCurrentIndex(0);
//    layerStatusCheckBox->setChecked(true);
//
//    // 添加到表单布局
//    formLayout->addRow("Layer Name:", layerNameEdit);
//    formLayout->addRow("Layer Type:", layerTypeComboBox);
//    formLayout->addRow("Layer Status:", layerStatusCheckBox);
//    formLayout->addRow("Layer Description:", layerDescriptionEdit);
//
//    // 整体垂直布局，并水平居中
//    QHBoxLayout* formCenterLayout = new QHBoxLayout();
//    formCenterLayout->addStretch();
//    formCenterLayout->addLayout(formLayout);
//    formCenterLayout->addStretch();
//
//    // 按钮水平布局
//    QHBoxLayout* layerbuttonLayout = new QHBoxLayout();
//    editLayerButton = new QPushButton("Edit", this);
//    saveLayerButton = new QPushButton("Save", this);
//    layerbuttonLayout->addStretch();
//    layerbuttonLayout->addWidget(editLayerButton);
//    layerbuttonLayout->addSpacing(20);
//    layerbuttonLayout->addWidget(saveLayerButton);
//    layerbuttonLayout->addStretch();
//
//    rightLayout->addLayout(formCenterLayout);
//    rightLayout->addLayout(layerbuttonLayout);
//    rightWidget->setLayout(rightLayout);
//
//    // 将左侧和右侧部分添加到 QSplitter 中
//    splitter->addWidget(leftWidget);
//    splitter->addWidget(rightWidget);
//    splitter->setSizes({ 200, 600 });
//    leftWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
//    rightWidget->setMinimumWidth(1);
//    splitter->setStretchFactor(0, 1);
//    splitter->setStretchFactor(1, 1);
//
//    // 主布局
//    tab2Layout->addWidget(splitter);  // 使用splitter作为主要布局
//    tab2->setLayout(tab2Layout);
//
//    // 连接信号槽
//    connect(patternComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &DisplayWidget::onPatternComboBoxChanged);
//    connect(addLayerButton, &QPushButton::clicked, this, &DisplayWidget::addLayer);
//    connect(delLayerButton, &QPushButton::clicked, this, &DisplayWidget::removeLayer);
//    connect(layerList, &QListWidget::itemSelectionChanged, this, &DisplayWidget::onLayerSelected);
//    connect(saveLayerButton, &QPushButton::clicked, this, &DisplayWidget::saveLayerInfo);
//    connect(editLayerButton, &QPushButton::clicked, this, &DisplayWidget::editLayerInfo);
//
//    return tab2;
//}
//
//QWidget* DisplayWidget::creatThumbnailTab()
//{
//    QWidget* tab3 = new QWidget(this);
//    QVBoxLayout* tab3Layout = new QVBoxLayout(tab3);
//
//    // 创建一个简单的标签
//    QLabel* label = new QLabel("This is Mode 3", tab3);
//    label->setAlignment(Qt::AlignCenter);  // 设置标签内容居中
//    tab3Layout->addWidget(label);
//
//    tab3->setLayout(tab3Layout);
//
//    return tab3;
//}
//
//// 新增Pattern
//void DisplayWidget::addPattern() {
//    // 创建新 Pattern
//    std::shared_ptr<Pattern> newPattern = PatternManager::addPattern();
//    QString newPatternName = newPattern->getPatternName();
//
//    // 添加新Pattern到 patternList
//    auto* item = new QListWidgetItem(QStringLiteral("%1").arg(newPattern->getPatternName()), patternList);
//    item->setData(Qt::UserRole, QVariant::fromValue(newPattern));
//    patternList->addItem(item);
//
//    // 选中新建项（多选模式需要先清理选中项）
//    patternList->clearSelection();
//    patternList->setCurrentItem(item);
//
//    // 更新 ComboBox 中的可选项
//    updatePatternComboBox();
//
//    // 在 ComboBox 中选中新 Pattern 项（匹配patternName）
//    int comboBoxIndex = patternComboBox->findText(newPatternName);
//    if (comboBoxIndex != -1) {
//        patternComboBox->setCurrentIndex(comboBoxIndex);
//    }
//}
//
//// 删除Pattern
//void DisplayWidget::removePattern() {
//    QList<QListWidgetItem*> selectedItems = patternList->selectedItems();
//    if (selectedItems.isEmpty()) {
//        QMessageBox::warning(this, "Delete failed", "Please select the patterns to delete!");
//        return;
//    }
//
//    // 确认删除
//    if (QMessageBox::question(this, "Delete Confirmation",
//        QString("Are you sure you want to delete the selected %1 patterns?").arg(selectedItems.size()),
//        QMessageBox::Yes | QMessageBox::No) == QMessageBox::No) {
//        return;
//    }
//
//    for (auto* item : selectedItems) {
//        PatternManager::removePatternByName(item->text());
//        delete item;
//    }
//
//    updatePatternComboBox();
//}
//
//// 获取PatternList/patternComboBox中的选择项获取对象并显示对象信息，同步patternComboBox中选项，同步layerList中pattern对象的layer
//void DisplayWidget::onPatternSelected()
//{
//    // 获取当前选中的项
//    QListWidgetItem* selectedItem = patternList->currentItem();
//    if (!selectedItem) return;
//
//    // 通过名称从 PatternManager 获取 Pattern
//    QString patternName = selectedItem->text();
//    auto patternPtr = PatternManager::getPatternByName(patternName);
//
//    if (!patternPtr) return;
//
//    // 更新 UI 信息
//    patternNameEdit->setText(patternPtr->getPatternName());
//    patternWidthSpinBox->setValue(patternPtr->getPatternWidth());
//    patternHeightSpinBox->setValue(patternPtr->getPatternHeight());
//    patternDescriptionEdit->setText(patternPtr->getPatternDescription());
//
//    QSignalBlocker blocker(patternComboBox); // 进入作用域时屏蔽信号，退出时自动恢复
//    updatePatternComboBox();
//    // 在 ComboBox 中找到匹配的 PatternName 并选中
//    int comboBoxIndex = patternComboBox->findText(patternName);
//    if (comboBoxIndex != -1) {
//        patternComboBox->setCurrentIndex(comboBoxIndex);
//    }
//
//    // 更新 Layer 列表
//    updateLayerListView(patternPtr);
//}
//
//// 更新PatternList中的对象
//void DisplayWidget::updatePatternListView()
//{
//    patternList->clear();
//
//    // 遍历 PatternManager 中的所有 Pattern 添加到 patternList
//    const QVector<std::shared_ptr<Pattern>>& patterns = PatternManager::getAllPatterns();
//    for (const auto& pattern : patterns) {
//        QListWidgetItem* item = new QListWidgetItem(pattern->getPatternName());
//        patternList->addItem(item);
//    }
//
//    updatePatternComboBox();
//}
//
//// 编辑当前Pattern
////void DisplayWidget::editPatternInformation()
////{
////    // 创建对话框
////    QDialog dialog(this);
////    dialog.setWindowTitle("Edit Layers");
////    dialog.resize(800, 500);  // 适当调整大小
////
////    // 创建 QSplitter
////    QSplitter* splitter = new QSplitter(Qt::Horizontal, &dialog);
////
////    // 左侧（图层列表 + 按钮）
////    QWidget* leftWidget = new QWidget(&dialog);
////    QVBoxLayout* leftLayout = new QVBoxLayout(leftWidget);
////
////    // 按钮水平布局
////    QHBoxLayout* buttonLayout = new QHBoxLayout();
////    QComboBox* patternComboBox = new QComboBox();
////    QPushButton* addLayerButton = new QPushButton("Add");
////    QPushButton* delLayerButton = new QPushButton("Del");
////    patternComboBox->setMinimumWidth(80);
////    addLayerButton->setFixedWidth(40);
////    delLayerButton->setFixedWidth(40);
////    buttonLayout->setSpacing(5);
////    buttonLayout->addWidget(patternComboBox);
////    buttonLayout->addWidget(addLayerButton);
////    buttonLayout->addWidget(delLayerButton);
////
////    // 添加Layer列表组件
////    QListWidget* layerList = new QListWidget(&dialog);
////    layerList->setSelectionMode(QAbstractItemView::ExtendedSelection);
////    leftLayout->addLayout(buttonLayout);
////    leftLayout->addWidget(layerList);
////    leftWidget->setLayout(leftLayout);
////
////    // 右侧 Layer 表单布局
////    QWidget* rightWidget = new QWidget(&dialog);
////    QVBoxLayout* rightLayout = new QVBoxLayout(rightWidget);
////    QFormLayout* formLayout = new QFormLayout();
////
////    // 初始化控件
////    QLineEdit* layerNameEdit = new QLineEdit(&dialog);
////    QComboBox* layerTypeComboBox = new QComboBox(&dialog);
////    QCheckBox* layerStatusCheckBox = new QCheckBox("Active", &dialog);
////    QTextEdit* layerDescriptionEdit = new QTextEdit(&dialog);
////
////    // 设置控件大小
////    layerNameEdit->setFixedWidth(180);
////    layerTypeComboBox->setFixedWidth(100);
////    layerStatusCheckBox->setFixedWidth(100);
////    layerDescriptionEdit->setFixedSize(300, 80);
////
////    // 设置控件的默认值
////    layerTypeComboBox->addItem("Mono");
////    layerTypeComboBox->addItem("Gradient");
////    layerTypeComboBox->addItem("Checker");
////    layerTypeComboBox->addItem("BitMap");
////    layerTypeComboBox->addItem("CRect");
////    layerTypeComboBox->addItem("CCircle");
////    layerTypeComboBox->addItem("Line");
////    layerTypeComboBox->addItem("Text");
////    layerTypeComboBox->setCurrentIndex(0);
////    layerStatusCheckBox->setChecked(true);
////
////    // 添加到表单布局
////    formLayout->addRow("Layer Name:", layerNameEdit);
////    formLayout->addRow("Layer Type:", layerTypeComboBox);
////    formLayout->addRow("Layer Status:", layerStatusCheckBox);
////    formLayout->addRow("Layer Description:", layerDescriptionEdit);
////
////    // 居中表单
////    QHBoxLayout* formCenterLayout = new QHBoxLayout();
////    formCenterLayout->addStretch();
////    formCenterLayout->addLayout(formLayout);
////    formCenterLayout->addStretch();
////
////    // 按钮水平布局
////    QHBoxLayout* layerButtonLayout = new QHBoxLayout();
////    QPushButton* editLayerButton = new QPushButton("Edit");
////    QPushButton* saveLayerButton = new QPushButton("Save");
////    layerButtonLayout->addStretch();
////    layerButtonLayout->addWidget(editLayerButton);
////    layerButtonLayout->addSpacing(20);
////    layerButtonLayout->addWidget(saveLayerButton);
////    layerButtonLayout->addStretch();
////
////    rightLayout->addLayout(formCenterLayout);
////    rightLayout->addLayout(layerButtonLayout);
////    rightWidget->setLayout(rightLayout);
////
////    // 将左侧和右侧部分添加到 QSplitter 中
////    splitter->addWidget(leftWidget);
////    splitter->addWidget(rightWidget);
////    splitter->setSizes({ 200, 600 });
////    leftWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
////    rightWidget->setMinimumWidth(1);
////    splitter->setStretchFactor(0, 1);
////    splitter->setStretchFactor(1, 1);
////
////    // 主布局
////    QVBoxLayout* mainLayout = new QVBoxLayout(&dialog);
////    mainLayout->addWidget(splitter);
////    dialog.setLayout(mainLayout);
////
////    // 连接信号槽
////    connect(addLayerButton, &QPushButton::clicked, this, &DisplayWidget::addLayer);
////    connect(delLayerButton, &QPushButton::clicked, this, &DisplayWidget::removeLayer);
////    connect(layerList, &QListWidget::itemSelectionChanged, this, &DisplayWidget::onLayerSelected);
////    connect(saveLayerButton, &QPushButton::clicked, this, &DisplayWidget::saveLayerInfo);
////    connect(editLayerButton, &QPushButton::clicked, this, &DisplayWidget::editLayerInfo);
////    connect(patternComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &DisplayWidget::onPatternComboBoxChanged);
////
////    // 显示对话框
////    dialog.exec();
////}
//
//
//// 保存当前Pattern信息
//void DisplayWidget::savePatternInformation()
//{
//    // 确认对话框
//    if (QMessageBox::question(this, "Save Confirmation", "Are you sure you want to save the current pattern information?",
//        QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) {
//        return;
//    }
//
//    // Lambda 处理警告消息
//    auto showWarning = [this](const QString& message) {
//        QMessageBox::warning(this, "Warning", message);
//        };
//
//    // 获取输入框数据
//    QString name = patternNameEdit->text().trimmed();
//    int width = patternWidthSpinBox->value();
//    int height = patternHeightSpinBox->value();
//    QString description = patternDescriptionEdit->toPlainText();
//
//    // 检查选中的 Pattern
//    QListWidgetItem* selectedItem = patternList->currentItem();
//    if (!selectedItem) {
//        showWarning("No pattern selected!");
//        return;
//    }
//
//    // 查找选中的 Pattern
//    auto selectedPattern = PatternManager::getPatternByName(selectedItem->text());
//    if (!selectedPattern) {
//        showWarning("Invalid pattern object!");
//        return;
//    }
//
//    if (name.isEmpty()) {
//        showWarning("Pattern name cannot be empty!");
//        return;
//    }
//
//    // 检查名称是否已存在
//    for (const auto& existingPattern : PatternManager::getAllPatterns()) {
//        if (existingPattern != selectedPattern && existingPattern->getPatternName() == name) {
//            showWarning("A pattern with this name already exists!");
//            return;
//        }
//    }
//
//    // 更新 Pattern 信息
//    selectedPattern->setPatternName(name);
//    selectedPattern->setPatternWidth(width);
//    selectedPattern->setPatternHeight(height);
//    selectedPattern->setPatternDescription(description);
//
//    // 更新 UI 显示
//    selectedItem->setText(name);
//    updatePatternComboBox();
//}
//
//// 撤销当前Pattern修改的信息
//void DisplayWidget::cancelPatternInformation()
//{
//    if (QMessageBox::question(this, "Cancel Editing", "Are you sure you want to discard changes?",
//        QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) {
//        return;
//    }
//
//    onPatternSelected();
//}
//
//// 绘制当前Pattern(包含多个Layer)
//void DisplayWidget::onPatternSelectedDraw()
//{
//    // Lambda 处理警告消息
//    auto showWarning = [this](const QString& message) {
//        QMessageBox::warning(this, "Warning", message);
//        };
//
//    // 获取当前选中的 Pattern 名称
//    QListWidgetItem* selectedItem = patternList->currentItem();
//    if (!selectedItem) {
//        showWarning("No pattern selected!");
//        return;
//    }
//
//    // 通过 PatternManager 查找 Pattern
//    std::shared_ptr<Pattern> selectedPattern = PatternManager::getPatternByName(selectedItem->text());
//    if (!selectedPattern) {
//        showWarning("Invalid pattern selection!");
//        return;
//    }
//
//    // 检查是否有 Layer
//    if (selectedPattern->getLayers().isEmpty()) {
//        showWarning("The selected pattern has no Layers!\n" "Please continue after adding the layer.");
//        return;
//    }
//
//    // 显示 PatternDrawDialog
//    QScopedPointer<PatternDrawDialog> drawDialog(new PatternDrawDialog(selectedPattern));
//    drawDialog->exec();
//}
//
//// 更新PatternComboBox中的下拉选项
//void DisplayWidget::updatePatternComboBox() {
//    // 记录当前选中的项
//    int currentIndex = patternComboBox->currentIndex();
//    patternComboBox->clear();
//
//    // 获取所有 Pattern 对象的名称
//    QStringList patternNames = PatternManager::getAllPatternNames();
//    if (patternNames.isEmpty()) {
//        patternComboBox->setCurrentIndex(-1);
//        return;
//    }
//    patternComboBox->addItems(patternNames);
//
//    // 恢复之前的选项，确保索引有效
//    if (currentIndex >= 0 && currentIndex < patternNames.size()) {
//        patternComboBox->setCurrentIndex(currentIndex); // 恢复选中的索引
//    }
//    else {
//        patternComboBox->setCurrentIndex(0);
//    }
//}
//
//// 根据PatternComboBox选中pattern对象
//void DisplayWidget::onPatternComboBoxChanged(int index)
//{
//    if (index < 0) return;  // 避免无效索引
//
//    // 获取用户选择的 pattern 名称
//    QString selectedPatternName = patternComboBox->itemText(index);
//
//    // **防止信号触发 `onPatternSelected`，导致无限循环**
//    patternList->blockSignals(true);
//
//    // 在 patternList 中找到匹配的项并选中
//    for (int i = 0; i < patternList->count(); ++i) {
//        QListWidgetItem* item = patternList->item(i);
//        if (item->text() == selectedPatternName) {
//            patternList->setCurrentItem(item);  // 选中匹配项
//            break;
//        }
//    }
//
//    patternList->blockSignals(false); // **恢复信号**
//
//    // 获取当前选择的 pattern 对象
//    auto patternPtr = PatternManager::getPatternByName(selectedPatternName);
//    if (!patternPtr) {
//        return;
//    }
//
//    layerList->clear();
//
//    // 获取当前 pattern 中的所有 Layer
//    auto layers = patternPtr->getLayers();
//
//    // 将每个 Layer 添加到 layerList 中
//    for (const auto& layer : layers) {
//        if (layer) {
//            QListWidgetItem* layerItem = new QListWidgetItem(layer->getLayerName());
//            layerList->addItem(layerItem);
//        }
//    }
//
//    onPatternSelected();
//}
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//void DisplayWidget::addLayer()
//{
//    // Lambda 处理警告消息
//    auto showWarning = [this](const QString& message) {
//        QMessageBox::warning(this, "Warning", message);
//        };
//
//    // 获取当前选中的 Pattern
//    QListWidgetItem* selectedItem = patternList->currentItem();
//    if (!selectedItem) {
//        showWarning("No pattern selected!");
//        return;
//    }
//
//    QString patternName = selectedItem->text();
//    auto patternPtr = PatternManager::getPatternByName(patternName);
//    if (!patternPtr) {
//        showWarning("Pattern not found!");
//        return;
//    }
//
//    // 弹出对话框让用户选择 Layer 类型
//    QStringList layerTypes = { "Mono", "Gradient", "Checker", "BitMap", "CRect", "CCircle", "Line", "Text", "Type 1", "Type 2" };
//    bool ok;
//    QString selectedLayerType = QInputDialog::getItem(this, "Select Layer Type", "Layer Type:", layerTypes, 0, false, &ok);
//
//    if (!ok || selectedLayerType.isEmpty()) {
//        return;  // 用户取消选择
//    }
//
//    // 获取下一个可用的 Layer 编号，并格式化名称
//    int nextLayerNum = PatternManager::getNextLayerNumber(patternName);
//    QString layerName = QString("Layer %1").arg(nextLayerNum);
//
//    // 使用 map 来动态创建不同类型的 Layer
//    std::shared_ptr<Layer> newLayer = PatternManager::createLayerByType(selectedLayerType);
//    if (!newLayer) {
//        showWarning("Failed to create layer of type: " + selectedLayerType);
//        return;
//    }
//
//    // 设置 Layer 属性
//    newLayer->setLayerName(layerName);
//    newLayer->setLayerType(selectedLayerType);
//    newLayer->setLayerStatus(true);
//    newLayer->setLayerDescription("New Layer");
//
//    // 将新 Layer 添加到 Pattern 中
//    patternPtr->addLayer(newLayer);
//
//    // 将新 Layer 添加layerList
//    auto layerItem = new QListWidgetItem(layerName);
//    layerList->addItem(layerItem);
//
//    // 选中新创建的 Layer
//    layerList->clearSelection();
//    layerList->setCurrentItem(layerItem);
//    layerItem->setSelected(true);
//
//    // 更新图层列表UI
//    updateLayerListView(patternPtr);
//}
//
//void DisplayWidget::removeLayer()
//{
//    // Lambda 处理警告消息
//    auto showWarning = [this](const QString& message) {
//        QMessageBox::warning(this, "Warning", message);
//        };
//
//    // 获取当前选中的 Pattern
//    QListWidgetItem* selectedItem = patternList->currentItem();
//    if (!selectedItem) {
//        showWarning("No pattern selected!");
//        return;
//    }
//
//    QString patternName = selectedItem->text();
//    auto patternPtr = PatternManager::getPatternByName(patternName);
//    if (!patternPtr) {
//        showWarning("Pattern not found!");
//        return;
//    }
//
//    // 获取所有选中的 Layer
//    QList<QListWidgetItem*> selectedLayers = layerList->selectedItems();
//    if (selectedLayers.isEmpty()) {
//        showWarning("No layer selected!");
//        return;
//    }
//
//    // 确认删除对话框
//    QString message = "Are you sure you want to delete the selected layers?\n\n";
//    for (auto* item : selectedLayers) {
//        message += "- " + item->text() + "\n";
//    }
//    int reply = QMessageBox::question(this, "Confirm Deletion", message,
//        QMessageBox::Yes | QMessageBox::No);
//    if (reply != QMessageBox::Yes) {
//        return;
//    }
//
//    // 批量删除选中的 Layer
//    bool updated = false;
//    for (auto* item : selectedLayers) {
//        QString layerName = item->text();
//        bool success = PatternManager::removeLayerFromPattern(patternName, layerName);
//        if (success) {
//            delete layerList->takeItem(layerList->row(item));  // 从 UI 列表中移除
//        }
//        else {
//            showWarning("Failed to remove layer: " + layerName);
//        }
//    }
//
//    // 如果有成功删除的项，则更新 UI
//    if (updated) {
//        updateLayerListView(patternPtr);
//    }
//}
//
//void DisplayWidget::onLayerSelected()
//{
//    // 获取当前选中的 Layer 项
//    QListWidgetItem* selectedItem = layerList->currentItem();
//    if (!selectedItem) {
//        return;
//    }
//
//    QString layerName = selectedItem->text();  // 获取 Layer 的名称
//
//    // 获取当前 Pattern
//    QListWidgetItem* selectedPatternItem = patternList->currentItem();
//    if (!selectedPatternItem) {
//        QMessageBox::warning(this, "Warning", "No pattern selected5!");
//        return;
//    }
//
//    QString patternName = selectedPatternItem->text();
//    auto patternPtr = PatternManager::getPatternByName(patternName);
//    if (!patternPtr) {
//        QMessageBox::warning(this, "Warning", "Pattern not found!");
//        return;
//    }
//
//    // 在当前 Pattern 中查找该 Layer
//    auto layers = patternPtr->getLayers();
//    std::shared_ptr<Layer> foundLayer;
//    for (const auto& layer : layers) {
//        if (layer && layer->getLayerName() == layerName) {
//            foundLayer = layer;
//            break;
//        }
//    }
//
//    if (!foundLayer) {
//        return;
//    }
//
//    layerNameEdit->setText(foundLayer->getLayerName());
//    layerTypeComboBox->setCurrentText(foundLayer->getLayerType());
//    layerStatusCheckBox->setChecked(foundLayer->getLayerStatus());
//    layerDescriptionEdit->setText(foundLayer->getLayerDescription());
//}
//
//void DisplayWidget::updateLayerListView(const std::shared_ptr<Pattern>& patternPtr)
//{
//    // 获取pattern名称
//    QString patternName = patternPtr->getPatternName();
//
//    // 如果 patternList 没有选中项，则不执行更新
//    if (!patternList->currentItem()) {
//        //qWarning() << "updateLayerListView: No pattern selected, skipping update.";
//        return;
//    }
//
//    if (!patternPtr) {
//        /*qWarning() << "updateLayerListView: Invalid pattern pointer!";*/
//        return;
//    }
//
//    if (!layerList) {
//        /*qWarning() << "updateLayerListView: layerList is not initialized!";*/
//        return;
//    }
//
//    // 记录当前选中的图层索引
//    int selectedIndex = layerList->currentRow();
//
//    // 清空现有的图层列表
//    layerList->clear();
//
//    // 获取所有图层，并更新UI
//    auto layers = patternPtr->getLayers();
//    for (const auto& layer : layers) {
//        if (layer) {
//            layerList->addItem(layer->getLayerName());
//        }
//    }
//
//    // 如果 layerList 为空，则不选择任何项
//    if (layerList->count() == 0) {
//        return;
//    }
//
//    // 只有在 patternList 有选中项时，才执行选中 layerList 的逻辑
//    if (selectedIndex >= 0 && selectedIndex < layerList->count()) {
//        layerList->setCurrentRow(selectedIndex);
//    }
//    else {
//        layerList->setCurrentRow(0);  // 选中第一项
//    }
//}
//
//void DisplayWidget::saveLayerInfo()
//{
//    // 获取layerList当前选中的 Layer
//    int selectedRow = layerList->currentRow();
//    if (selectedRow < 0) {
//        return;
//    }
//
//    // 获取当前选中的 Pattern
//    QString selectedPatternName = patternComboBox->currentText();
//    auto patternPtr = PatternManager::getPatternByName(selectedPatternName);
//    if (!patternPtr) return;
//
//    // 获取所有 Layer
//    auto layers = patternPtr->getLayers();
//    if (selectedRow >= layers.size()) return;
//
//    // 获取当前选中的 Layer
//    auto oldLayerPtr = layers[selectedRow];
//    if (!oldLayerPtr) return;
//
//    // 从 UI 控件获取 Layer 信息
//    QString layerName = layerNameEdit->text();
//    QString newLayerType = layerTypeComboBox->currentText();
//    bool layerStatus = layerStatusCheckBox->isChecked();
//    QString layerDescription = layerDescriptionEdit->toPlainText();
//
//    // 检查 Layer 名称是否合法
//    if (layerName.isEmpty()) {
//        QMessageBox::warning(this, "Warning", "Layer name cannot be empty!");
//        return;
//    }
//
//    // 检查 Layer 名称是否已存在（防止重名）
//    for (const auto& existingLayer : layers) {
//        if (existingLayer->getLayerName() == layerName && existingLayer != oldLayerPtr) {
//            QMessageBox::warning(this, "Warning", "Layer name already exists!");
//            return;
//        }
//    }
//
//    // 确认修改
//    if (QMessageBox::question(this, "Confirm Update", "Are you sure you want to update this layer?",
//        QMessageBox::Yes | QMessageBox::No) == QMessageBox::No) {
//        return;
//    }
//
//    // LayerType 变更时，创建新的 Layer 并替换
//    if (oldLayerPtr->getLayerType() != newLayerType) {
//        std::shared_ptr<Layer> newLayerPtr = createNewLayer(newLayerType);
//        if (!newLayerPtr) {
//            QMessageBox::warning(this, "Warning", "Invalid Layer Type!");
//            return;
//        }
//
//        // 迁移共有属性
//        newLayerPtr->setLayerName(layerName);
//        newLayerPtr->setLayerType(newLayerType);
//        newLayerPtr->setLayerStatus(layerStatus);
//        newLayerPtr->setLayerDescription(layerDescription);
//
//        // 替换原来的 Layer
//        patternPtr->replaceLayer(selectedRow, newLayerPtr);
//    }
//    else {
//        // LayerType 不变，直接修改属性
//        oldLayerPtr->setLayerName(layerName);
//        oldLayerPtr->setLayerStatus(layerStatus);
//        oldLayerPtr->setLayerDescription(layerDescription);
//    }
//
//    updateLayerListView(patternPtr);
//
//    // 在 layerList 中重新选中当前修改的 Layer
//    for (int i = 0; i < layerList->count(); ++i) {
//        QListWidgetItem* item = layerList->item(i);
//        if (item->text() == layerName) {
//            layerList->setCurrentItem(item);
//            break;
//        }
//    }
//}
//
//// 创建 Layer 实例
//std::shared_ptr<Layer> DisplayWidget::createNewLayer(const QString& layerType)
//{
//    if (layerType == "Mono") return std::make_shared<MonoLayer>();
//    if (layerType == "Gradient") return std::make_shared<GradientLayer>();
//    if (layerType == "Checker") return std::make_shared<CheckerLayer>();
//    if (layerType == "BitMap") return std::make_shared<BitMapLayer>();
//    if (layerType == "CRect") return std::make_shared<CRectLayer>();
//    if (layerType == "CCircle") return std::make_shared<CCircleLayer>();
//    if (layerType == "Line") return std::make_shared<LineLayer>();
//    if (layerType == "Text") return std::make_shared<TextLayer>();
//    return nullptr;
//}
//
//void DisplayWidget::editLayerInfo()
//{
//    // 获取当前选中的 Pattern
//    QString selectedPatternName = patternComboBox->currentText();
//    auto patternPtr = PatternManager::getPatternByName(selectedPatternName);
//    if (!patternPtr) { return; }
//
//    int selectedRow = layerList->currentRow();
//    if (selectedRow < 0) { return; }
//
//    auto layerPtr = patternPtr->getLayers()[selectedRow];
//    if (!layerPtr) return;
//
//    QString layerType = layerPtr->getLayerType();
//
//    // 通用模板函数来创建并处理对话框
//    auto openDialog = [&](auto* layer, auto* dialogType) {
//        using LayerType = std::remove_pointer_t<decltype(layer)>;
//        using DialogType = std::remove_pointer_t<decltype(dialogType)>;
//
//        DialogType dialog(static_cast<LayerType*>(layerPtr.get()));
//        if (dialog.exec() == QDialog::Accepted) {
//            layerPtr->setParameters(dialog.getLayerParameters());
//        }
//        };
//
//    //QMap<QString, std::function<void()>> dialogMap{
//   //    {"Mono", [&]() {
//   //        MonoLayerConfigDialog dialog(static_cast<MonoLayer*>(layerPtr.get()));
//   //        if (dialog.exec() == QDialog::Accepted) {
//   //            layerPtr->setParameters(dialog.getLayerParameters());
//   //        }
//   //    }},
//   //};
//
//    // 定义映射：LayerType -> 对应的 ConfigDialog
//    static const QMap<QString, std::function<void()>> dialogMap{
//        {"Mono",     [&]() { openDialog(static_cast<MonoLayer*>(nullptr), static_cast<MonoLayerConfigDialog*>(nullptr)); }},
//        {"Gradient", [&]() { openDialog(static_cast<GradientLayer*>(nullptr), static_cast<GradientLayerConfigDialog*>(nullptr)); }},
//        {"Checker",  [&]() { openDialog(static_cast<CheckerLayer*>(nullptr), static_cast<CheckerLayerConfigDialog*>(nullptr)); }},
//        {"BitMap",   [&]() { openDialog(static_cast<BitMapLayer*>(nullptr), static_cast<BitMapLayerConfigDialog*>(nullptr)); }},
//        {"CRect",    [&]() { openDialog(static_cast<CRectLayer*>(nullptr), static_cast<CRectLayerConfigDialog*>(nullptr)); }},
//        {"CCircle",  [&]() { openDialog(static_cast<CCircleLayer*>(nullptr), static_cast<CCircleLayerConfigDialog*>(nullptr)); }},
//        {"Line",     [&]() { openDialog(static_cast<LineLayer*>(nullptr), static_cast<LineLayerConfigDialog*>(nullptr)); }},
//        {"Text",     [&]() { openDialog(static_cast<TextLayer*>(nullptr), static_cast<TextLayerConfigDialog*>(nullptr)); }}
//    };
//
//    if (dialogMap.contains(layerType)) {
//        dialogMap[layerType]();
//    }
//}
//
//// 输出所有Pattern的详细信息 (调试用)
//void DisplayWidget::outputAllPatternInfo()
//{
//    PatternManager patternManager;
//    patternManager.printPatternInfo();
//
//}
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// I/O
//
//void DisplayWidget::saveConfiguration() {
//    // 获取当前所有的 Pattern 和 Layer 配置
//    QJsonObject rootObject;
//
//    // 保存 Pattern 配置信息
//    QJsonArray patternArray;
//    for (const auto& pattern : PatternManager::getAllPatterns()) {
//        QJsonObject patternObject;
//        patternObject["patternName"] = pattern->getPatternName();
//        patternObject["patternWidth"] = pattern->getPatternWidth();
//        patternObject["patternHeight"] = pattern->getPatternHeight();
//        patternObject["patternDescription"] = pattern->getPatternDescription();
//
//        // 保存图层信息
//        QJsonArray layerArray;
//        for (const auto& layer : pattern->getLayers()) {
//            QJsonObject layerObject;
//            layerObject["layerName"] = layer->getLayerName();
//            layerObject["layerType"] = layer->getLayerType();
//            layerObject["layerStatus"] = layer->getLayerStatus();
//            layerObject["layerDescription"] = layer->getLayerDescription();
//
//            QString layerType = layer->getLayerType();
//            if (layerType == "Mono") {
//                processMonoLayer(layer.get(), layerObject);
//            }
//            else if (layerType == "Gradient") {
//                processGradientLayer(layer.get(), layerObject);
//            }
//            else if (layerType == "Checker") {
//                processCheckerLayer(layer.get(), layerObject);
//            }
//            else if (layerType == "CRect") {
//                processCRectLayer(layer.get(), layerObject);
//            }
//            else if (layerType == "CCircle") {
//                processCCircleLayer(layer.get(), layerObject);
//            }
//            else if (layerType == "BitMap") {
//                processBitMapLayer(layer.get(), layerObject);
//            }
//            else if (layerType == "Line") {
//                processLineLayer(layer.get(), layerObject);
//            }
//            else if (layerType == "Text") {
//                processTextLayer(layer.get(), layerObject);
//            }
//
//            layerArray.append(layerObject);
//        }
//        patternObject["layers"] = layerArray;
//        patternArray.append(patternObject);
//    }
//
//    rootObject["patterns"] = patternArray;
//
//    // 将数据写入文件
//    QFile saveFile("config.json");
//    if (saveFile.open(QIODevice::WriteOnly)) {
//        QJsonDocument saveDoc(rootObject);
//        saveFile.write(saveDoc.toJson());
//        //qDebug() << "Saving configuration to file: " << saveDoc.toJson();  // 打印保存的 JSON 内容
//    }
//    else {
//        QMessageBox::critical(nullptr, "Save Failed", "Unable to open the file for saving. Please check permissions or disk space!");
//    }
//}
//
//// 各Layer类型写入Json文件函数
//void DisplayWidget::processMonoLayer(Layer* layer, QJsonObject& layerObject) {
//    MonoLayer* monoLayer = dynamic_cast<MonoLayer*>(layer);
//    if (!monoLayer) return;
//
//    QMap<QString, QVariant> params = monoLayer->getParameters();
//    QJsonObject monoLayerParams;
//    monoLayerParams["width"] = params["width"].toInt();
//    monoLayerParams["height"] = params["height"].toInt();
//    monoLayerParams["alpha"] = params["alpha"].toInt();
//    monoLayerParams["borderWidth"] = params["borderWidth"].toInt();
//    monoLayerParams["borderHeight"] = params["borderHeight"].toInt();
//    monoLayerParams["borderFillColor"] = params["borderFillColor"].toBool();
//    monoLayerParams["backgroundColor"] = params["backgroundColor"].value<QColor>().name(QColor::HexArgb);
//    monoLayerParams["borderColor"] = params["borderColor"].value<QColor>().name(QColor::HexArgb);
//
//    layerObject["monoLayerParams"] = monoLayerParams;
//}
//
//void DisplayWidget::processGradientLayer(Layer* layer, QJsonObject& layerObject) {
//    GradientLayer* gradientLayer = dynamic_cast<GradientLayer*>(layer);
//    if (!gradientLayer) return;
//
//    QMap<QString, QVariant> params = gradientLayer->getParameters();
//    QJsonObject gradientLayerParams;
//    gradientLayerParams["width"] = params["width"].toInt();
//    gradientLayerParams["height"] = params["height"].toInt();
//    gradientLayerParams["alpha"] = params["alpha"].toInt();
//    gradientLayerParams["counts"] = params["counts"].toInt();
//    gradientLayerParams["orientation"] = params["orientation"].toString();
//
//    // 处理 gradientBars
//    QJsonArray gradientBarsArray;
//    QList<QMap<QString, QVariant>> gradientBars = params["gradientBars"].value<QList<QMap<QString, QVariant>>>();
//
//    for (const auto& gradientBar : gradientBars) {
//        QJsonObject barJson;
//        barJson["steps"] = gradientBar["steps"].toInt();
//        barJson["startColor"] = gradientBar["startColor"].value<QColor>().name(QColor::HexArgb);
//        barJson["endColor"] = gradientBar["endColor"].value<QColor>().name(QColor::HexArgb);
//
//        gradientBarsArray.append(barJson);
//    }
//
//    gradientLayerParams["gradientBars"] = gradientBarsArray;
//
//    layerObject["gradientLayerParams"] = gradientLayerParams;
//}
//
//void DisplayWidget::processCheckerLayer(Layer* layer, QJsonObject& layerObject) {
//    CheckerLayer* checkerLayer = dynamic_cast<CheckerLayer*>(layer);
//    if (!checkerLayer) return;
//
//    QMap<QString, QVariant> params = checkerLayer->getParameters();
//    QJsonObject checkerLayerParams;
//    checkerLayerParams["width"] = params["width"].toInt();
//    checkerLayerParams["height"] = params["height"].toInt();
//    checkerLayerParams["alpha"] = params["alpha"].toInt();
//    checkerLayerParams["rows"] = params["rows"].toInt();
//    checkerLayerParams["cols"] = params["cols"].toInt();
//    checkerLayerParams["color1"] = params["color1"].value<QColor>().name(QColor::HexArgb);
//    checkerLayerParams["color2"] = params["color2"].value<QColor>().name(QColor::HexArgb);
//
//    layerObject["checkerLayerParams"] = checkerLayerParams;
//}
//
//void DisplayWidget::processCRectLayer(Layer* layer, QJsonObject& layerObject) {
//    CRectLayer* crectLayer = dynamic_cast<CRectLayer*>(layer);
//    if (!crectLayer) return;
//
//    QMap<QString, QVariant> params = crectLayer->getParameters();
//    QJsonObject crectLayerParams;
//    crectLayerParams["width"] = params["width"].toInt();
//    crectLayerParams["height"] = params["height"].toInt();
//    crectLayerParams["alpha"] = params["alpha"].toInt();
//    crectLayerParams["counts"] = params["counts"].toInt();
//    crectLayerParams["color1"] = params["color1"].value<QColor>().name(QColor::HexArgb);
//    crectLayerParams["color2"] = params["color2"].value<QColor>().name(QColor::HexArgb);
//
//    layerObject["crectLayerParams"] = crectLayerParams;
//}
//
//void DisplayWidget::processCCircleLayer(Layer* layer, QJsonObject& layerObject) {
//    CCircleLayer* ccircleLayer = dynamic_cast<CCircleLayer*>(layer);
//    if (!ccircleLayer) return;
//
//    QMap<QString, QVariant> params = ccircleLayer->getParameters();
//    QJsonObject ccircleLayerParams;
//    ccircleLayerParams["width"] = params["width"].toInt();
//    ccircleLayerParams["height"] = params["height"].toInt();
//    ccircleLayerParams["alpha"] = params["alpha"].toInt();
//    ccircleLayerParams["centerX"] = params["centerX"].toInt();
//    ccircleLayerParams["centerY"] = params["centerY"].toInt();
//    ccircleLayerParams["counts"] = params["counts"].toInt();
//    ccircleLayerParams["concentric"] = params["concentric"].toBool();
//    ccircleLayerParams["radiusX"] = params["radiusX"].toInt();
//    ccircleLayerParams["radiusY"] = params["radiusY"].toInt();
//    ccircleLayerParams["lineWidth"] = params["lineWidth"].toInt();
//    ccircleLayerParams["color1"] = params["color1"].value<QColor>().name(QColor::HexArgb);
//    ccircleLayerParams["color2"] = params["color2"].value<QColor>().name(QColor::HexArgb);
//
//    layerObject["ccircleLayerParams"] = ccircleLayerParams;
//}
//
//void DisplayWidget::processBitMapLayer(Layer* layer, QJsonObject& layerObject) {
//    BitMapLayer* bitmapLayer = dynamic_cast<BitMapLayer*>(layer);
//    if (!bitmapLayer) return;
//
//    QMap<QString, QVariant> params = bitmapLayer->getParameters();
//    QJsonObject bitmapLayerParams;
//    bitmapLayerParams["width"] = params["width"].toInt();
//    bitmapLayerParams["height"] = params["height"].toInt();
//    bitmapLayerParams["alpha"] = params["alpha"].toInt();
//    bitmapLayerParams["bitMapWidth"] = params["bitMapWidth"].toInt();
//    bitmapLayerParams["bitMapHeight"] = params["bitMapHeight"].toInt();
//    bitmapLayerParams["color"] = params["color"].value<QColor>().name(QColor::HexArgb);
//
//    layerObject["bitmapLayerParams"] = bitmapLayerParams;
//}
//
//void DisplayWidget::processLineLayer(Layer* layer, QJsonObject& layerObject) {
//    LineLayer* lineLayer = dynamic_cast<LineLayer*>(layer);
//    if (!lineLayer) return;
//
//    QMap<QString, QVariant> params = lineLayer->getParameters();
//    QJsonObject lineLayerParams;
//    lineLayerParams["width"] = params["width"].toInt();
//    lineLayerParams["height"] = params["height"].toInt();
//    lineLayerParams["alpha"] = params["alpha"].toInt();
//    lineLayerParams["startX"] = params["startX"].toInt();
//    lineLayerParams["startY"] = params["startY"].toInt();
//    lineLayerParams["endX"] = params["endX"].toInt();
//    lineLayerParams["endY"] = params["endY"].toInt();
//    lineLayerParams["lineWidth"] = params["lineWidth"].toInt();
//    lineLayerParams["backColor"] = params["backColor"].value<QColor>().name(QColor::HexArgb);
//    lineLayerParams["lineColor"] = params["lineColor"].value<QColor>().name(QColor::HexArgb);
//
//    layerObject["lineLayerParams"] = lineLayerParams;
//}
//
//void DisplayWidget::processTextLayer(Layer* layer, QJsonObject& layerObject) {
//    TextLayer* textLayer = dynamic_cast<TextLayer*>(layer);
//    if (!textLayer) return;
//
//    QMap<QString, QVariant> params = textLayer->getParameters();
//    QJsonObject textLayerParams;
//    textLayerParams["width"] = params["width"].toInt();
//    textLayerParams["height"] = params["height"].toInt();
//    textLayerParams["alpha"] = params["alpha"].toInt();
//    textLayerParams["startX"] = params["startX"].toInt();
//    textLayerParams["startY"] = params["startY"].toInt();
//    textLayerParams["endX"] = params["endX"].toInt();
//    textLayerParams["endY"] = params["endY"].toInt();
//    textLayerParams["text"] = params["text"].toString();
//    textLayerParams["font"] = params["font"].toString();
//    textLayerParams["size"] = params["size"].toInt();
//    textLayerParams["style"] = params["style"].toString();
//    textLayerParams["bgColor"] = params["bgColor"].value<QColor>().name(QColor::HexArgb);
//    textLayerParams["textColor"] = params["textColor"].value<QColor>().name(QColor::HexArgb);
//
//    layerObject["textLayerParams"] = textLayerParams;
//}
//
//// 各Layer类型载入Json文件函数
//void DisplayWidget::loadConfiguration()
//{
//    QFile loadFile("config.json");
//
//    if (!loadFile.open(QIODevice::ReadOnly)) {
//        qWarning() << "Failed to open file for loading configuration.";
//        return;
//    }
//
//    QJsonDocument loadDoc = QJsonDocument::fromJson(loadFile.readAll());
//    if (loadDoc.isNull()) {
//        qWarning() << "Failed to parse configuration file.";
//        return;
//    }
//
//    // 解析 JSON 文件
//    QJsonObject rootObject = loadDoc.object();
//    QJsonArray patternArray = rootObject["patterns"].toArray();
//
//    for (const QJsonValue& patternValue : patternArray) {
//        QJsonObject patternObject = patternValue.toObject();
//        QString patternName = patternObject["patternName"].toString();
//        int patternWidth = patternObject["patternWidth"].toInt();
//        int patternHeight = patternObject["patternHeight"].toInt();
//        QString patternDescription = patternObject["patternDescription"].toString();
//
//        // 使用 addPattern 创建一个新的 Pattern，并传入读取的属性
//        auto patternPtr = PatternManager::loadPattern(patternName, patternWidth, patternHeight, patternDescription);
//
//        // 读取图层信息
//        QJsonArray layerArray = patternObject["layers"].toArray();
//        for (const QJsonValue& layerValue : layerArray) {
//            QJsonObject layerObject = layerValue.toObject();
//            QString layerName = layerObject["layerName"].toString();
//            QString layerType = layerObject["layerType"].toString();
//            bool layerStatus = layerObject["layerStatus"].toBool();
//            QString layerDescription = layerObject["layerDescription"].toString();
//
//            std::shared_ptr<Layer> layer = nullptr;
//
//            // 处理类型图层
//            if (layerType == "Mono") {
//                layer = createMonoLayer(layerObject, layerName);
//            }
//            else if (layerType == "Gradient") {
//                layer = createGradientLayer(layerObject, layerName);
//            }
//
//            if (layer) {
//                layer->setLayerName(layerName);
//                layer->setLayerType(layerType);
//                layer->setLayerStatus(layerStatus);
//                layer->setLayerDescription(layerDescription);
//                patternPtr->addLayer(std::shared_ptr<Layer>(layer)); // 将图层添加到 Pattern
//            }
//            updateLayerListView(patternPtr);
//        }
//    }
//    updatePatternListView();
//}
//
//// 各Layer类型载入Json文件函数
//std::shared_ptr<MonoLayer> DisplayWidget::createMonoLayer(const QJsonObject& layerObject, const QString& layerName)
//{
//    std::shared_ptr<MonoLayer> monoLayer = std::make_shared<MonoLayer>(layerName);
//    QJsonObject monoLayerParams = layerObject["monoLayerParams"].toObject();
//    QMap<QString, QVariant> params;
//    params["width"] = monoLayerParams["width"].toInt();
//    params["height"] = monoLayerParams["height"].toInt();
//    params["alpha"] = monoLayerParams["Alpha"].toInt();
//    params["borderWidth"] = monoLayerParams["borderWidth"].toInt();
//    params["borderHeight"] = monoLayerParams["borderHeight"].toInt();
//    params["borderFillColor"] = monoLayerParams["borderFillColor"].toBool();
//    params["backgroundColor"] = QColor::fromString(monoLayerParams["backgroundColor"].toString());
//    params["borderColor"] = QColor::fromString(monoLayerParams["borderColor"].toString());
//
//    monoLayer->setParameters(params);
//    return monoLayer;
//}
//
//std::shared_ptr<GradientLayer> DisplayWidget::createGradientLayer(const QJsonObject& layerObject, const QString& layerName)
//{
//    std::shared_ptr<GradientLayer> gradientLayer = std::make_shared<GradientLayer>(layerName);
//    QJsonObject gradientLayerParams = layerObject["gradientLayerParams"].toObject();
//
//    QMap<QString, QVariant> params;
//    params["width"] = gradientLayerParams["width"].toInt();
//    params["height"] = gradientLayerParams["height"].toInt();
//    params["alpha"] = gradientLayerParams["alpha"].toInt();
//    params["counts"] = gradientLayerParams["counts"].toInt();
//    params["orientation"] = gradientLayerParams["orientation"].toString();
//
//    // 处理 gradientBars
//    QJsonArray gradientBarsArray = gradientLayerParams["gradientBars"].toArray();
//    QList<QMap<QString, QVariant>> gradientBars;
//
//    for (const QJsonValue& barValue : gradientBarsArray) {
//        QJsonObject barJson = barValue.toObject();
//        QMap<QString, QVariant> gradientBar;
//        gradientBar["steps"] = barJson["steps"].toInt();
//        gradientBar["startColor"] = QColor(barJson["startColor"].toString());
//        gradientBar["endColor"] = QColor(barJson["endColor"].toString());
//
//        gradientBars.append(gradientBar);
//    }
//
//    // 将 gradientBars 转换为 QVariant
//    params["gradientBars"] = QVariant::fromValue(gradientBars);
//
//    gradientLayer->setParameters(params);
//    return gradientLayer;
//}
//
//
//
//
////void DisplayWidget::saveConfiguration() {
////    QFile saveFile("config.dat");
////    if (!saveFile.open(QIODevice::WriteOnly)) {
////        QMessageBox::critical(nullptr, "保存失败", "无法打开文件进行保存，请检查权限或磁盘空间！");
////        return;
////    }
////
////    QDataStream out(&saveFile);
////    out.setVersion(QDataStream::Qt_6_5);  // 确保 Qt 版本一致
////
////    // 写入 Pattern 数量
////    const auto& patterns = PatternManager::getAllPatterns();
////    out << patterns.size();
////    qDebug() << "Saving " << patterns.size() << " patterns";  // 调试输出
////
////    for (const auto& pattern : patterns) {
////        qDebug() << "Saving pattern: " << pattern->getPatternName();
////        out << pattern->getPatternName();
////        out << pattern->getPatternWidth();
////        out << pattern->getPatternHeight();
////        out << pattern->getPatternDescription();
////
////        // 写入图层信息
////        const auto& layers = pattern->getLayers();
////        out << layers.size();
////        qDebug() << "Saving 1111" << layers.size() << " layers for pattern" << pattern->getPatternName();  // 调试输出
////
////        for (const auto& layer : layers) {
////            out << layer->getLayerName();
////            out << layer->getLayerType();
////            out << layer->getLayerStatus();
////            out << layer->getLayerDescription();
////
////            // 处理不同类型的图层
////            if (layer->getLayerType() == "Mono") {
////                processMonoLayerSave(out, layer.get());
////            }
////            else if (layer->getLayerType() == "Gradient") {
////                processGradientLayerSave(out, layer.get());
////            }
////        }
////    }
////
////    saveFile.flush();  // 确保所有数据写入
////    qDebug() << "Configuration saved successfully";
////}
////
////
////
////void DisplayWidget::processMonoLayerSave(QDataStream& out, Layer* layer) {
////    MonoLayer* monoLayer = dynamic_cast<MonoLayer*>(layer);
////    if (!monoLayer) return;
////
////    QMap<QString, QVariant> params = monoLayer->getParameters();
////    qDebug() << "Saving MonoLayer: " << params["width"].toInt() << params["height"].toInt();  // 调试输出
////
////    out << params["width"].toInt();
////    out << params["height"].toInt();
////    out << params["alpha"].toInt();
////    out << params["borderWidth"].toInt();
////    out << params["borderHeight"].toInt();
////    out << params["borderFillColor"].toBool();
////    out << params["backgroundColor"].value<QColor>().rgba();  // 使用整数存颜色
////    out << params["borderColor"].value<QColor>().rgba();
////}
////
////void DisplayWidget::processGradientLayerSave(QDataStream& out, Layer* layer) {
////    GradientLayer* gradientLayer = dynamic_cast<GradientLayer*>(layer);
////    if (!gradientLayer) return;
////
////    QMap<QString, QVariant> params = gradientLayer->getParameters();
////    qDebug() << "Saving GradientLayer: " << params["width"].toInt() << params["height"].toInt();  // 调试输出
////
////    out << params["width"].toInt();
////    out << params["height"].toInt();
////    out << params["alpha"].toInt();
////    out << params["counts"].toInt();
////    out << params["orientation"].toString();
////
////    // 处理渐变条
////    QList<QMap<QString, QVariant>> gradientBars = params["gradientBars"].value<QList<QMap<QString, QVariant>>>();
////    out << gradientBars.size();  // 先存数量
////
////    qDebug() << "Saving gradientBars: " << gradientBars.size();  // 调试输出
////    for (const auto& gradientBar : gradientBars) {
////        out << gradientBar["steps"].toInt();
////        out << gradientBar["startColor"].value<QColor>().rgba();
////        out << gradientBar["endColor"].value<QColor>().rgba();
////    }
////}
////
//
