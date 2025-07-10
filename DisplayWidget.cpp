#include "BitMapLayerConfigDialog.h"
#include "CCircleLayerConfigDialog.h"
#include "CheckerLayerConfigDialog.h"
#include "CRectLayerConfigDialog.h"
#include "DisplayWidget.h"
#include "GradientLayerConfigDialog.h"
#include "Layer.h"
#include "LineLayerConfigDialog.h"
#include "MonoLayerConfigDialog.h"
#include "Pattern.h"
#include "PatternDrawDialog.h"
#include "PatternManager.h"
#include "PictureLayerConfigDialog.h"
#include "TextLayerConfigDialog.h"
#include <memory>
#include <QBuffer>
#include <QFont>
#include <QFormLayout>
#include <QGroupBox>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QPixmap>
#include <QPushButton>
#include <QScreen>
#include <QSpinBox>
#include <QSplitter>
#include <QTabWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QDir>
#include <QMenu>
#include <QTimer>

#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

DisplayWidget::DisplayWidget(QWidget* parent)
    : QWidget(parent)
{
    setupUI();

    qApp->installEventFilter(this);
}

DisplayWidget::~DisplayWidget()
{
}

bool DisplayWidget::eventFilter(QObject* obj, QEvent* event) {
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

        if (keyEvent->key() == Qt::Key_Up || keyEvent->key() == Qt::Key_Down) {
            if (!patternList) return false;

            int currentRow = patternList->currentRow();
            int total = patternList->count();

            if (total == 0) return false; // 没有条目
            int newRow = currentRow;

            if (keyEvent->key() == Qt::Key_Up) {
                newRow = (currentRow - 1 + total) % total;
            }
            else if (keyEvent->key() == Qt::Key_Down) {
                newRow = (currentRow + 1) % total;
            }

            patternList->setCurrentRow(newRow);  // 更新列表项

            // 如果预览窗口打开，就更新图案，没有预览窗口，只更新patternList选项
            if (privewDialog && privewDialog->isVisible()) {
                updatePreviewPattern();  // 更新图案
            }

            return true;  // 表示按键已处理
        }
    }
    return QWidget::eventFilter(obj, event);
}

////////////////////////////////////////////////////////////////////-------------UI-------------/////////////////////////////////////////////////////////////////////

void DisplayWidget::setupUI()
{
    this->setFocusPolicy(Qt::StrongFocus);
    this->setFocus();

    QScreen* screen = QGuiApplication::primaryScreen();
    QRect geometry = screen->geometry();
    qreal pixelRatio = screen->devicePixelRatio();

    int width = geometry.width() * pixelRatio;
    int height = geometry.height() * pixelRatio;

    // 创建 QSplitter
    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);

    // 左侧（图案列表 + 按钮）
    QWidget* leftWidget = new QWidget(this);
    leftWidget->setMaximumWidth(200);
    QVBoxLayout* leftLayout = new QVBoxLayout(leftWidget);
    leftLayout->setContentsMargins(0, 14, 0, 0);

    patternInfoBox = new QGroupBox(tr("Pattern Info"));
    patternFormLayout = new QFormLayout();
    patternFormLayout->setSpacing(20);
    patternNameEdit = new QLineEdit();
    patternWidthSpinBox = new QSpinBox();
    patternHeightSpinBox = new QSpinBox();
    patternWidthSpinBox->setRange(1, 100000);
    patternHeightSpinBox->setRange(1, 100000);
    patternWidthSpinBox->setValue(width);
    patternHeightSpinBox->setValue(height);
    patternDescriptionEdit = new QTextEdit();

    patternNameEdit->setFixedWidth(80);
    patternWidthSpinBox->setFixedWidth(80);
    patternHeightSpinBox->setFixedWidth(80);
    
    widthLabel = new QLabel(tr("Width\t:"));
    heightLabel = new QLabel(tr("Height\t:"));
    patternFormLayout->addRow(widthLabel, patternWidthSpinBox);
    patternFormLayout->addRow(heightLabel, patternHeightSpinBox);
    //patternFormLayout->addRow("Description:", patternDescriptionEdit);
    patternInfoBox->setLayout(patternFormLayout);

    // Pattern列表组件
    patternListBox = new QGroupBox(tr("Pattern List"));
    QVBoxLayout* patternListLayout = new QVBoxLayout(patternListBox);

    patternList = new QListWidget(this);
    patternList->setFocusPolicy(Qt::StrongFocus);
    patternList->setSelectionMode(QAbstractItemView::ExtendedSelection); // 多选模式
    patternList->setDragEnabled(true);
    patternList->setAcceptDrops(true);
    patternList->setDropIndicatorShown(true);
    patternList->setDragDropMode(QAbstractItemView::InternalMove);
    patternList->setContextMenuPolicy(Qt::CustomContextMenu); // 上下文菜单策略
    patternList->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed | QAbstractItemView::SelectedClicked);

    // Pattern列表按钮
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    addButton = new QPushButton(tr("Add"), this);
    delButton = new QPushButton(tr("Remove"), this);
    addButton->setMinimumWidth(46);
    delButton->setMinimumWidth(46);
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(delButton);

    patternListLayout->addWidget(patternList);
    patternListLayout->addLayout(buttonLayout);
    patternListBox->setLayout(patternListLayout);

    leftLayout->addWidget(patternInfoBox);
    leftLayout->addWidget(patternListBox);
    leftWidget->setLayout(leftLayout);

    // 右侧（Tab 选项卡）
    tabWidget = new QTabWidget(this);
    infoTab = creatInformationTab();
    tabWidget->addTab(infoTab, tr("Information"));
    tabWidget->setStyleSheet("QTabWidget::pane { border: 1px solid lightgray; }");

    // 添加到 QSplitter
    splitter->addWidget(leftWidget);
    splitter->setStyleSheet("QSplitter::handle { background-color: transparent; }");
    splitter->addWidget(tabWidget);
    splitter->setSizes(QList<int>() << 180 << 200);
    leftWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    tabWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);
    tabWidget->setMinimumWidth(1);

    // 设置主布局
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(splitter);
    setLayout(mainLayout);

    this->setFocusPolicy(Qt::StrongFocus);
    this->setFocusProxy(patternList);

    // 连接信号槽
    //connect(patternNameEdit, &QLineEdit::editingFinished, this, &DisplayWidget::savePatternInformation);
    connect(patternWidthSpinBox, &QSpinBox::editingFinished, this, &DisplayWidget::savePatternInformation);
    connect(patternHeightSpinBox, &QSpinBox::editingFinished, this, &DisplayWidget::savePatternInformation);
    connect(addButton, &QPushButton::clicked, this, &DisplayWidget::addPattern);
    connect(delButton, &QPushButton::clicked, this, &DisplayWidget::removePattern);
    connect(patternList, &QListWidget::itemSelectionChanged, this, &DisplayWidget::onPatternSelected);
    connect(patternList->model(), &QAbstractItemModel::rowsMoved, this, &DisplayWidget::onPatternListItemChanged); // 切换索引
    connect(patternList, &QListWidget::customContextMenuRequested, this, &DisplayWidget::showPatternListContextMenu); // 右键菜单
    connect(patternList, &QListWidget::itemChanged, this, &DisplayWidget::onPatternNameChanged); //编辑模式修改patternName
    //connect(patternList, &QListWidget::currentRowChanged, this, [](int row) {
    //    /*qDebug() << "currentRowChanged to row:" << row;*/
    //    });
}

