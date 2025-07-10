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
//    // ���� QSplitter
//    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
//
//    // ��ࣨͼ���б� + ��ť��
//    QWidget* leftWidget = new QWidget(this);
//    QVBoxLayout* leftLayout = new QVBoxLayout(leftWidget);
//
//    // Pattern�б�ť
//    QHBoxLayout* buttonLayout = new QHBoxLayout();
//    addButton = new QPushButton("Add", this);
//    delButton = new QPushButton("Del", this);
//    buttonLayout->addWidget(addButton);
//    buttonLayout->addWidget(delButton);
//
//    // Pattern�б����
//    patternList = new QListWidget(this);
//    patternList->setSelectionMode(QAbstractItemView::ExtendedSelection); // ��ѡģʽ
//    leftLayout->addLayout(buttonLayout);
//    leftLayout->addWidget(patternList);
//    leftWidget->setLayout(leftLayout);
//
//    // �ҲࣨTab ѡ���
//    tabWidget = new QTabWidget(this);
//    QWidget* tab1 = creatInformationTab();
//    QWidget* tab2 = creatLayerTab();
//    QWidget* tab3 = creatThumbnailTab();
//    tabWidget->addTab(tab1, "Information");
//    tabWidget->addTab(tab2, "Layer");
//    tabWidget->addTab(tab3, "Thumbnail");
//
//    // ��ӵ� QSplitter
//    splitter->addWidget(leftWidget);
//    splitter->addWidget(tabWidget);
//    splitter->setSizes({ 250, 700 });
//    leftWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
//    tabWidget->setMinimumWidth(1);
//    splitter->setStretchFactor(0, 1);
//    splitter->setStretchFactor(1, 1);
//
//    // ����������
//    QVBoxLayout* mainLayout = new QVBoxLayout(this);
//    mainLayout->addWidget(splitter);
//    setLayout(mainLayout);
//
//    // �����źŲ�
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
//    // ����������
//    QFormLayout* formLayout = new QFormLayout();
//
//    // ��ʼ���ؼ�
//    patternNameEdit = new QLineEdit(tab1);
//    patternWidthSpinBox = new QSpinBox(tab1);
//    patternHeightSpinBox = new QSpinBox(tab1);
//    patternDescriptionEdit = new QTextEdit(tab1);
//
//    // ���ÿ����Ⱥ͸߶�
//    patternNameEdit->setFixedWidth(180);
//    patternWidthSpinBox->setFixedWidth(100);
//    patternHeightSpinBox->setFixedWidth(100);
//    patternWidthSpinBox->setRange(1, 10000);
//    patternHeightSpinBox->setRange(1, 10000);
//    patternDescriptionEdit->setFixedSize(300, 80); // �ı����С
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
//    // ��ӵ�������
//    formLayout->addRow("Pattern Name:", patternNameEdit);
//    formLayout->addRow("Pattern Width:", patternWidthSpinBox);
//    formLayout->addRow("Pattern Height:", patternHeightSpinBox);
//    formLayout->addRow("Pattern Description:", patternDescriptionEdit);
//
//    // ���崹ֱ���֣���ˮƽ����
//    QHBoxLayout* formCenterLayout = new QHBoxLayout();
//    formCenterLayout->addStretch();
//    formCenterLayout->addLayout(formLayout);
//    formCenterLayout->addStretch();
//
//    // ��ťˮƽ����
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
//    // ������
//    tab1Layout->addLayout(formCenterLayout);
//    tab1Layout->addLayout(buttonLayout);
//    tab1->setLayout(tab1Layout);
//
//    // �����źŲ�
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
//    // ���� QSplitter
//    QSplitter* splitter = new QSplitter(Qt::Horizontal, tab2);
//
//    // ��ࣨͼ���б� + ��ť)
//    QWidget* leftWidget = new QWidget(tab2);
//    QVBoxLayout* leftLayout = new QVBoxLayout(leftWidget);
//
//    // ��ťˮƽ����
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
//    // ���Layer�б����
//    layerList = new QListWidget(tab2);
//    layerList->setSelectionMode(QAbstractItemView::ExtendedSelection);
//    leftLayout->addLayout(buttonLayout);
//    leftLayout->addWidget(layerList);
//    leftWidget->setLayout(leftLayout);
//
//    // �Ҳ�Layer������
//    QWidget* rightWidget = new QWidget(tab2);
//    QVBoxLayout* rightLayout = new QVBoxLayout(rightWidget);
//
//    QFormLayout* formLayout = new QFormLayout();
//
//    // ��ʼ���ؼ�
//    layerNameEdit = new QLineEdit(tab2);
//    layerTypeComboBox = new QComboBox(tab2);
//    layerStatusCheckBox = new QCheckBox("Active", tab2);
//    layerDescriptionEdit = new QTextEdit(tab2);
//
//    // ���ÿ����Ⱥ͸߶�
//    layerNameEdit->setFixedWidth(180);
//    layerTypeComboBox->setFixedWidth(100);
//    layerStatusCheckBox->setFixedWidth(100);
//    layerDescriptionEdit->setFixedSize(300, 80);
//
//    // ���ÿؼ���Ĭ��ֵ
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
//    // ��ӵ�������
//    formLayout->addRow("Layer Name:", layerNameEdit);
//    formLayout->addRow("Layer Type:", layerTypeComboBox);
//    formLayout->addRow("Layer Status:", layerStatusCheckBox);
//    formLayout->addRow("Layer Description:", layerDescriptionEdit);
//
//    // ���崹ֱ���֣���ˮƽ����
//    QHBoxLayout* formCenterLayout = new QHBoxLayout();
//    formCenterLayout->addStretch();
//    formCenterLayout->addLayout(formLayout);
//    formCenterLayout->addStretch();
//
//    // ��ťˮƽ����
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
//    // �������Ҳಿ����ӵ� QSplitter ��
//    splitter->addWidget(leftWidget);
//    splitter->addWidget(rightWidget);
//    splitter->setSizes({ 200, 600 });
//    leftWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
//    rightWidget->setMinimumWidth(1);
//    splitter->setStretchFactor(0, 1);
//    splitter->setStretchFactor(1, 1);
//
//    // ������
//    tab2Layout->addWidget(splitter);  // ʹ��splitter��Ϊ��Ҫ����
//    tab2->setLayout(tab2Layout);
//
//    // �����źŲ�
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
//    // ����һ���򵥵ı�ǩ
//    QLabel* label = new QLabel("This is Mode 3", tab3);
//    label->setAlignment(Qt::AlignCenter);  // ���ñ�ǩ���ݾ���
//    tab3Layout->addWidget(label);
//
//    tab3->setLayout(tab3Layout);
//
//    return tab3;
//}
//
//// ����Pattern
//void DisplayWidget::addPattern() {
//    // ������ Pattern
//    std::shared_ptr<Pattern> newPattern = PatternManager::addPattern();
//    QString newPatternName = newPattern->getPatternName();
//
//    // �����Pattern�� patternList
//    auto* item = new QListWidgetItem(QStringLiteral("%1").arg(newPattern->getPatternName()), patternList);
//    item->setData(Qt::UserRole, QVariant::fromValue(newPattern));
//    patternList->addItem(item);
//
//    // ѡ���½����ѡģʽ��Ҫ������ѡ���
//    patternList->clearSelection();
//    patternList->setCurrentItem(item);
//
//    // ���� ComboBox �еĿ�ѡ��
//    updatePatternComboBox();
//
//    // �� ComboBox ��ѡ���� Pattern �ƥ��patternName��
//    int comboBoxIndex = patternComboBox->findText(newPatternName);
//    if (comboBoxIndex != -1) {
//        patternComboBox->setCurrentIndex(comboBoxIndex);
//    }
//}
//
//// ɾ��Pattern
//void DisplayWidget::removePattern() {
//    QList<QListWidgetItem*> selectedItems = patternList->selectedItems();
//    if (selectedItems.isEmpty()) {
//        QMessageBox::warning(this, "Delete failed", "Please select the patterns to delete!");
//        return;
//    }
//
//    // ȷ��ɾ��
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
//// ��ȡPatternList/patternComboBox�е�ѡ�����ȡ������ʾ������Ϣ��ͬ��patternComboBox��ѡ�ͬ��layerList��pattern�����layer
//void DisplayWidget::onPatternSelected()
//{
//    // ��ȡ��ǰѡ�е���
//    QListWidgetItem* selectedItem = patternList->currentItem();
//    if (!selectedItem) return;
//
//    // ͨ�����ƴ� PatternManager ��ȡ Pattern
//    QString patternName = selectedItem->text();
//    auto patternPtr = PatternManager::getPatternByName(patternName);
//
//    if (!patternPtr) return;
//
//    // ���� UI ��Ϣ
//    patternNameEdit->setText(patternPtr->getPatternName());
//    patternWidthSpinBox->setValue(patternPtr->getPatternWidth());
//    patternHeightSpinBox->setValue(patternPtr->getPatternHeight());
//    patternDescriptionEdit->setText(patternPtr->getPatternDescription());
//
//    QSignalBlocker blocker(patternComboBox); // ����������ʱ�����źţ��˳�ʱ�Զ��ָ�
//    updatePatternComboBox();
//    // �� ComboBox ���ҵ�ƥ��� PatternName ��ѡ��
//    int comboBoxIndex = patternComboBox->findText(patternName);
//    if (comboBoxIndex != -1) {
//        patternComboBox->setCurrentIndex(comboBoxIndex);
//    }
//
//    // ���� Layer �б�
//    updateLayerListView(patternPtr);
//}
//
//// ����PatternList�еĶ���
//void DisplayWidget::updatePatternListView()
//{
//    patternList->clear();
//
//    // ���� PatternManager �е����� Pattern ��ӵ� patternList
//    const QVector<std::shared_ptr<Pattern>>& patterns = PatternManager::getAllPatterns();
//    for (const auto& pattern : patterns) {
//        QListWidgetItem* item = new QListWidgetItem(pattern->getPatternName());
//        patternList->addItem(item);
//    }
//
//    updatePatternComboBox();
//}
//
//// �༭��ǰPattern
////void DisplayWidget::editPatternInformation()
////{
////    // �����Ի���
////    QDialog dialog(this);
////    dialog.setWindowTitle("Edit Layers");
////    dialog.resize(800, 500);  // �ʵ�������С
////
////    // ���� QSplitter
////    QSplitter* splitter = new QSplitter(Qt::Horizontal, &dialog);
////
////    // ��ࣨͼ���б� + ��ť��
////    QWidget* leftWidget = new QWidget(&dialog);
////    QVBoxLayout* leftLayout = new QVBoxLayout(leftWidget);
////
////    // ��ťˮƽ����
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
////    // ���Layer�б����
////    QListWidget* layerList = new QListWidget(&dialog);
////    layerList->setSelectionMode(QAbstractItemView::ExtendedSelection);
////    leftLayout->addLayout(buttonLayout);
////    leftLayout->addWidget(layerList);
////    leftWidget->setLayout(leftLayout);
////
////    // �Ҳ� Layer ������
////    QWidget* rightWidget = new QWidget(&dialog);
////    QVBoxLayout* rightLayout = new QVBoxLayout(rightWidget);
////    QFormLayout* formLayout = new QFormLayout();
////
////    // ��ʼ���ؼ�
////    QLineEdit* layerNameEdit = new QLineEdit(&dialog);
////    QComboBox* layerTypeComboBox = new QComboBox(&dialog);
////    QCheckBox* layerStatusCheckBox = new QCheckBox("Active", &dialog);
////    QTextEdit* layerDescriptionEdit = new QTextEdit(&dialog);
////
////    // ���ÿؼ���С
////    layerNameEdit->setFixedWidth(180);
////    layerTypeComboBox->setFixedWidth(100);
////    layerStatusCheckBox->setFixedWidth(100);
////    layerDescriptionEdit->setFixedSize(300, 80);
////
////    // ���ÿؼ���Ĭ��ֵ
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
////    // ��ӵ�������
////    formLayout->addRow("Layer Name:", layerNameEdit);
////    formLayout->addRow("Layer Type:", layerTypeComboBox);
////    formLayout->addRow("Layer Status:", layerStatusCheckBox);
////    formLayout->addRow("Layer Description:", layerDescriptionEdit);
////
////    // ���б�
////    QHBoxLayout* formCenterLayout = new QHBoxLayout();
////    formCenterLayout->addStretch();
////    formCenterLayout->addLayout(formLayout);
////    formCenterLayout->addStretch();
////
////    // ��ťˮƽ����
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
////    // �������Ҳಿ����ӵ� QSplitter ��
////    splitter->addWidget(leftWidget);
////    splitter->addWidget(rightWidget);
////    splitter->setSizes({ 200, 600 });
////    leftWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
////    rightWidget->setMinimumWidth(1);
////    splitter->setStretchFactor(0, 1);
////    splitter->setStretchFactor(1, 1);
////
////    // ������
////    QVBoxLayout* mainLayout = new QVBoxLayout(&dialog);
////    mainLayout->addWidget(splitter);
////    dialog.setLayout(mainLayout);
////
////    // �����źŲ�
////    connect(addLayerButton, &QPushButton::clicked, this, &DisplayWidget::addLayer);
////    connect(delLayerButton, &QPushButton::clicked, this, &DisplayWidget::removeLayer);
////    connect(layerList, &QListWidget::itemSelectionChanged, this, &DisplayWidget::onLayerSelected);
////    connect(saveLayerButton, &QPushButton::clicked, this, &DisplayWidget::saveLayerInfo);
////    connect(editLayerButton, &QPushButton::clicked, this, &DisplayWidget::editLayerInfo);
////    connect(patternComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &DisplayWidget::onPatternComboBoxChanged);
////
////    // ��ʾ�Ի���
////    dialog.exec();
////}
//
//
//// ���浱ǰPattern��Ϣ
//void DisplayWidget::savePatternInformation()
//{
//    // ȷ�϶Ի���
//    if (QMessageBox::question(this, "Save Confirmation", "Are you sure you want to save the current pattern information?",
//        QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) {
//        return;
//    }
//
//    // Lambda ��������Ϣ
//    auto showWarning = [this](const QString& message) {
//        QMessageBox::warning(this, "Warning", message);
//        };
//
//    // ��ȡ���������
//    QString name = patternNameEdit->text().trimmed();
//    int width = patternWidthSpinBox->value();
//    int height = patternHeightSpinBox->value();
//    QString description = patternDescriptionEdit->toPlainText();
//
//    // ���ѡ�е� Pattern
//    QListWidgetItem* selectedItem = patternList->currentItem();
//    if (!selectedItem) {
//        showWarning("No pattern selected!");
//        return;
//    }
//
//    // ����ѡ�е� Pattern
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
//    // ��������Ƿ��Ѵ���
//    for (const auto& existingPattern : PatternManager::getAllPatterns()) {
//        if (existingPattern != selectedPattern && existingPattern->getPatternName() == name) {
//            showWarning("A pattern with this name already exists!");
//            return;
//        }
//    }
//
//    // ���� Pattern ��Ϣ
//    selectedPattern->setPatternName(name);
//    selectedPattern->setPatternWidth(width);
//    selectedPattern->setPatternHeight(height);
//    selectedPattern->setPatternDescription(description);
//
//    // ���� UI ��ʾ
//    selectedItem->setText(name);
//    updatePatternComboBox();
//}
//
//// ������ǰPattern�޸ĵ���Ϣ
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
//// ���Ƶ�ǰPattern(�������Layer)
//void DisplayWidget::onPatternSelectedDraw()
//{
//    // Lambda ��������Ϣ
//    auto showWarning = [this](const QString& message) {
//        QMessageBox::warning(this, "Warning", message);
//        };
//
//    // ��ȡ��ǰѡ�е� Pattern ����
//    QListWidgetItem* selectedItem = patternList->currentItem();
//    if (!selectedItem) {
//        showWarning("No pattern selected!");
//        return;
//    }
//
//    // ͨ�� PatternManager ���� Pattern
//    std::shared_ptr<Pattern> selectedPattern = PatternManager::getPatternByName(selectedItem->text());
//    if (!selectedPattern) {
//        showWarning("Invalid pattern selection!");
//        return;
//    }
//
//    // ����Ƿ��� Layer
//    if (selectedPattern->getLayers().isEmpty()) {
//        showWarning("The selected pattern has no Layers!\n" "Please continue after adding the layer.");
//        return;
//    }
//
//    // ��ʾ PatternDrawDialog
//    QScopedPointer<PatternDrawDialog> drawDialog(new PatternDrawDialog(selectedPattern));
//    drawDialog->exec();
//}
//
//// ����PatternComboBox�е�����ѡ��
//void DisplayWidget::updatePatternComboBox() {
//    // ��¼��ǰѡ�е���
//    int currentIndex = patternComboBox->currentIndex();
//    patternComboBox->clear();
//
//    // ��ȡ���� Pattern ���������
//    QStringList patternNames = PatternManager::getAllPatternNames();
//    if (patternNames.isEmpty()) {
//        patternComboBox->setCurrentIndex(-1);
//        return;
//    }
//    patternComboBox->addItems(patternNames);
//
//    // �ָ�֮ǰ��ѡ�ȷ��������Ч
//    if (currentIndex >= 0 && currentIndex < patternNames.size()) {
//        patternComboBox->setCurrentIndex(currentIndex); // �ָ�ѡ�е�����
//    }
//    else {
//        patternComboBox->setCurrentIndex(0);
//    }
//}
//
//// ����PatternComboBoxѡ��pattern����
//void DisplayWidget::onPatternComboBoxChanged(int index)
//{
//    if (index < 0) return;  // ������Ч����
//
//    // ��ȡ�û�ѡ��� pattern ����
//    QString selectedPatternName = patternComboBox->itemText(index);
//
//    // **��ֹ�źŴ��� `onPatternSelected`����������ѭ��**
//    patternList->blockSignals(true);
//
//    // �� patternList ���ҵ�ƥ����ѡ��
//    for (int i = 0; i < patternList->count(); ++i) {
//        QListWidgetItem* item = patternList->item(i);
//        if (item->text() == selectedPatternName) {
//            patternList->setCurrentItem(item);  // ѡ��ƥ����
//            break;
//        }
//    }
//
//    patternList->blockSignals(false); // **�ָ��ź�**
//
//    // ��ȡ��ǰѡ��� pattern ����
//    auto patternPtr = PatternManager::getPatternByName(selectedPatternName);
//    if (!patternPtr) {
//        return;
//    }
//
//    layerList->clear();
//
//    // ��ȡ��ǰ pattern �е����� Layer
//    auto layers = patternPtr->getLayers();
//
//    // ��ÿ�� Layer ��ӵ� layerList ��
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
//    // Lambda ��������Ϣ
//    auto showWarning = [this](const QString& message) {
//        QMessageBox::warning(this, "Warning", message);
//        };
//
//    // ��ȡ��ǰѡ�е� Pattern
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
//    // �����Ի������û�ѡ�� Layer ����
//    QStringList layerTypes = { "Mono", "Gradient", "Checker", "BitMap", "CRect", "CCircle", "Line", "Text", "Type 1", "Type 2" };
//    bool ok;
//    QString selectedLayerType = QInputDialog::getItem(this, "Select Layer Type", "Layer Type:", layerTypes, 0, false, &ok);
//
//    if (!ok || selectedLayerType.isEmpty()) {
//        return;  // �û�ȡ��ѡ��
//    }
//
//    // ��ȡ��һ�����õ� Layer ��ţ�����ʽ������
//    int nextLayerNum = PatternManager::getNextLayerNumber(patternName);
//    QString layerName = QString("Layer %1").arg(nextLayerNum);
//
//    // ʹ�� map ����̬������ͬ���͵� Layer
//    std::shared_ptr<Layer> newLayer = PatternManager::createLayerByType(selectedLayerType);
//    if (!newLayer) {
//        showWarning("Failed to create layer of type: " + selectedLayerType);
//        return;
//    }
//
//    // ���� Layer ����
//    newLayer->setLayerName(layerName);
//    newLayer->setLayerType(selectedLayerType);
//    newLayer->setLayerStatus(true);
//    newLayer->setLayerDescription("New Layer");
//
//    // ���� Layer ��ӵ� Pattern ��
//    patternPtr->addLayer(newLayer);
//
//    // ���� Layer ���layerList
//    auto layerItem = new QListWidgetItem(layerName);
//    layerList->addItem(layerItem);
//
//    // ѡ���´����� Layer
//    layerList->clearSelection();
//    layerList->setCurrentItem(layerItem);
//    layerItem->setSelected(true);
//
//    // ����ͼ���б�UI
//    updateLayerListView(patternPtr);
//}
//
//void DisplayWidget::removeLayer()
//{
//    // Lambda ��������Ϣ
//    auto showWarning = [this](const QString& message) {
//        QMessageBox::warning(this, "Warning", message);
//        };
//
//    // ��ȡ��ǰѡ�е� Pattern
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
//    // ��ȡ����ѡ�е� Layer
//    QList<QListWidgetItem*> selectedLayers = layerList->selectedItems();
//    if (selectedLayers.isEmpty()) {
//        showWarning("No layer selected!");
//        return;
//    }
//
//    // ȷ��ɾ���Ի���
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
//    // ����ɾ��ѡ�е� Layer
//    bool updated = false;
//    for (auto* item : selectedLayers) {
//        QString layerName = item->text();
//        bool success = PatternManager::removeLayerFromPattern(patternName, layerName);
//        if (success) {
//            delete layerList->takeItem(layerList->row(item));  // �� UI �б����Ƴ�
//        }
//        else {
//            showWarning("Failed to remove layer: " + layerName);
//        }
//    }
//
//    // ����гɹ�ɾ���������� UI
//    if (updated) {
//        updateLayerListView(patternPtr);
//    }
//}
//
//void DisplayWidget::onLayerSelected()
//{
//    // ��ȡ��ǰѡ�е� Layer ��
//    QListWidgetItem* selectedItem = layerList->currentItem();
//    if (!selectedItem) {
//        return;
//    }
//
//    QString layerName = selectedItem->text();  // ��ȡ Layer ������
//
//    // ��ȡ��ǰ Pattern
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
//    // �ڵ�ǰ Pattern �в��Ҹ� Layer
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
//    // ��ȡpattern����
//    QString patternName = patternPtr->getPatternName();
//
//    // ��� patternList û��ѡ�����ִ�и���
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
//    // ��¼��ǰѡ�е�ͼ������
//    int selectedIndex = layerList->currentRow();
//
//    // ������е�ͼ���б�
//    layerList->clear();
//
//    // ��ȡ����ͼ�㣬������UI
//    auto layers = patternPtr->getLayers();
//    for (const auto& layer : layers) {
//        if (layer) {
//            layerList->addItem(layer->getLayerName());
//        }
//    }
//
//    // ��� layerList Ϊ�գ���ѡ���κ���
//    if (layerList->count() == 0) {
//        return;
//    }
//
//    // ֻ���� patternList ��ѡ����ʱ����ִ��ѡ�� layerList ���߼�
//    if (selectedIndex >= 0 && selectedIndex < layerList->count()) {
//        layerList->setCurrentRow(selectedIndex);
//    }
//    else {
//        layerList->setCurrentRow(0);  // ѡ�е�һ��
//    }
//}
//
//void DisplayWidget::saveLayerInfo()
//{
//    // ��ȡlayerList��ǰѡ�е� Layer
//    int selectedRow = layerList->currentRow();
//    if (selectedRow < 0) {
//        return;
//    }
//
//    // ��ȡ��ǰѡ�е� Pattern
//    QString selectedPatternName = patternComboBox->currentText();
//    auto patternPtr = PatternManager::getPatternByName(selectedPatternName);
//    if (!patternPtr) return;
//
//    // ��ȡ���� Layer
//    auto layers = patternPtr->getLayers();
//    if (selectedRow >= layers.size()) return;
//
//    // ��ȡ��ǰѡ�е� Layer
//    auto oldLayerPtr = layers[selectedRow];
//    if (!oldLayerPtr) return;
//
//    // �� UI �ؼ���ȡ Layer ��Ϣ
//    QString layerName = layerNameEdit->text();
//    QString newLayerType = layerTypeComboBox->currentText();
//    bool layerStatus = layerStatusCheckBox->isChecked();
//    QString layerDescription = layerDescriptionEdit->toPlainText();
//
//    // ��� Layer �����Ƿ�Ϸ�
//    if (layerName.isEmpty()) {
//        QMessageBox::warning(this, "Warning", "Layer name cannot be empty!");
//        return;
//    }
//
//    // ��� Layer �����Ƿ��Ѵ��ڣ���ֹ������
//    for (const auto& existingLayer : layers) {
//        if (existingLayer->getLayerName() == layerName && existingLayer != oldLayerPtr) {
//            QMessageBox::warning(this, "Warning", "Layer name already exists!");
//            return;
//        }
//    }
//
//    // ȷ���޸�
//    if (QMessageBox::question(this, "Confirm Update", "Are you sure you want to update this layer?",
//        QMessageBox::Yes | QMessageBox::No) == QMessageBox::No) {
//        return;
//    }
//
//    // LayerType ���ʱ�������µ� Layer ���滻
//    if (oldLayerPtr->getLayerType() != newLayerType) {
//        std::shared_ptr<Layer> newLayerPtr = createNewLayer(newLayerType);
//        if (!newLayerPtr) {
//            QMessageBox::warning(this, "Warning", "Invalid Layer Type!");
//            return;
//        }
//
//        // Ǩ�ƹ�������
//        newLayerPtr->setLayerName(layerName);
//        newLayerPtr->setLayerType(newLayerType);
//        newLayerPtr->setLayerStatus(layerStatus);
//        newLayerPtr->setLayerDescription(layerDescription);
//
//        // �滻ԭ���� Layer
//        patternPtr->replaceLayer(selectedRow, newLayerPtr);
//    }
//    else {
//        // LayerType ���䣬ֱ���޸�����
//        oldLayerPtr->setLayerName(layerName);
//        oldLayerPtr->setLayerStatus(layerStatus);
//        oldLayerPtr->setLayerDescription(layerDescription);
//    }
//
//    updateLayerListView(patternPtr);
//
//    // �� layerList ������ѡ�е�ǰ�޸ĵ� Layer
//    for (int i = 0; i < layerList->count(); ++i) {
//        QListWidgetItem* item = layerList->item(i);
//        if (item->text() == layerName) {
//            layerList->setCurrentItem(item);
//            break;
//        }
//    }
//}
//
//// ���� Layer ʵ��
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
//    // ��ȡ��ǰѡ�е� Pattern
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
//    // ͨ��ģ�庯��������������Ի���
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
//    // ����ӳ�䣺LayerType -> ��Ӧ�� ConfigDialog
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
//// �������Pattern����ϸ��Ϣ (������)
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
//    // ��ȡ��ǰ���е� Pattern �� Layer ����
//    QJsonObject rootObject;
//
//    // ���� Pattern ������Ϣ
//    QJsonArray patternArray;
//    for (const auto& pattern : PatternManager::getAllPatterns()) {
//        QJsonObject patternObject;
//        patternObject["patternName"] = pattern->getPatternName();
//        patternObject["patternWidth"] = pattern->getPatternWidth();
//        patternObject["patternHeight"] = pattern->getPatternHeight();
//        patternObject["patternDescription"] = pattern->getPatternDescription();
//
//        // ����ͼ����Ϣ
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
//    // ������д���ļ�
//    QFile saveFile("config.json");
//    if (saveFile.open(QIODevice::WriteOnly)) {
//        QJsonDocument saveDoc(rootObject);
//        saveFile.write(saveDoc.toJson());
//        //qDebug() << "Saving configuration to file: " << saveDoc.toJson();  // ��ӡ����� JSON ����
//    }
//    else {
//        QMessageBox::critical(nullptr, "Save Failed", "Unable to open the file for saving. Please check permissions or disk space!");
//    }
//}
//
//// ��Layer����д��Json�ļ�����
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
//    // ���� gradientBars
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
//// ��Layer��������Json�ļ�����
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
//    // ���� JSON �ļ�
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
//        // ʹ�� addPattern ����һ���µ� Pattern���������ȡ������
//        auto patternPtr = PatternManager::loadPattern(patternName, patternWidth, patternHeight, patternDescription);
//
//        // ��ȡͼ����Ϣ
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
//            // ��������ͼ��
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
//                patternPtr->addLayer(std::shared_ptr<Layer>(layer)); // ��ͼ����ӵ� Pattern
//            }
//            updateLayerListView(patternPtr);
//        }
//    }
//    updatePatternListView();
//}
//
//// ��Layer��������Json�ļ�����
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
//    // ���� gradientBars
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
//    // �� gradientBars ת��Ϊ QVariant
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
////        QMessageBox::critical(nullptr, "����ʧ��", "�޷����ļ����б��棬����Ȩ�޻���̿ռ䣡");
////        return;
////    }
////
////    QDataStream out(&saveFile);
////    out.setVersion(QDataStream::Qt_6_5);  // ȷ�� Qt �汾һ��
////
////    // д�� Pattern ����
////    const auto& patterns = PatternManager::getAllPatterns();
////    out << patterns.size();
////    qDebug() << "Saving " << patterns.size() << " patterns";  // �������
////
////    for (const auto& pattern : patterns) {
////        qDebug() << "Saving pattern: " << pattern->getPatternName();
////        out << pattern->getPatternName();
////        out << pattern->getPatternWidth();
////        out << pattern->getPatternHeight();
////        out << pattern->getPatternDescription();
////
////        // д��ͼ����Ϣ
////        const auto& layers = pattern->getLayers();
////        out << layers.size();
////        qDebug() << "Saving 1111" << layers.size() << " layers for pattern" << pattern->getPatternName();  // �������
////
////        for (const auto& layer : layers) {
////            out << layer->getLayerName();
////            out << layer->getLayerType();
////            out << layer->getLayerStatus();
////            out << layer->getLayerDescription();
////
////            // ����ͬ���͵�ͼ��
////            if (layer->getLayerType() == "Mono") {
////                processMonoLayerSave(out, layer.get());
////            }
////            else if (layer->getLayerType() == "Gradient") {
////                processGradientLayerSave(out, layer.get());
////            }
////        }
////    }
////
////    saveFile.flush();  // ȷ����������д��
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
////    qDebug() << "Saving MonoLayer: " << params["width"].toInt() << params["height"].toInt();  // �������
////
////    out << params["width"].toInt();
////    out << params["height"].toInt();
////    out << params["alpha"].toInt();
////    out << params["borderWidth"].toInt();
////    out << params["borderHeight"].toInt();
////    out << params["borderFillColor"].toBool();
////    out << params["backgroundColor"].value<QColor>().rgba();  // ʹ����������ɫ
////    out << params["borderColor"].value<QColor>().rgba();
////}
////
////void DisplayWidget::processGradientLayerSave(QDataStream& out, Layer* layer) {
////    GradientLayer* gradientLayer = dynamic_cast<GradientLayer*>(layer);
////    if (!gradientLayer) return;
////
////    QMap<QString, QVariant> params = gradientLayer->getParameters();
////    qDebug() << "Saving GradientLayer: " << params["width"].toInt() << params["height"].toInt();  // �������
////
////    out << params["width"].toInt();
////    out << params["height"].toInt();
////    out << params["alpha"].toInt();
////    out << params["counts"].toInt();
////    out << params["orientation"].toString();
////
////    // ��������
////    QList<QMap<QString, QVariant>> gradientBars = params["gradientBars"].value<QList<QMap<QString, QVariant>>>();
////    out << gradientBars.size();  // �ȴ�����
////
////    qDebug() << "Saving gradientBars: " << gradientBars.size();  // �������
////    for (const auto& gradientBar : gradientBars) {
////        out << gradientBar["steps"].toInt();
////        out << gradientBar["startColor"].value<QColor>().rgba();
////        out << gradientBar["endColor"].value<QColor>().rgba();
////    }
////}
////
//