QWidget* DisplayWidget::creatInformationTab()
{
    QScreen* screen = QGuiApplication::primaryScreen();
    QRect geometry = screen->geometry();
    qreal pixelRatio = screen->devicePixelRatio();

    int width = geometry.width() * pixelRatio;
    int height = geometry.height() * pixelRatio;

    QWidget* tab = new QWidget(this);
    QHBoxLayout* mainLayout = new QHBoxLayout(tab);

    // ==================== 左侧布局 ====================
    QWidget* leftWidget = new QWidget(tab);
    leftWidget->setMaximumWidth(190);
    QVBoxLayout* leftLayout = new QVBoxLayout(leftWidget);

    // 图层列表 + 按钮（下部）
    layerListBox = new QGroupBox(tr("Layer List"));
    QVBoxLayout* layerListLayout = new QVBoxLayout(layerListBox);

    layerList = new QListWidget();
    layerList->setSelectionMode(QAbstractItemView::ExtendedSelection);
    layerList->setDragEnabled(true);
    layerList->setAcceptDrops(true);
    layerList->setDropIndicatorShown(true);
    layerList->setDragDropMode(QAbstractItemView::InternalMove);
    layerList->setContextMenuPolicy(Qt::CustomContextMenu); // 上下文菜单策略

    layerListLayout->addWidget(layerList);

    QHBoxLayout* layerButtonLayout = new QHBoxLayout();
    addLayerButton = new QPushButton(tr("Add"));
    editButton = new QPushButton(tr("Edit"));
    delLayerButton = new QPushButton(tr("Remove"));
    addLayerButton->setMinimumWidth(46);
    editButton->setMinimumWidth(46);
    delLayerButton->setMinimumWidth(46);
    layerButtonLayout->addWidget(addLayerButton);
    layerButtonLayout->addWidget(editButton);
    layerButtonLayout->addWidget(delLayerButton);

    layerListLayout->addLayout(layerButtonLayout);
    layerListBox->setLayout(layerListLayout);

    QVBoxLayout* layerControlLayout = new QVBoxLayout();
    layerControlLayout->addWidget(layerListBox);

    leftLayout->addLayout(layerControlLayout);
    leftWidget->setLayout(leftLayout);
    // ==================== 右侧布局 ====================
    QWidget* rightWidget = new QWidget(tab);
    QVBoxLayout* rightLayout = new QVBoxLayout(rightWidget);

    // Pattern 缩略图区域
    patternPreviewBox = new QGroupBox(tr("Pattern Preview"));
    QVBoxLayout* patternPreviewLayout = new QVBoxLayout(patternPreviewBox);

    QHBoxLayout* previewCenterLayout = new QHBoxLayout();
    patternPreview = new QLabel("");
    patternPreview->setAlignment(Qt::AlignCenter);
    previewCenterLayout->addStretch();
    previewCenterLayout->addWidget(patternPreview);
    previewCenterLayout->addStretch();

    patternPreviewLayout->addStretch();
    patternPreviewLayout->addLayout(previewCenterLayout);
    patternPreviewLayout->addStretch();

    // 操作按钮区（底部）
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    saveButton = new QPushButton(tr("Save Image"));
    previewButton = new QPushButton(tr("Preview"));
    previewButton->setMinimumWidth(60);
    saveButton->setMinimumWidth(60);
    buttonLayout->addStretch();
    buttonLayout->addWidget(previewButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(saveButton);
    buttonLayout->addStretch();
    patternPreviewLayout->addLayout(buttonLayout);

    patternPreviewBox->setLayout(patternPreviewLayout);

    rightLayout->addWidget(patternPreviewBox);
    rightLayout->setStretch(0, 1); // patternPreviewBox 撑满右边

    // ==================== 主布局整合 ====================
    mainLayout->addWidget(leftWidget);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(rightWidget);
    mainLayout->setStretch(0, 1); // 左侧占比
    mainLayout->setStretch(1, 1); // 右侧占比

    tab->setLayout(mainLayout);
    tabWidget->addTab(tab, tr("Layer Info"));

    // ==================== 信号连接 ====================
    connect(addLayerButton, &QPushButton::clicked, this, &DisplayWidget::addLayer);
    connect(delLayerButton, &QPushButton::clicked, this, &DisplayWidget::removeLayer);
    connect(editButton, &QPushButton::clicked, this, &DisplayWidget::editLayer);
    connect(previewButton, &QPushButton::clicked, this, &DisplayWidget::onPatternSelectedDraw);
    connect(saveButton, &QPushButton::clicked, this, &DisplayWidget::savePatterAsImage);
    connect(layerList->model(), &QAbstractItemModel::rowsMoved, this, &DisplayWidget::onLayerListItemChanged);
    connect(layerList, &QListWidget::customContextMenuRequested, this, &DisplayWidget::showLayerListContextMenu);

    return tab;
}

void DisplayWidget::updateConfig()
{
    // 当配置内容更改时
    // 发射信号通知 MainWindow 更新标志
    emit configChanged(true);  // 配置已更改

}

void DisplayWidget::retranslateUi()
{
    if (widthLabel) widthLabel->setText(tr("Width\t:"));
    if (heightLabel) heightLabel->setText(tr("Height\t:"));

    if (patternInfoBox) patternInfoBox->setTitle(tr("Pattern Info"));
    if (patternListBox) patternListBox->setTitle(tr("Pattern List"));
    if (addButton) addButton->setText(tr("Add"));
    if (delButton) delButton->setText(tr("Remove"));

    // 右侧部分
    if (layerListBox) layerListBox->setTitle(tr("Layer List"));
    if (addLayerButton) addLayerButton->setText(tr("Add"));
    if (delLayerButton) delLayerButton->setText(tr("Remove"));

    if (patternPreviewBox) patternPreviewBox->setTitle(tr("Pattern Preview"));
    if (editButton) editButton->setText(tr("Edit"));
    if (saveButton) saveButton->setText(tr("Save Image"));
    if (previewButton) previewButton->setText(tr("Preview"));

    if (tabWidget && infoTab) {
        int infoIndex = tabWidget->indexOf(infoTab);
        if (infoIndex != -1) {
            tabWidget->setTabText(infoIndex, tr("Information"));
        }
    }
}

////////////////////////////////////////////////////////////////////----------Pattern---------- /////////////////////////////////////////////////////////////////////

void DisplayWidget::addPattern() {

    // 1. 确保 patternList 已初始化
    if (!patternList) { return; }

    // 2. 创建新 Pattern，检查是否返回 nullptr
    std::shared_ptr<Pattern> newPattern = PatternManager::addPattern();
    if (!newPattern) { return; }

    QString newPatternName = newPattern->getPatternName();

    // 临时阻断信号，不触发 itemChanged，因为addItem会触发itemChanged，导致调用槽函数
    bool oldState = patternList->blockSignals(true);

    // 3. 添加新Pattern到 patternList
    auto* item = new QListWidgetItem(QStringLiteral("%1").arg(newPatternName), patternList);
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    item->setData(Qt::UserRole, newPatternName);
    patternList->addItem(item);

    patternList->blockSignals(oldState); // 恢复信号

    // 4. 选中新建项（多选模式需要先清理选中项）
    patternList->clearSelection();
    patternList->setCurrentItem(item);

    updateConfig();
}

void DisplayWidget::removePattern() {
    QList<QListWidgetItem*> selectedItems = patternList->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, tr("Delete failed"), tr("Please select the patterns to delete!"));
        return;
    }

    // 确认删除
    QString part1 = tr("Are you sure you want to delete the selected ");
    QString part2 = tr(" patterns?");
    QString message = part1 + QString("%1").arg(selectedItems.size()) + part2;

    if (QMessageBox::question(this, tr("Delete Confirmation"),
        message,
        QMessageBox::Yes | QMessageBox::No) == QMessageBox::No) {
        return;
    }

    for (auto* item : selectedItems) {
        PatternManager::removePatternByName(item->text());
        delete item;
    }

    updatePatternListView();

    int totalCount = patternList->count();
    if (totalCount == 0) {
        layerList->clear();
        return;
    }

    auto patternPtr = PatternManager::getPatternByName(patternList->currentItem()->text());
    updateLayerListView(patternPtr);

    updateConfig();
}

void DisplayWidget::clearAllPatterns() {
    PatternManager::clearAllPatterns();  // 调用静态方法清空所有图案

    // 清空列表项
    if (patternList) {
        patternList->clear();
    }

    if (layerList) {
        layerList->clear();
    }

    update();  // 更新界面，重绘
}

void DisplayWidget::onPatternSelected()
{
    // 获取 patternList 中当前选择项
    QListWidgetItem* selectedItem = patternList->currentItem();

    if (!selectedItem && patternList->count() > 0) {
        patternList->setCurrentRow(0);
        selectedItem = patternList->currentItem();
    }

    if (!selectedItem) return;

    // 通过名称从 PatternManager 获取 Pattern
    QString patternName = selectedItem->text();
    auto patternPtr = PatternManager::getPatternByName(patternName);

    if (!patternPtr) return;

    // 更新 UI 信息
    patternNameEdit->setText(patternPtr->getPatternName());
    patternWidthSpinBox->setValue(patternPtr->getPatternWidth());
    patternHeightSpinBox->setValue(patternPtr->getPatternHeight());
    patternDescriptionEdit->setText(patternPtr->getPatternDescription());

    updateLayerListView(patternPtr);
    onPatternSelectPixmapDraw();
}

void DisplayWidget::updatePatternListView()
{
    // 记录当前选中的索引
    int currentIndex = patternList->currentRow();
    patternList->clear();

    // 遍历 PatternManager 中的所有 Pattern 添加到 patternList
   const QVector<std::shared_ptr<Pattern>>& patterns = PatternManager::getAllPatterns();
    for (const auto& pattern : patterns) {
        QListWidgetItem* item = new QListWidgetItem(pattern->getPatternName());
        item->setFlags(item->flags() | Qt::ItemIsEditable); // 这里确保载入配置文件的项也可以触发itemChange
        item->setData(Qt::UserRole, pattern->getPatternName()); // 设置初始的patternName
        patternList->addItem(item);
    }

    // 恢复选中项
    if (currentIndex >= 0 && currentIndex < patternList->count()) {
        patternList->setCurrentRow(currentIndex);
    }
    else if (patternList->count() > 0) {
        patternList->setCurrentRow(0);
    }
}

void DisplayWidget::savePatternInformation()
{
    // 确认对话框
    //if (QMessageBox::question(this, "Save Confirmation", "Are you sure you want to save the current pattern information?",
    //    QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) {
    //    return;
    //}

    // Lambda 处理警告消息
    auto showWarning = [this](const QString& message) {
        QMessageBox::warning(this, tr("Warning"), message);
        };

    // 获取输入框数据
    QString name = patternNameEdit->text().trimmed();
    int width = patternWidthSpinBox->value();
    int height = patternHeightSpinBox->value();
    //QString description = patternDescriptionEdit->toPlainText();

    // 检查选中的 Pattern
    QListWidgetItem* selectedItem = patternList->currentItem();
    if (!selectedItem) {
        //showWarning("No pattern selected1!");
        return;
    }

    // 查找选中的 Pattern
    auto selectedPattern = PatternManager::getPatternByName(selectedItem->text());
    if (!selectedPattern) {
        showWarning(tr("Invalid pattern object!"));
        return;
    }

    if (name.isEmpty()) {
        showWarning(tr("Pattern name cannot be empty!"));
        return;
    }

    // 检查名称是否已存在
    for (const auto& existingPattern : PatternManager::getAllPatterns()) {
        if (existingPattern != selectedPattern && existingPattern->getPatternName() == name) {
            showWarning(tr("A pattern with this name already exists!"));
            return;
        }
    }

    // 更新 Pattern 信息
    selectedPattern->setPatternName(name);
    selectedPattern->setPatternWidth(width);
    selectedPattern->setPatternHeight(height);
    //selectedPattern->setPatternDescription(description);

    auto layers = selectedPattern->getLayers();
    if (!layers.isEmpty()) {
        // 只更新当前选中的 Layer 信息
        int selectedLayerIndex = layerList->currentRow();
        auto layers = selectedPattern->getLayers();
        if (selectedLayerIndex >= 0 && selectedLayerIndex < layers.size()) {
            auto selectedLayer = layers[selectedLayerIndex];

            // 从列表中获取对应的 item widget（包含复选框）
            QListWidgetItem* item = layerList->item(selectedLayerIndex);
            QWidget* itemWidget = layerList->itemWidget(item);
            if (itemWidget) {
                QCheckBox* checkBox = itemWidget->findChild<QCheckBox*>();
                if (checkBox) {
                    bool layerStatus = checkBox->isChecked();
                    selectedLayer->setLayerStatus(layerStatus);

                    onPatternSelectPixmapDraw();
                }
            }
        }
        else {
            showWarning(tr("No layer selected to save!"));
        }
    }

    // 更新 UI 显示
    selectedItem->setText(name);

    onPatternSelectPixmapDraw();
}

//void DisplayWidget::cancelPatternInformation()
//{
//    //if (QMessageBox::question(this, "Cancel Editing", "Are you sure you want to discard changes?",
//    //    QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) {
//    //    return;
//    //}
//
//    onPatternSelected();
//}

void DisplayWidget::onPatternSelectedDraw()
{
    // Lambda 处理警告消息
    auto showWarning = [this](const QString& message) {
        QMessageBox::warning(this, tr("Warning"), message);
        };

    // 获取当前选中的 Pattern 名称
    QListWidgetItem* selectedItem = patternList->currentItem();
    if (!selectedItem) {
        showWarning(tr("No pattern selected!"));
        return;
    }

    // 通过 PatternManager 查找 Pattern
    std::shared_ptr<Pattern> selectedPattern = PatternManager::getPatternByName(selectedItem->text());
    if (!selectedPattern) {
        //showWarning("Invalid pattern selection!");
        return;
    }

    // 检查是否有 Layer   ——不再检查，直接绘制纯黑背景
    //if (selectedPattern->getLayers().isEmpty()) {
    //    showWarning(tr("The selected pattern has no Layers!\n" "Please continue after adding the layer."));
    //    return;
    //}

    // 创建预览窗口并显示——只打开一次预览窗口
    privewDialog = new PatternDrawDialog(selectedPattern, this);
    privewDialog->show();  // 非阻塞，允许继续切换

    // 关闭后清空指针，避免悬挂引用
    connect(privewDialog, &QDialog::finished, this, [this]() {
        privewDialog = nullptr;
        });
}

void DisplayWidget::updatePreviewPattern()
{
    if (!privewDialog) return;

    QListWidgetItem* selectedItem = patternList->currentItem();
    if (!selectedItem) return;

    std::shared_ptr<Pattern> selectedPattern = PatternManager::getPatternByName(selectedItem->text());
    if (!selectedPattern) return;

    privewDialog->updatePattern(selectedPattern);
}

void DisplayWidget::onPatternSelectPixmapDraw() {
    auto showWarning = [this](const QString& message) {
        QMessageBox::warning(this, tr("Warning"), message);
        };

    QListWidgetItem* selectedItem = patternList->currentItem();
    if (!selectedItem) {
        return;
    }

    std::shared_ptr<Pattern> selectedPattern = PatternManager::getPatternByName(selectedItem->text());
    if (!selectedPattern) {
        //showWarning("Invalid pattern selection!");
        return;
    }

    // 目标区域大小
    int patternWidth = selectedPattern->getWidth();
    int patternHeight = selectedPattern->getHeight();
    if (patternWidth <= 0 || patternHeight <= 0) {
        showWarning(tr("Pattern size is invalid!"));
        return;
    }

    // 缩略图大小
    QSize previewSize(320, 230);

    QImage patternImage(patternWidth, patternHeight, QImage::Format_ARGB32_Premultiplied);
    patternImage.fill(QColor(0, 0, 0, 255));

    QPainter painter(&patternImage);
    painter.setRenderHint(QPainter::Antialiasing, false);
    selectedPattern->drawPattern(painter, true);
    painter.end();

    // 2. 缩放为缩略图大小
    QPixmap pixmap = QPixmap::fromImage(patternImage).scaled(previewSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // 3. 显示在 QLabel 上
    patternPreview->setPixmap(pixmap);
    patternPreview->setFixedSize(pixmap.size());
}

void DisplayWidget::savePatterAsImage() {
    // 获取当前选中的 Pattern
    QListWidgetItem* selectedItem = patternList->currentItem();
    if (!selectedItem) {
        QMessageBox::warning(this, tr("Warning"), tr("No pattern selected!"));
        return;
    }

    std::shared_ptr<Pattern> selectedPattern = PatternManager::getPatternByName(selectedItem->text());
    if (!selectedPattern) {
        QMessageBox::warning(this, tr("Warning"), tr("Invalid pattern selection!"));
        return;
    }

    // 检查是否有图层
    if (selectedPattern->getLayers().isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("Pattern has no layers!"));
        return;
    }

    // 创建 QImage 作为绘图缓冲区
    QImage image(selectedPattern->getWidth(), selectedPattern->getHeight(), QImage::Format_ARGB32);
    image.fill(Qt::transparent); // 或 Qt::white

    QPainter painter(&image);
    selectedPattern->drawPattern(painter); // 假设你在 Pattern 中实现了 draw(QPainter*) 方法
    painter.end();

    // 让用户选择保存路径
    QString filePath = QFileDialog::getSaveFileName(
        this,
        tr("Save Pattern as Image"),
        selectedItem->text() + ".bmp",
        "BMP Files (*.bmp);;PNG Files (*.png);;JPG Files (*.jpg)"
    );

    if (filePath.isEmpty()) return;

    // 保存图片
    if (!image.save(filePath)) {
        QMessageBox::critical(this, tr("Save Failed"), tr("Could not save image."));
    }
    else {
        QMessageBox::information(this, tr("Success"), tr("Pattern image saved successfully."));
    }
}

void DisplayWidget::onPatternListItemChanged() {
    // 获取所有 pattern 名称的新顺序
    QStringList newOrder;
    for (int i = 0; i < patternList->count(); ++i) {
        QListWidgetItem* item = patternList->item(i);
        newOrder.append(item->text());
    }

    // 更新patternList索引
    PatternManager::updatePatternOrder(newOrder);

    updatePatternListView();

    updateConfig();
}

void DisplayWidget::showPatternListContextMenu(const QPoint& pos) {
    QListWidgetItem* hoveredItem = patternList->itemAt(pos);
    bool hasSelection = !patternList->selectedItems().isEmpty();

    // 创建菜单
    QMenu contextMenu(this);

    // 添加菜单项
    QAction* addAction = contextMenu.addAction(tr("Add"));
    QAction* removeAction = contextMenu.addAction(tr("Remove"));
    QAction* renameAction = contextMenu.addAction(tr("Rename"));
    contextMenu.addSeparator();
    QAction* moveUpAction = contextMenu.addAction(tr("Move Up"));
    QAction* moveDownAction = contextMenu.addAction(tr("Move Down"));

    // 执行菜单并获取用户选择
    QAction* selectedAction = contextMenu.exec(patternList->viewport()->mapToGlobal(pos));

    // 处理用户选择
    if (selectedAction == addAction) {
        addPattern();
    }
    else if (selectedAction == removeAction) {
        removePattern();
    }
    else if (selectedAction == renameAction) {
        // 进入编辑模式，直接在列表中编辑
        QListWidgetItem* selectedItem = patternList->currentItem();
        if (selectedItem) {
            selectedItem->setData(Qt::UserRole, selectedItem->text());
            patternList->editItem(selectedItem);  // 启动编辑模式
        }
        /*renamePattern();*/
    }
    else if (selectedAction == moveUpAction) {
        movePatternUp();
    }
    else if (selectedAction == moveDownAction) {
        movePatternDown();
    }
}

// 弹窗重命名
void DisplayWidget::renamePattern() {
    // 获取选中的项
    QListWidgetItem* selectedItem = patternList->currentItem();
    if (!selectedItem) return;

    QString oldName = selectedItem->text();  // 旧名称
    bool ok;
    // 弹出输入框让用户输入新的名称
    QString newName = QInputDialog::getText(this, tr("Rename Pattern"), tr("Enter new pattern name:"), QLineEdit::Normal, oldName, &ok);

    // 如果用户输入了有效的名字
    if (ok && !newName.isEmpty() && newName != oldName) {
        // 调用 PatternManager 的重命名函数
        bool result = PatternManager::renamePatternName(oldName, newName);

        if (result) {
            // 更新界面上的显示
            selectedItem->setText(newName);
            // 可选：如果需要，可以重新排序或者做其他更新
            patternNameEdit->setText(newName);
        }
        else {
            QMessageBox::warning(this, tr("Rename Failed"), tr("Pattern name already exists or failed to rename."));
        }
    }

    updateConfig();
}

// 编辑模式重命名
void DisplayWidget::onPatternNameChanged(QListWidgetItem* item) {
    if (!item) return;

    static bool renaming = false;
    if (renaming || !item) return;
    renaming = true;

    QString oldName = item->data(Qt::UserRole).toString();
    QString newName = item->text().trimmed();

    if (oldName == newName) {
        renaming = false;
        return;
    }

    if (newName.isEmpty()) {
        item->setText(oldName);
        QMessageBox::warning(this, tr("Rename Failed"), tr("Pattern name cannot be empty."));
        item->setText(oldName);
        renaming = false;
        return;
    }

    // 尝试重命名
    bool ok = PatternManager::renamePatternName(oldName, newName);
    if (!ok) {
        // 失败时恢复旧名字
        QMessageBox::warning(this, tr("Rename Failed"), tr("Pattern name already exists or failed to rename."));
        item->setText(oldName);
    }
    else {
        // 成功时更新旧名字缓存
        item->setData(Qt::UserRole, newName);
        // 更新patternNameEdit 控件
        if (patternNameEdit) {
            patternNameEdit->setText(newName);
        }
    }

    renaming = false;

    updateConfig();
}

void DisplayWidget::movePatternUp() {
    int currentRow = patternList->currentRow();
    if (currentRow == -1 || currentRow == 0) return;

    QListWidgetItem* currentItem = patternList->takeItem(currentRow);
    patternList->insertItem(currentRow - 1, currentItem);
    patternList->setCurrentRow(currentRow - 1);

    onPatternListItemChanged();

    updateConfig();
}

void DisplayWidget::movePatternDown() {
    int currentRow = patternList->currentRow();
    if (currentRow == -1 || currentRow >= patternList->count() - 1) return;

    QListWidgetItem* currentItem = patternList->takeItem(currentRow);
    patternList->insertItem(currentRow + 1, currentItem);
    patternList->setCurrentRow(currentRow + 1);

    onPatternListItemChanged();

    updateConfig();
}

////////////////////////////////////////////////////////////////////----------Layer---------- /////////////////////////////////////////////////////////////////////

void DisplayWidget::addLayer()
{
    // Lambda 处理警告消息
    auto showWarning = [this](const QString& message) {
        QMessageBox::warning(this, tr("Warning"), message);
        };

    // 获取当前 Pattern
    QListWidgetItem* selectedItem = patternList->currentItem();

    if (!selectedItem && patternList->count() > 0) {
        patternList->setCurrentRow(0);
        selectedItem = patternList->currentItem();
    }

    if (!selectedItem) {
        showWarning(tr("No pattern selected!"));
        return;
    }

    QString patternName = selectedItem->text();
    auto patternPtr = PatternManager::getPatternByName(patternName);
    if (!patternPtr) {
        showWarning(tr("Pattern not found!"));
        return;
    }

    // 弹出对话框让用户选择 Layer 类型
    QStringList layerTypes = { "Mono", "Gradient", "Checker", "BitMap", "CRect", "CCircle", "Line", "Text", "Picture" };
    bool ok;
    QString selectedLayerType = QInputDialog::getItem(this, tr("Select Layer Type"), tr("Layer Type:"), layerTypes, 0, false, &ok);

    if (!ok || selectedLayerType.isEmpty()) {
        return;  // 用户取消选择
    }

    // 使用工厂创建 Layer
    std::shared_ptr<Layer> newLayer = PatternManager::createLayerByType(selectedLayerType);
    if (!newLayer) {
        showWarning(tr("Failed to create layer of type: ") + selectedLayerType);
        return;
    }

    // 设置 Layer 属性
    newLayer->setLayerType(selectedLayerType);
    newLayer->setLayerStatus(true);

    // 暂时单独存起来，不马上加到 Pattern
    pendingNewLayer = newLayer;
    pendingLayerType = selectedLayerType;
    editLayer();

    if (pendingNewLayer) {
        // OK了，真的添加进去
        patternPtr->addLayer(pendingNewLayer);

        auto layerItem = new QListWidgetItem(pendingLayerType);
        layerItem->setText(selectedLayerType);
        layerItem->setSelected(true);
        layerList->addItem(layerItem);
        layerList->clearSelection();
        layerList->setCurrentItem(layerItem);
        layerList->blockSignals(false);

        updateLayerListView(patternPtr);
        updateConfig();
    }
}

//void DisplayWidget::editLayer()
//{
//    //PatternManager patternManager;
//    //patternManager.printPatternInfo();
//
//    // 获取当前选中的 Pattern
//    auto patternPtr = PatternManager::getPatternByName(patternList->currentItem()->text());
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
//    };
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
//        {"Text",     [&]() { openDialog(static_cast<TextLayer*>(nullptr), static_cast<TextLayerConfigDialog*>(nullptr)); }},
//        { "Picture",     [&]() { openDialog(static_cast<PictureLayer*>(nullptr), static_cast<PictureLayerConfigDialog*>(nullptr)); } }
//    };
//
//    if (dialogMap.contains(layerType)) {
//        dialogMap[layerType]();
//    }
//
//    onPatternSelectPixmapDraw();
//}

// 通用模板工具函数，适用于创建Layer编辑界面
template<typename LayerT, typename DialogT>
bool  openDialog(std::shared_ptr<Layer>& layerPtr) {
    DialogT dialog(static_cast<LayerT*>(layerPtr.get()));
    if (dialog.exec() == QDialog::Accepted) {
        layerPtr->setParameters(dialog.getLayerParameters());
        return true;
    }
    return false;
}

void DisplayWidget::editLayer()
{
    std::shared_ptr<Layer> layerPtr;

    if (pendingNewLayer) {
        layerPtr = pendingNewLayer;
    }
    else {
        int selectedRow = layerList->currentRow();
        if (selectedRow < 0) return;

        QListWidgetItem* selectedPatternItem = patternList->currentItem();
        if (!selectedPatternItem) return;

        auto patternPtr = PatternManager::getPatternByName(selectedPatternItem->text());
        if (!patternPtr) return;

        auto layerListVec = patternPtr->getLayers();
        if (selectedRow >= layerListVec.size()) return;

        layerPtr = layerListVec[selectedRow];
    }

    if (!layerPtr) return;

    QString layerType = layerPtr->getLayerType();

    static const QMap<QString, std::function<bool(std::shared_ptr<Layer>&)>> dialogMap{
    {"Mono",     [](auto& l) { return openDialog<MonoLayer, MonoLayerConfigDialog>(l); }},
    {"Gradient", [](auto& l) { return openDialog<GradientLayer, GradientLayerConfigDialog>(l); }},
    {"Checker",  [](auto& l) { return openDialog<CheckerLayer, CheckerLayerConfigDialog>(l); }},
    {"BitMap",   [](auto& l) { return openDialog<BitMapLayer, BitMapLayerConfigDialog>(l); }},
    {"CRect",    [](auto& l) { return openDialog<CRectLayer, CRectLayerConfigDialog>(l); }},
    {"CCircle",  [](auto& l) { return openDialog<CCircleLayer, CCircleLayerConfigDialog>(l); }},
    {"Line",     [](auto& l) { return openDialog<LineLayer, LineLayerConfigDialog>(l); }},
    {"Text",     [](auto& l) { return openDialog<TextLayer, TextLayerConfigDialog>(l); }},
    {"Picture",  [](auto& l) { return openDialog<PictureLayer, PictureLayerConfigDialog>(l); }},
    };

    bool accepted = false;
    if (dialogMap.contains(layerType)) {
        accepted = dialogMap[layerType](layerPtr);
    }

    // 如果是新建的Layer，用户取消了编辑，就丢弃
    if (pendingNewLayer && !accepted) {
        pendingNewLayer.reset();  // 丢弃
        pendingLayerType.clear();
        return;
    }

    // 如果是已有的Layer，编辑完成后，刷新界面
 /*   if (!pendingNewLayer) {
        onPatternSelectPixmapDraw();
        updateConfig();
    }*/

    onPatternSelectPixmapDraw();

    updateConfig();
}

void DisplayWidget::removeLayer()
{
    // Lambda 处理警告消息
    auto showWarning = [this](const QString& message) {
        QMessageBox::warning(this, tr("Warning"), message);
        };

    // 获取当前选中的 Pattern
    QListWidgetItem* selectedItem = patternList->currentItem();

    if (!selectedItem && patternList->count() > 0) {
        patternList->setCurrentRow(0);
        selectedItem = patternList->currentItem();
    }

    if (!selectedItem) {
        showWarning(tr("No pattern selected!"));
        return;
    }

    QString patternName = selectedItem->text();
    auto patternPtr = PatternManager::getPatternByName(patternName);
    if (!patternPtr) {
        showWarning(tr("Pattern not found!"));
        return;
    }

    // 获取所有选中的 Layer
    QList<QListWidgetItem*> selectedLayers = layerList->selectedItems();
    if (selectedLayers.isEmpty()) {
        showWarning(tr("No layer selected!"));
        return;
    }

    // 确认删除对话框
    QString message = tr("Are you sure you want to delete the selected layers?\n\n");
    for (auto* item : selectedLayers) {
        message += "- " + item->text() + "\n";
    }

    int reply = QMessageBox::question(this, tr("Confirm Deletion"), message,
        QMessageBox::Yes | QMessageBox::No);
    if (reply != QMessageBox::Yes) {
        return;
    }

    /// 倒序删除，避免索引错乱
    std::sort(selectedLayers.begin(), selectedLayers.end(),
        [this](QListWidgetItem* a, QListWidgetItem* b) {
            return layerList->row(a) > layerList->row(b);
        });

    for (auto* item : selectedLayers) {
        int index = layerList->row(item);
        patternPtr->removeLayer(index);
        delete layerList->takeItem(index);
    }

    updateLayerListView(patternPtr);

    onPatternSelectPixmapDraw();

    updateConfig();

}

void DisplayWidget::updateLayerListView(const std::shared_ptr<Pattern>& patternPtr)
{
    // 如果 patternList 没有选中项，则不执行更新
    if (!patternList->currentItem()) { return; }
    if (!patternPtr) { return; }
    if (!layerList) { return; }

    // 记录当前选中的图层索引
    int selectedIndex = layerList->currentRow();
    layerList->clear();

    // 获取所有图层，并更新UI，显示图层类型作为层的名称
    auto layers = patternPtr->getLayers();
    for (size_t i = 0; i < static_cast<size_t>(layers.size()); ++i) {
        const auto& layer = layers[i];
        if (layer) {
            QWidget* itemWidget = new QWidget();
            QHBoxLayout* itemLayout = new QHBoxLayout(itemWidget);

            // 创建标签显示图层类型
            QLabel* label = new QLabel(layer->getLayerType());
            label->setVisible(false);  // 隐藏文本标签

            QCheckBox* checkBox = new QCheckBox();
            checkBox->setChecked(layer->getLayerStatus());

            itemLayout->addWidget(label);
            itemLayout->addStretch();
            itemLayout->addWidget(checkBox);

            // 创建 QListWidgetItem
            QListWidgetItem* item = new QListWidgetItem(layerList);
            item->setSizeHint(itemWidget->sizeHint());
            item->setText(layer->getLayerType());

            // 设置项的 Widget
            layerList->setItemWidget(item, itemWidget);
            item->setData(Qt::UserRole, i);  // 使用 UserRole 来存储图层的索引

            connect(checkBox, &QCheckBox::stateChanged, [this, i](int state) {
                auto patternPtr = PatternManager::getPatternByName(patternList->currentItem()->text());
                if (!patternPtr) return;

                auto layers = patternPtr->getLayers();
                if (i >= 0 && i < layers.size()) {
                    layers[i]->setLayerStatus(state == Qt::Checked);

                    onPatternSelectPixmapDraw();
                }
            });
        }

        // 设置复选框的状态（对于当前选中的层）
        std::shared_ptr<Layer> foundLayer = layers[i];
        QWidget* itemWidget = layerList->itemWidget(layerList->item(i));
        QCheckBox* checkBox = itemWidget->findChild<QCheckBox*>();
        if (checkBox) {
            checkBox->setChecked(foundLayer->getLayerStatus());
        }
    }

    // 如果 layerList 为空，则不选择任何项
    if (layerList->count() == 0) {
        return;
    }

    // 只有在 patternList 有选中项时，才执行选中 layerList 的逻辑
    if (selectedIndex >= 0 && selectedIndex < layerList->count()) {
        layerList->setCurrentRow(selectedIndex);
    }
    else {
        layerList->setCurrentRow(0);
    }

    onPatternSelectPixmapDraw();
}

void DisplayWidget::onLayerListItemChanged() {
    if (!patternList->currentItem()) return;

    auto patternPtr = PatternManager::getPatternByName(patternList->currentItem()->text());
    if (!patternPtr) return;

    auto originalLayers = patternPtr->getLayers();
    QList<std::shared_ptr<Layer>> newOrder;

    for (int i = 0; i < layerList->count(); ++i) {
        QListWidgetItem* item = layerList->item(i);
        int originalIndex = item->data(Qt::UserRole).toInt();
        if (originalIndex >= 0 && originalIndex < originalLayers.size()) {
            newOrder.append(originalLayers[originalIndex]);
        }
    }

    patternPtr->updateLayerOrder(newOrder);

    updateLayerListView(patternPtr);

    updateConfig();
}

void DisplayWidget::showLayerListContextMenu(const QPoint& pos)
{
    // 检查鼠标是否悬停在某个 item 上
    QListWidgetItem* hoveredItem = layerList->itemAt(pos);
    bool hasSelection = !layerList->selectedItems().isEmpty();

    // 创建菜单
    QMenu contextMenu(this);

    // 添加菜单项
    QAction* addAction = contextMenu.addAction(tr("Add"));
    QAction* editAction = contextMenu.addAction(tr("Edit"));
    QAction* removeAction = contextMenu.addAction(tr("Remove"));
    contextMenu.addSeparator();
    QAction* moveUpAction = contextMenu.addAction(tr("Move Up"));
    QAction* moveDownAction = contextMenu.addAction(tr("Move Down"));

    // 执行菜单并获取用户选择
    QAction* selectedAction = contextMenu.exec(layerList->viewport()->mapToGlobal(pos));

    // 处理用户选择
    if (selectedAction == addAction) {
        addLayer();
    }
    else if (selectedAction == editAction) {
        editLayer();
    }
    else if (selectedAction == removeAction) {
        removeLayer();
    }
    else if (selectedAction == moveUpAction) {
        moveLayerUp();
    }
    else if (selectedAction == moveDownAction) {
        moveLayerDown();
    }
}

void DisplayWidget::moveLayerUp() {
    int currentRow = layerList->currentRow();
    if (currentRow == -1) return;
    if (currentRow == 0) return;

    QListWidgetItem* currentItem = layerList->takeItem(currentRow);
    layerList->insertItem(currentRow - 1, currentItem);
    layerList->setCurrentRow(currentRow - 1);

    onLayerListItemChanged();

    updateConfig();
}

void DisplayWidget::moveLayerDown() {
    int currentRow = layerList->currentRow();
    if (currentRow == -1) return;
    if (currentRow >= layerList->count() - 1) return;

    QListWidgetItem* currentItem = layerList->takeItem(currentRow);
    layerList->insertItem(currentRow + 1, currentItem);
    layerList->setCurrentRow(currentRow + 1);

    onLayerListItemChanged();

    updateConfig();
}

void DisplayWidget::outputAllPatternInfo()
{
    PatternManager patternManager;
    patternManager.printPatternInfo();

}

////////////////////////////////////////////////////////////////////----------I/O---------- /////////////////////////////////////////////////////////////////////

// 保存为Json格式文件
void DisplayWidget::saveConfiguration() {
    // 获取当前所有的 Pattern 和 Layer 配置
    QJsonObject rootObject;

    // 保存 Pattern 配置信息
    QJsonArray patternArray;
    for (const auto& pattern : PatternManager::getAllPatterns()) {
        QJsonObject patternObject;
        patternObject["patternName"] = pattern->getPatternName();
        patternObject["patternWidth"] = pattern->getPatternWidth();
        patternObject["patternHeight"] = pattern->getPatternHeight();
        patternObject["patternDescription"] = pattern->getPatternDescription();

        // 保存图层信息
        QJsonArray layerArray;
        for (const auto& layer : pattern->getLayers()) {
            QJsonObject layerObject;
            layerObject["layerType"] = layer->getLayerType();
            layerObject["layerStatus"] = layer->getLayerStatus();

            QString layerType = layer->getLayerType();
            if (layerType == "Mono") {
                processMonoLayer(layer.get(), layerObject);
            } else if (layerType == "Gradient") {
                processGradientLayer(layer.get(), layerObject);
            } else if (layerType == "Checker") {
                processCheckerLayer(layer.get(), layerObject);
            } else if (layerType == "CRect") {
                processCRectLayer(layer.get(), layerObject);
            } else if (layerType == "CCircle") {
                processCCircleLayer(layer.get(), layerObject);
            } else if (layerType == "BitMap") {
                processBitMapLayer(layer.get(), layerObject);
            } else if (layerType == "Line") {
                processLineLayer(layer.get(), layerObject);
            } else if (layerType == "Text") {
                processTextLayer(layer.get(), layerObject);
            } else if (layerType == "Picture") {
                processPictureLayer(layer.get(), layerObject);
            }

            layerArray.append(layerObject);
        }
        patternObject["layers"] = layerArray;
        patternArray.append(patternObject);
    }

    rootObject["patterns"] = patternArray;

    // 将数据写入文件
    QFile saveFile("config.json");
    if (saveFile.open(QIODevice::WriteOnly)) {
        QJsonDocument saveDoc(rootObject);
        saveFile.write(saveDoc.toJson());
    }
    else {
        QMessageBox::critical(nullptr, "Save Failed", "Unable to open the file for saving. Please check permissions or disk space!");
    }
}

// 保存.dat格式文件
bool DisplayWidget::saveConfigurationBinaryPro(const QString& filePath) {
    // QString savePath = filePath.isEmpty() ? currentConfigPath : filePath;  // 不设置默认路径
    QString savePath = filePath;

    if (savePath.isEmpty()) {
        QMessageBox::critical(nullptr, tr("Save Failed"), tr("No valid file path provided."));
        return false;
    }

    QFile saveFile(savePath);

    if (!saveFile.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(nullptr, tr("Save Failed"), tr("Unable to open file for saving."));
        return false;
    }

    QDataStream out(&saveFile);

    // 获取当前所有的 Pattern
    auto patterns = PatternManager::getAllPatterns();
    out << static_cast<quint32>(patterns.size());  // 先写入 Pattern 数量

    for (const auto& pattern : patterns) {
        // 写入 Pattern 信息
        out << pattern->getPatternName()
            << static_cast<quint32>(pattern->getPatternWidth())
            << static_cast<quint32>(pattern->getPatternHeight())
            << pattern->getPatternDescription();

        // 获取图层信息
        auto layers = pattern->getLayers();
        out << static_cast<quint32>(layers.size());  // 先写入图层数量

        for (const auto& layer : layers) {
            out << layer->getLayerType()
                << layer->getLayerStatus();

            // 根据 Layer 类型序列化参数
            if (layer->getLayerType() == "Mono") {
                serializeMonoLayer(out, layer.get());
            }
            else if (layer->getLayerType() == "Gradient") {
                serializeGradientLayer(out, layer.get());
            }
            else if (layer->getLayerType() == "Checker") {
                serializeCheckerLayer(out, layer.get());
            }
            else if (layer->getLayerType() == "BitMap") {
                serializeBitMapLayer(out, layer.get());
            }
            else if (layer->getLayerType() == "CRect") {
                serializeCRectLayer(out, layer.get());
            }
            else if (layer->getLayerType() == "CCircle") {
                serializeCCircleLayer(out, layer.get());
            }
            else if (layer->getLayerType() == "Line") {
                serializeLineLayer(out, layer.get());
            }
            else if (layer->getLayerType() == "Text") {
                serializeTextLayer(out, layer.get());
            }
            else if (layer->getLayerType() == "Picture") {
                serializePictureLayer(out, layer.get());
            }
        }
    }

    saveFile.close();
    return true;
}

// 各Layer类型写入Json文件函数
void DisplayWidget::processMonoLayer(Layer* layer, QJsonObject& layerObject) {
    MonoLayer* monoLayer = dynamic_cast<MonoLayer*>(layer);
    if (!monoLayer) return;

    QMap<QString, QVariant> params = monoLayer->getParameters();
    QJsonObject monoLayerParams;
    monoLayerParams["startX"] = params["startX"].toInt();
    monoLayerParams["startY"] = params["startY"].toInt();
    monoLayerParams["width"] = params["width"].toInt();
    monoLayerParams["height"] = params["height"].toInt();
    monoLayerParams["lineWidth"] = params["lineWidth"].toInt(); 
    monoLayerParams["fill"] = params["fill"].toBool();
    monoLayerParams["color"] = params["color"].value<QColor>().name(QColor::HexArgb);

    layerObject["monoLayerParams"] = monoLayerParams;
}

void DisplayWidget::processGradientLayer(Layer* layer, QJsonObject& layerObject) {
    GradientLayer* gradientLayer = dynamic_cast<GradientLayer*>(layer);
    if (!gradientLayer) return;

    QMap<QString, QVariant> params = gradientLayer->getParameters();
    QJsonObject gradientLayerParams;
    gradientLayerParams["startX"] = params["startX"].toInt();
    gradientLayerParams["startY"] = params["startY"].toInt();
    gradientLayerParams["width"] = params["width"].toInt();
    gradientLayerParams["height"] = params["height"].toInt();
    gradientLayerParams["counts"] = params["counts"].toInt();
    gradientLayerParams["orientation"] = params["orientation"].toString();

    // 处理 gradientBars
    QJsonArray gradientBarsArray;
    QList<QMap<QString, QVariant>> gradientBars = params["gradientBars"].value<QList<QMap<QString, QVariant>>>();

    for (const auto& gradientBar : gradientBars) {
        QJsonObject barJson;
        barJson["steps"] = gradientBar["steps"].toInt();
        barJson["startColor"] = gradientBar["startColor"].value<QColor>().name(QColor::HexArgb);
        barJson["endColor"] = gradientBar["endColor"].value<QColor>().name(QColor::HexArgb);

        gradientBarsArray.append(barJson);
    }

    gradientLayerParams["gradientBars"] = gradientBarsArray;

    layerObject["gradientLayerParams"] = gradientLayerParams;
}

void DisplayWidget::processCheckerLayer(Layer* layer, QJsonObject& layerObject) {
    CheckerLayer* checkerLayer = dynamic_cast<CheckerLayer*>(layer);
    if (!checkerLayer) return;

    QMap<QString, QVariant> params = checkerLayer->getParameters();
    QJsonObject checkerLayerParams;
    checkerLayerParams["startX"] = params["startX"].toInt();
    checkerLayerParams["startY"] = params["startY"].toInt();
    checkerLayerParams["width"] = params["width"].toInt();
    checkerLayerParams["height"] = params["height"].toInt();
    checkerLayerParams["rows"] = params["rows"].toInt();
    checkerLayerParams["cols"] = params["cols"].toInt();
    checkerLayerParams["color1"] = params["color1"].value<QColor>().name(QColor::HexArgb);
    checkerLayerParams["color2"] = params["color2"].value<QColor>().name(QColor::HexArgb);

    layerObject["checkerLayerParams"] = checkerLayerParams;
}

void DisplayWidget::processBitMapLayer(Layer* layer, QJsonObject& layerObject) {
    BitMapLayer* bitMapLayer = dynamic_cast<BitMapLayer*>(layer);
    if (!bitMapLayer) return;

    QMap<QString, QVariant> params = bitMapLayer->getParameters();
    QJsonObject bitMapLayerParams;
    bitMapLayerParams["startX"] = params["startX"].toInt();
    bitMapLayerParams["startY"] = params["startY"].toInt();
    bitMapLayerParams["width"] = params["width"].toInt();
    bitMapLayerParams["height"] = params["height"].toInt();
    bitMapLayerParams["bitMapWidth"] = params["bitMapWidth"].toInt();
    bitMapLayerParams["bitMapHeight"] = params["bitMapHeight"].toInt();
    bitMapLayerParams["color"] = params["color"].value<QColor>().name(QColor::HexArgb);

    // 处理 pixelColors
    QList<QColor> pixelColors = params["pixelColors"].value<QList<QColor>>();
    QJsonArray pixelColorsArray;
    for (const QColor& pixelColor : pixelColors) {
        pixelColorsArray.append(pixelColor.name(QColor::HexArgb));
    }
    bitMapLayerParams["pixelColors"] = pixelColorsArray;

    layerObject["bitMapLayerParams"] = bitMapLayerParams;
}

void DisplayWidget::processCRectLayer(Layer* layer, QJsonObject& layerObject) {
    CRectLayer* cRectLayer = dynamic_cast<CRectLayer*>(layer);
    if (!cRectLayer) return;

    QMap<QString, QVariant> params = cRectLayer->getParameters();
    QJsonObject cRectLayerParams;
    cRectLayerParams["startX"] = params["startX"].toInt();
    cRectLayerParams["startY"] = params["startY"].toInt();
    cRectLayerParams["width"] = params["width"].toInt();
    cRectLayerParams["height"] = params["height"].toInt();
    cRectLayerParams["counts"] = params["counts"].toInt();
    cRectLayerParams["fill"] = params["fill"].toBool();
    cRectLayerParams["color1"] = params["color1"].value<QColor>().name(QColor::HexArgb);
    cRectLayerParams["color2"] = params["color2"].value<QColor>().name(QColor::HexArgb);

    layerObject["cRectLayerParams"] = cRectLayerParams;
}

void DisplayWidget::processCCircleLayer(Layer* layer, QJsonObject& layerObject) {
    CCircleLayer* cCircleLayer = dynamic_cast<CCircleLayer*>(layer);
    if (!cCircleLayer) return;

    QMap<QString, QVariant> params = cCircleLayer->getParameters();
    QJsonObject cCircleLayerParams;
    cCircleLayerParams["centerX"] = params["centerX"].toInt();
    cCircleLayerParams["centerY"] = params["centerY"].toInt();
    cCircleLayerParams["radiusX"] = params["radiusX"].toInt();
    cCircleLayerParams["radiusY"] = params["radiusY"].toInt();
    cCircleLayerParams["counts"] = params["counts"].toInt();
    cCircleLayerParams["concentric"] = params["concentric"].toBool();
    cCircleLayerParams["lineWidth"] = params["lineWidth"].toInt();
    cCircleLayerParams["color"] = params["color"].value<QColor>().name(QColor::HexArgb);

    layerObject["cCircleLayerParams"] = cCircleLayerParams;
}

void DisplayWidget::processLineLayer(Layer* layer, QJsonObject& layerObject) {
    LineLayer* lineLayer = dynamic_cast<LineLayer*>(layer);
    if (!lineLayer) return;

    QMap<QString, QVariant> params = lineLayer->getParameters();
    QJsonObject lineLayerParams;
    lineLayerParams["startX"] = params["startX"].toInt();
    lineLayerParams["startY"] = params["startY"].toInt();
    lineLayerParams["endX"] = params["endX"].toInt();
    lineLayerParams["endY"] = params["endY"].toInt();
    lineLayerParams["lineWidth"] = params["lineWidth"].toInt();
    lineLayerParams["lineColor"] = params["lineColor"].value<QColor>().name(QColor::HexArgb);

    layerObject["lineLayerParams"] = lineLayerParams;
}

void DisplayWidget::processTextLayer(Layer* layer, QJsonObject& layerObject) {
    TextLayer* textLayer = dynamic_cast<TextLayer*>(layer);
    if (!textLayer) return;

    QMap<QString, QVariant> params = textLayer->getParameters();
    QJsonObject textLayerParams;
    textLayerParams["startX"] = params["startX"].toInt();
    textLayerParams["startY"] = params["startY"].toInt();
    textLayerParams["text"] = params["text"].toString();
    textLayerParams["font"] = params["font"].toString();
    textLayerParams["size"] = params["size"].toInt();
    textLayerParams["style"] = params["style"].toString();
    textLayerParams["textColor"] = params["textColor"].value<QColor>().name(QColor::HexArgb);

    layerObject["textLayerParams"] = textLayerParams;
}

void DisplayWidget::processPictureLayer(Layer* layer, QJsonObject& layerObject) {
    PictureLayer* pictureLayer = dynamic_cast<PictureLayer*>(layer);
    if (!pictureLayer) return;

    QMap<QString, QVariant> params = pictureLayer->getParameters();
    QJsonObject pictureLayerParams;
    pictureLayerParams["startX"] = params["startX"].toInt();
    pictureLayerParams["startY"] = params["startY"].toInt();
    pictureLayerParams["width"] = params["width"].toInt();
    pictureLayerParams["height"] = params["height"].toInt();
    pictureLayerParams["mode"] = params["mode"].toString();
    pictureLayerParams["imagePath"] = params["imagePath"].toString();

    layerObject["pictureLayerParams"] = pictureLayerParams;
}

// 各Layer类型写入.dat文件函数
void DisplayWidget::serializeMonoLayer(QDataStream& out, Layer* layer) {
    MonoLayer* monoLayer = dynamic_cast<MonoLayer*>(layer);
    if (!monoLayer) return;

    QMap<QString, QVariant> params = monoLayer->getParameters();

    out << params["startX"].toInt();
    out << params["startY"].toInt();
    out << params["width"].toInt();
    out << params["height"].toInt();
    out << params["fill"].toBool();
    out << params["color"].value<QColor>();
}

void DisplayWidget::serializeGradientLayer(QDataStream& out, Layer* layer) {
    GradientLayer* gradientLayer = dynamic_cast<GradientLayer*>(layer);
    if (!gradientLayer) return;

    QMap<QString, QVariant> params = gradientLayer->getParameters();

    out << params["startX"].toInt();
    out << params["startY"].toInt();
    out << params["width"].toInt();
    out << params["height"].toInt();
    out << params["counts"].toInt();
    out << params["orientation"].toString();

    // 获取 gradientBars
    QList<QMap<QString, QVariant>> gradientBars = params["gradientBars"].value<QList<QMap<QString, QVariant>>>();

    // 写入 gradientBars 数量
    out << static_cast<quint32>(gradientBars.size());

    for (const auto& gradientBar : gradientBars) {
        out << gradientBar["steps"].toInt();
        out << gradientBar["startColor"].value<QColor>();
        out << gradientBar["endColor"].value<QColor>();
    }
}

void DisplayWidget::serializeCheckerLayer(QDataStream& out, Layer* layer) {
    CheckerLayer* checkerLayer = dynamic_cast<CheckerLayer*>(layer);
    if (!checkerLayer) return;

    QMap<QString, QVariant> params = checkerLayer->getParameters();

    out << params["startX"].toInt();
    out << params["startY"].toInt();
    out << params["width"].toInt();
    out << params["height"].toInt();
    out << params["rows"].toInt();
    out << params["cols"].toInt();
    out << params["color1"].value<QColor>();
    out << params["color2"].value<QColor>();
}

void DisplayWidget::serializeBitMapLayer(QDataStream& out, Layer* layer) {
    BitMapLayer* bitMapLayer = dynamic_cast<BitMapLayer*>(layer);
    if (!bitMapLayer) return;

    QMap<QString, QVariant> params = bitMapLayer->getParameters();

    out << params["startX"].toInt();
    out << params["startY"].toInt();
    out << params["width"].toInt();
    out << params["height"].toInt();
    out << params["bitMapWidth"].toInt();
    out << params["bitMapHeight"].toInt();
    out << params["color"].value<QColor>();

    QList<QColor> pixelColors = params["pixelColors"].value<QList<QColor>>();
    quint32 pixelCount = pixelColors.size();
    out << pixelCount;
    for (const QColor& pixelColor : pixelColors) {
        out << pixelColor;
    }
}

void DisplayWidget::serializeCRectLayer(QDataStream& out, Layer* layer) {
    CRectLayer* cRectLayer = dynamic_cast<CRectLayer*>(layer);
    if (!cRectLayer) return;

    QMap<QString, QVariant> params = cRectLayer->getParameters();

    out << params["startX"].toInt();
    out << params["startY"].toInt();
    out << params["width"].toInt();
    out << params["height"].toInt();
    out << params["counts"].toInt();
    out << params["fill"].toBool();
    out << params["color1"].value<QColor>();
    out << params["color2"].value<QColor>();
}

void DisplayWidget::serializeCCircleLayer(QDataStream& out, Layer* layer) {
    CCircleLayer* cCircleLayer = dynamic_cast<CCircleLayer*>(layer);
    if (!cCircleLayer) return;

    QMap<QString, QVariant> params = cCircleLayer->getParameters();

    out << params["centerX"].toInt();
    out << params["centerY"].toInt();
    out << params["radiusX"].toInt();
    out << params["radiusY"].toInt();
    out << params["counts"].toInt();
    out << params["concentric"].toBool();
    out << params["lineWidth"].toInt();
    out << params["color"].value<QColor>();
}

void DisplayWidget::serializeLineLayer(QDataStream& out, Layer* layer) {
    LineLayer* lineLayer = dynamic_cast<LineLayer*>(layer);
    if (!lineLayer) return;

    QMap<QString, QVariant> params = lineLayer->getParameters();

    out << params["startX"].toInt();
    out << params["startY"].toInt();
    out << params["endX"].toInt();
    out << params["endY"].toInt();
    out << params["lineWidth"].toInt();
    out << params["lineColor"].value<QColor>();
}

void DisplayWidget::serializeTextLayer(QDataStream& out, Layer* layer) {
    TextLayer* textLayer = dynamic_cast<TextLayer*>(layer);
    if (!textLayer) return;

    QMap<QString, QVariant> params = textLayer->getParameters();

    out << params["startX"].toInt();
    out << params["startY"].toInt();
    out << params["text"].toString();
    out << params["font"].toString();
    out << params["size"].toInt();
    out << params["style"].toString();
    out << params["textColor"].value<QColor>();
}

void DisplayWidget::serializePictureLayer(QDataStream& out, Layer* layer) {
    PictureLayer* pictureLayer = dynamic_cast<PictureLayer*>(layer);
    if (!pictureLayer) return;

    QMap<QString, QVariant> params = pictureLayer->getParameters();

    out << params["startX"].toInt();
    out << params["startY"].toInt();
    out << params["width"].toInt();
    out << params["height"].toInt();
    out << params["mode"].toString();
    out << params["imagePath"].toString();
}

// 载入Json配置文件
void DisplayWidget::loadConfiguration()
{
    QFile loadFile("config.json");

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open file for loading configuration.";
        return;
    }

    QJsonDocument loadDoc = QJsonDocument::fromJson(loadFile.readAll());
    if (loadDoc.isNull()) {
        qWarning() << "Failed to parse configuration file.";
        return;
    }

    // 解析 JSON 文件
    QJsonObject rootObject = loadDoc.object();
    QJsonArray patternArray = rootObject["patterns"].toArray();

    for (int i = 0; i < patternArray.size(); ++i) {
        QJsonObject patternObject = patternArray[i].toObject();
        QString patternName = patternObject["patternName"].toString();
        int patternWidth = patternObject["patternWidth"].toInt();
        int patternHeight = patternObject["patternHeight"].toInt();
        QString patternDescription = patternObject["patternDescription"].toString();

        // 使用 addPattern 创建一个新的 Pattern，并传入读取的属性
        auto patternPtr = PatternManager::loadPatternwithJson(patternName, patternWidth, patternHeight, patternDescription);

        // 读取图层信息
        QJsonArray layerArray = patternObject["layers"].toArray();
        for (const QJsonValue& layerValue : layerArray) {
            QJsonObject layerObject = layerValue.toObject();
            QString layerType = layerObject["layerType"].toString();
            bool layerStatus = layerObject["layerStatus"].toBool();

            std::shared_ptr<Layer> layer = nullptr;

            // 处理类型图层
            if (layerType == "Mono") {
                layer = createMonoLayer(layerObject, layerType);
            } else if (layerType == "Gradient") {
                layer = createGradientLayer(layerObject, layerType);
            } else if (layerType == "Checker") {
                layer = createCheckerLayer(layerObject, layerType);
            } else if (layerType == "BitMap") {
                layer = createBitMapLayer(layerObject, layerType);
            } else if (layerType == "CRect") {
                layer = createCRectLayer(layerObject, layerType);
            } else if (layerType == "CCircle") {
                layer = createCCircleLayer(layerObject, layerType);
            } else if (layerType == "Line") {
                layer = createLineLayer(layerObject, layerType);
            } else if (layerType == "Text") {
                layer = createTextLayer(layerObject, layerType);
            }else if (layerType == "Picture") {
                layer = createPictureLayer(layerObject, layerType);
            }

            if (layer) {
                layer->setLayerType(layerType);
                layer->setLayerStatus(layerStatus);
                patternPtr->addLayer(std::shared_ptr<Layer>(layer)); // 将图层添加到 Pattern
            }
            updateLayerListView(patternPtr);
        }
    }
    updatePatternListView();
}

// 载入.dat配置文件
void DisplayWidget::loadConfigurationBinaryPro(const QString& filePath) {
    // ====== 清理旧数据 ======
    PatternManager::clearAllPatterns();
    patternList->clear();
    layerList->clear();
    onPatternSelectPixmapDraw();

    // 获取文件路径
    //QString finalPath = filePath.isEmpty() ? "config.dat" : filePath;  // 不设置默认路径
    QString finalPath = filePath;

    if (finalPath.isEmpty()) {
        finalPath = QDir::currentPath() + "/config.dat";
    }

    currentConfigPath = finalPath;
    QFile loadFile(finalPath);

    // 无默认配置文件"config.dat"时，使用临时配置，不加载任何内容
    if (!loadFile.exists()) {
        return;
    }

    QString part1 = tr("could not open file: ");
    QString message = part1 + QString("%1").arg(finalPath);

    if (!loadFile.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, tr("Load Failed"), message);
        return;
    }

    QDataStream in(&loadFile);

    quint32 patternCount;
    in >> patternCount;  // 读取 Pattern 数量

    for (quint32 i = 0; i < patternCount; ++i) {
        QString patternName;
        quint32 patternWidth, patternHeight;
        QString patternDescription;

        in >> patternName >> patternWidth >> patternHeight >> patternDescription;

        // 创建 Pattern
        auto patternPtr = PatternManager::loadPattern(patternName, patternWidth, patternHeight, patternDescription, i);

        quint32 layerCount;
        in >> layerCount;  // 读取图层数量

        for (quint32 j = 0; j < layerCount; ++j) {
            QString layerType;
            bool layerStatus;
            in >> layerType >> layerStatus;

            std::shared_ptr<Layer> layer = nullptr;

            // 根据 Layer 类型反序列化
            if (layerType == "Mono") {
                layer = deserializeMonoLayer(in, layerType);
            }
            else if (layerType == "Gradient") {
                layer = deserializeGradientLayer(in, layerType);
            }
            else if (layerType == "Checker") {
                layer = deserializeCheckerLayer(in, layerType);
            }
            else if (layerType == "BitMap") {
                layer = deserializeBitMapLayer(in, layerType);
            }
            else if (layerType == "CRect") {
                layer = deserializeCRectLayer(in, layerType);
            }
            else if (layerType == "CCircle") {
                layer = deserializeCCircleLayer(in, layerType);
            }
            else if (layerType == "Line") {
                layer = deserializeLineLayer(in, layerType);
            }
            else if (layerType == "Text") {
                layer = deserializeTextLayer(in, layerType);
            }
            else if (layerType == "Picture") {
                layer = deserializePictureLayer(in, layerType);
            }

            if (layer) {
                layer->setLayerType(layerType);
                layer->setLayerStatus(layerStatus);
                patternPtr->addLayer(layer);
            }
        }
    }
    updatePatternListView();

    if (!PatternManager::getAllPatterns().isEmpty()) {
        patternList->setCurrentRow(0);
        auto pattern = PatternManager::getAllPatterns().at(0);
        if (pattern) {
            updateLayerListView(pattern);
            if (!pattern->getLayers().isEmpty()) {
                layerList->setCurrentRow(0); 
            }
        }
    }

    loadFile.close();
}

// 各Layer类型载入Json文件函数
std::shared_ptr<MonoLayer> DisplayWidget::createMonoLayer(const QJsonObject& layerObject, const QString& layerType)
{
    std::shared_ptr<MonoLayer> monoLayer = std::make_shared<MonoLayer>(layerType);
    QJsonObject monoLayerParams = layerObject["monoLayerParams"].toObject();
    QMap<QString, QVariant> params;
    params["startX"] = monoLayerParams["startX"].toInt();
    params["startY"] = monoLayerParams["startY"].toInt();
    params["width"] = monoLayerParams["width"].toInt();
    params["height"] = monoLayerParams["height"].toInt();
    params["fill"] = monoLayerParams["fill"].toBool();
    params["color"] = QColor::fromString(monoLayerParams["color"].toString());

    monoLayer->setParameters(params);
    return monoLayer;
}

std::shared_ptr<GradientLayer> DisplayWidget::createGradientLayer(const QJsonObject& layerObject, const QString& layerType)
{
    std::shared_ptr<GradientLayer> gradientLayer = std::make_shared<GradientLayer>(layerType);
    QJsonObject gradientLayerParams = layerObject["gradientLayerParams"].toObject();
    QMap<QString, QVariant> params;
    params["startX"] = gradientLayerParams["startX"].toInt();
    params["startY"] = gradientLayerParams["startY"].toInt();
    params["width"] = gradientLayerParams["width"].toInt();
    params["height"] = gradientLayerParams["height"].toInt();
    params["counts"] = gradientLayerParams["counts"].toInt();
    params["orientation"] = gradientLayerParams["orientation"].toString();

    // 处理 gradientBars
    QJsonArray gradientBarsArray = gradientLayerParams["gradientBars"].toArray();
    QList<QMap<QString, QVariant>> gradientBars;

    for (const QJsonValue& barValue : gradientBarsArray) {
        QJsonObject barJson = barValue.toObject();
        QMap<QString, QVariant> gradientBar;
        gradientBar["steps"] = barJson["steps"].toInt();
        gradientBar["startColor"] = QColor(barJson["startColor"].toString());
        gradientBar["endColor"] = QColor(barJson["endColor"].toString());

        gradientBars.append(gradientBar);
    }

    // 将 gradientBars 转换为 QVariant
    params["gradientBars"] = QVariant::fromValue(gradientBars);

    gradientLayer->setParameters(params);
    return gradientLayer;
}

std::shared_ptr<CheckerLayer> DisplayWidget::createCheckerLayer(const QJsonObject& layerObject, const QString& layerType) {
    std::shared_ptr<CheckerLayer> checkerLayer = std::make_shared<CheckerLayer>(layerType);
    QJsonObject checkerLayerParams = layerObject["checkerLayerParams"].toObject();
    QMap<QString, QVariant> params;
    params["startX"] = checkerLayerParams["startX"].toInt();
    params["startY"] = checkerLayerParams["startY"].toInt();
    params["width"] = checkerLayerParams["width"].toInt();
    params["height"] = checkerLayerParams["height"].toInt();
    params["rows"] = checkerLayerParams["rows"].toInt();
    params["cols"] = checkerLayerParams["cols"].toInt();
    params["color1"] = QColor::fromString(checkerLayerParams["color1"].toString());
    params["color2"] = QColor::fromString(checkerLayerParams["color2"].toString());

    checkerLayer->setParameters(params);
    return checkerLayer;
}

std::shared_ptr<BitMapLayer> DisplayWidget::createBitMapLayer(const QJsonObject& layerObject, const QString& layerType) {
    std::shared_ptr<BitMapLayer> bitMapLayer = std::make_shared<BitMapLayer>(layerType);
    QJsonObject bitMapLayerParams = layerObject["bitMapLayerParams"].toObject();
    QMap<QString, QVariant> params;
    params["startX"] = bitMapLayerParams["startX"].toInt();
    params["startY"] = bitMapLayerParams["startY"].toInt();
    params["width"] = bitMapLayerParams["width"].toInt();
    params["height"] = bitMapLayerParams["height"].toInt();
    params["bitMapWidth"] = bitMapLayerParams["bitMapWidth"].toInt();
    params["bitMapHeight"] = bitMapLayerParams["bitMapHeight"].toInt();
    params["color"] = QColor::fromString(bitMapLayerParams["color"].toString());

    QJsonArray pixelColorsArray = bitMapLayerParams["pixelColors"].toArray();
    QList<QColor> pixelColors;
    for (const QJsonValue& value : pixelColorsArray) {
        pixelColors.append(QColor(value.toString())); 
    }
    params["pixelColors"] = QVariant::fromValue(pixelColors);

    bitMapLayer->setParameters(params);
    return bitMapLayer;
}

std::shared_ptr<CRectLayer> DisplayWidget::createCRectLayer(const QJsonObject& layerObject, const QString& layerType) {
    std::shared_ptr<CRectLayer> cRectLayer = std::make_shared<CRectLayer>(layerType);
    QJsonObject cRectLayerParams = layerObject["cRectLayerParams"].toObject();
    QMap<QString, QVariant> params;
    params["startX"] = cRectLayerParams["startX"].toInt();
    params["startY"] = cRectLayerParams["startY"].toInt();
    params["width"] = cRectLayerParams["width"].toInt();
    params["height"] = cRectLayerParams["height"].toInt();
    params["counts"] = cRectLayerParams["counts"].toInt();
    params["fill"] = cRectLayerParams["fill"].toBool();
    params["color1"] = QColor::fromString(cRectLayerParams["color1"].toString());
    params["color2"] = QColor::fromString(cRectLayerParams["color2"].toString());

    cRectLayer->setParameters(params);
    return cRectLayer;
}

std::shared_ptr<CCircleLayer> DisplayWidget::createCCircleLayer(const QJsonObject& layerObject, const QString& layerType) {
    std::shared_ptr<CCircleLayer> cCircletLayer = std::make_shared<CCircleLayer>(layerType);
    QJsonObject cCircleLayerParams = layerObject["cCircleLayerParams"].toObject();
    QMap<QString, QVariant> params;

    params["centerX"] = cCircleLayerParams["centerX"].toInt();
    params["centerY"] = cCircleLayerParams["centerY"].toInt();
    params["radiusX"] = cCircleLayerParams["radiusX"].toInt();
    params["radiusY"] = cCircleLayerParams["radiusY"].toInt();
    params["counts"] = cCircleLayerParams["counts"].toInt();
    params["concentric"] = cCircleLayerParams["concentric"].toBool();
    params["lineWidth"] = cCircleLayerParams["lineWidth"].toInt();
    params["color"] = QColor::fromString(cCircleLayerParams["color"].toString());

    cCircletLayer->setParameters(params);
    return cCircletLayer;
}

std::shared_ptr<LineLayer> DisplayWidget::createLineLayer(const QJsonObject& layerObject, const QString& layerType) {
    std::shared_ptr<LineLayer> lineLayer = std::make_shared<LineLayer>(layerType);
    QJsonObject lineLayerParams = layerObject["lineLayerParams"].toObject();
    QMap<QString, QVariant> params;
    params["startX"] = lineLayerParams["startX"].toInt();
    params["startY"] = lineLayerParams["startY"].toInt();
    params["endX"] = lineLayerParams["endX"].toInt();
    params["endY"] = lineLayerParams["endY"].toInt();
    params["lineWidth"] = lineLayerParams["lineWidth"].toInt();
    params["lineColor"] = QColor::fromString(lineLayerParams["lineColor"].toString());

    lineLayer->setParameters(params);
    return lineLayer;
}

std::shared_ptr<TextLayer> DisplayWidget::createTextLayer(const QJsonObject& layerObject, const QString& layerType) {
    std::shared_ptr<TextLayer> textLayer = std::make_shared<TextLayer>(layerType);
    QJsonObject textLayerParams = layerObject["textLayerParams"].toObject();
    QMap<QString, QVariant> params;

    params["startX"] = textLayerParams["startX"].toInt();
    params["startY"] = textLayerParams["startY"].toInt();
    params["text"] = textLayerParams["text"].toString();
    params["font"] = textLayerParams["font"].toString();
    params["size"] = textLayerParams["size"].toInt();
    params["style"] = textLayerParams["style"].toString();
    params["textColor"] = QColor::fromString(textLayerParams["textColor"].toString());

    textLayer->setParameters(params);
    return textLayer;
}

std::shared_ptr<PictureLayer> DisplayWidget::createPictureLayer(const QJsonObject& layerObject, const QString& layerType) {
    std::shared_ptr<PictureLayer> pictureLayer = std::make_shared<PictureLayer>(layerType);
    QJsonObject pictureLayerParams = layerObject["pictureLayerParams"].toObject();
    QMap<QString, QVariant> params;

    params["startX"] = pictureLayerParams["startX"].toInt();
    params["startY"] = pictureLayerParams["startY"].toInt();
    params["width"] = pictureLayerParams["width"].toInt();
    params["height"] = pictureLayerParams["height"].toInt();
    params["mode"] = pictureLayerParams["mode"].toString();
    params["imagePath"] = pictureLayerParams["imagePath"].toString();

    pictureLayer->setParameters(params);
    return pictureLayer;
}

// 各Layer类型载入.dat文件函数
std::shared_ptr<MonoLayer> DisplayWidget::deserializeMonoLayer(QDataStream& in, const QString& layerType) {
    int startX, startY, width, height;
    bool fill;
    QColor color;

    in >> startX >> startY >> width >> height >> fill >> color;

    std::shared_ptr<MonoLayer> monoLayer = std::make_shared<MonoLayer>(layerType);
    QMap<QString, QVariant> params;

    params["startX"] = startX;
    params["startY"] = startY;
    params["width"] = width;
    params["height"] = height;
    params["fill"] = fill;
    params["color"] = color;

    monoLayer->setParameters(params);
    return monoLayer;
}

std::shared_ptr<GradientLayer> DisplayWidget::deserializeGradientLayer(QDataStream& in, const QString& layerType) {
    int startX, startY, width, height, counts;
    QString orientation;
    quint32 gradientBarsCount;

    in >> startX >> startY >> width >> height >> counts >> orientation;
    in >> gradientBarsCount;

    QList<QMap<QString, QVariant>> gradientBars;

    for (quint32 i = 0; i < gradientBarsCount; ++i) {
        int steps;
        QColor startColor, endColor;

        in >> steps >> startColor >> endColor;

        QMap<QString, QVariant> gradientBar;
        gradientBar["steps"] = steps;
        gradientBar["startColor"] = startColor;
        gradientBar["endColor"] = endColor;

        gradientBars.append(gradientBar);
    }

    std::shared_ptr<GradientLayer> gradientLayer = std::make_shared<GradientLayer>(layerType);
    QMap<QString, QVariant> params;

    params["startX"] = startX;
    params["startY"] = startY;
    params["width"] = width;
    params["height"] = height;
    params["counts"] = counts;
    params["orientation"] = orientation;
    params["gradientBars"] = QVariant::fromValue(gradientBars);

    gradientLayer->setParameters(params);
    return gradientLayer;
}

std::shared_ptr<CheckerLayer> DisplayWidget::deserializeCheckerLayer(QDataStream& in, const QString& layerType) {
    int startX, startY, width, height, rows, cols;
    QColor color1, color2;

    in >> startX >> startY >> width >> height >> rows >> cols;
    in >> color1 >> color2;

    std::shared_ptr<CheckerLayer> checkerLayer = std::make_shared<CheckerLayer>(layerType);
    QMap<QString, QVariant> params;

    params["startX"] = startX;
    params["startY"] = startY;
    params["width"] = width;
    params["height"] = height;
    params["rows"] = rows;
    params["cols"] = cols;
    params["color1"] = color1;
    params["color2"] = color2;

    checkerLayer->setParameters(params);
    return checkerLayer;
}

std::shared_ptr<BitMapLayer> DisplayWidget::deserializeBitMapLayer(QDataStream& in, const QString& layerType) {
    int startX, startY, width, height, bitMapWidth, bitMapHeight;
    QColor color;

    in >> startX >> startY >> width >> height >> bitMapWidth >> bitMapHeight;
    in >> color;

    quint32  pixelCount;
    in >> pixelCount;

    QList<QColor> pixelColors;
    for (quint32 i = 0; i < pixelCount; ++i) {
        QColor pixelColor;
        in >> pixelColor;
        pixelColors.append(pixelColor);
    }

    std::shared_ptr<BitMapLayer> bitMapLayer = std::make_shared<BitMapLayer>(layerType);
    QMap<QString, QVariant> params;

    params["startX"] = startX;
    params["startY"] = startY;
    params["width"] = width;
    params["height"] = height;
    params["bitMapWidth"] = bitMapWidth;
    params["bitMapHeight"] = bitMapHeight;
    params["color"] = color;
    params["pixelColors"] = QVariant::fromValue(pixelColors);

    bitMapLayer->setParameters(params);
    return bitMapLayer;
}

std::shared_ptr<CRectLayer> DisplayWidget::deserializeCRectLayer(QDataStream& in, const QString& layerType) {
    int startX, startY, width, height, counts;
    bool fill;
    QColor color1, color2;

    in >> startX >> startY >> width >> height >> counts >> fill;
    in >> color1 >> color2;

    std::shared_ptr<CRectLayer> cRectLayer = std::make_shared<CRectLayer>(layerType);
    QMap<QString, QVariant> params;

    params["startX"] = startX;
    params["startY"] = startY;
    params["width"] = width;
    params["height"] = height;
    params["counts"] = counts;
    params["fill"] = fill;
    params["color1"] = color1;
    params["color2"] = color2;

    cRectLayer->setParameters(params);
    return cRectLayer;
}

std::shared_ptr<CCircleLayer> DisplayWidget::deserializeCCircleLayer(QDataStream& in, const QString& layerType) {
    int centerX, centerY, radiusX, radiusY, counts, lineWidth;
    bool concentric;
    QColor color;

    in >>  centerX >> centerY >> radiusX >> radiusY >> counts >> concentric >> lineWidth;
    in >> color;

    std::shared_ptr<CCircleLayer> cCircleLayer = std::make_shared<CCircleLayer>(layerType);
    QMap<QString, QVariant> params;

    params["centerX"] = centerX;
    params["centerY"] = centerY;
    params["radiusX"] = radiusX;
    params["radiusY"] = radiusY;
    params["counts"] = counts;
    params["concentric"] = concentric;
    params["lineWidth"] = lineWidth;
    params["color"] = color;

    cCircleLayer->setParameters(params);
    return cCircleLayer;
}

std::shared_ptr<LineLayer> DisplayWidget::deserializeLineLayer(QDataStream& in, const QString& layerType) {
    int startX, startY, endX, endY, lineWidth;
    QColor lineColor;

    in >> startX >> startY >> endX >> endY >> lineWidth;
    in >> lineColor;

    std::shared_ptr<LineLayer> lineLayer = std::make_shared<LineLayer>(layerType);
    QMap<QString, QVariant> params;
    params["startX"] = startX;
    params["startY"] = startY;
    params["endX"] = endX;
    params["endY"] = endY;
    params["lineWidth"] = lineWidth;
    params["lineColor"] = lineColor;

    lineLayer->setParameters(params);
    return lineLayer;
}

std::shared_ptr<TextLayer> DisplayWidget::deserializeTextLayer(QDataStream& in, const QString& layerType) {
    int startX, startY, size;
    QString text, font, style;
    QColor textColor;

    in >> startX >> startY;
    in >> text >> font >> size >> style;
    in >> textColor;

    std::shared_ptr<TextLayer> textLayer = std::make_shared<TextLayer>(layerType);
    QMap<QString, QVariant> params;

    params["startX"] = startX;
    params["startY"] = startY;
    params["text"] = text;
    params["font"] = font;
    params["size"] = size;
    params["style"] = style;
    params["textColor"] = textColor;

    textLayer->setParameters(params);
    return textLayer;
}

std::shared_ptr<PictureLayer> DisplayWidget::deserializePictureLayer(QDataStream& in, const QString& layerType) {
    int startX, startY, width, height;
    QString mode, imagePath;

    in >> startX >> startY >> width >> height;
    in >> mode >> imagePath;

    std::shared_ptr<PictureLayer> pictureLayer = std::make_shared<PictureLayer>(layerType);
    QMap<QString, QVariant> params;

    params["startX"] = startX;
    params["startY"] = startY;
    params["width"] = width;
    params["height"] = height;
    params["mode"] = mode;
    params["imagePath"] = imagePath;

    pictureLayer->setParameters(params);
    return pictureLayer;
}

