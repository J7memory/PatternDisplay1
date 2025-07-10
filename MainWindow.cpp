#include "MainWindow.h"
#include "PatternManager.h"
#include "DisplayWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QIcon>
#include <QFileDialog>
#include <QCloseEvent>
#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // ui.setupUi(this);
    setWindowTitle(tr("PatternDisplay"));
    setWindowIcon(QIcon("./icons/cc.ico"));
    setFixedSize(800, 500);

    createMenus();

    // 创建中央控件和布局
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    // 创建 QTabWidget 并设置 Tab 在左侧
    tabWidget = new QTabWidget(this);
    tabWidget->setTabPosition(QTabWidget::West);  // Tab 在左侧
    mainLayout->addWidget(tabWidget);

    // 创建选项卡
    createTabs();

    // 连接 DisplayWidget 的信号
    if (displayTab) {
        connect(displayTab, &DisplayWidget::configChanged, this, &MainWindow::updateConfigChanged);
    }

     // 加载语言文件
    loadLanguage("zh_CN");
}

MainWindow::~MainWindow()
{
    /*displayTab->saveConfiguration();*/
}

void MainWindow::closeEvent(QCloseEvent* event) {
    // 无改动时直接关闭
    if (!isConfigChanged) {
        event->accept();
        return;
    }

    // 有改动时弹出保存提示
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        tr("Close"),
        tr("Do you want to save the current configuration?"),
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel
    );

    if (reply == QMessageBox::Yes) {
        // 如果是临时文件（路径为空），另存为
        if (displayTab->currentConfigPath.isEmpty()) {
            saveAsFile();
        }
        else {
            displayTab->saveConfigurationBinaryPro(displayTab->currentConfigPath);
        }
        event->accept();
    }
    else if (reply == QMessageBox::No) {
        event->accept();
    }
    else if (reply == QMessageBox::Cancel) {
        event->ignore();
    }
}

void MainWindow::createMenus() {
    // 菜单栏
    menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    newAction = new QAction(tr("New"), this);
    openAction = new QAction(tr("Open"), this);
    saveAction = new QAction(tr("Save"), this);
    saveAsAction = new QAction(tr("Save As..."), this);

    menuBar->addAction(newAction);
    menuBar->addAction(openAction);
    menuBar->addAction(saveAction);
    menuBar->addAction(saveAsAction);

    // 创建 语言设置 子菜单
    settingsMenu = new QMenu(tr("Settings"), this);
    menuBar->addMenu(settingsMenu);

    // 创建语言设置子菜单
    languageMenu = new QMenu(tr("Language Settings"), this);
    englishAction = new QAction(tr("English"), this);
    chineseAction = new QAction(tr("Chinese"), this);

    languageMenu->addAction(englishAction);
    languageMenu->addAction(chineseAction);

    settingsMenu->addMenu(languageMenu);

    // 连接信号槽
    connect(newAction, &QAction::triggered, this, &MainWindow::newFile);
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);
    connect(saveAsAction, &QAction::triggered, this, &MainWindow::saveAsFile);

    connect(englishAction, &QAction::triggered, this, [this]() { loadLanguage("en_US"); });
    connect(chineseAction, &QAction::triggered, this, [this]() { loadLanguage("zh_CN"); });
}

void MainWindow::createTabs()
{
    //startTab = new QWidget(this);
    //demoTab = new QWidget(this);
    displayTab = new DisplayWidget(this);
    tabWidget->addTab(displayTab, tr("Display"));
    tabWidget->tabBar()->hide(); // 隐藏Tab标签
    tabWidget->setStyleSheet("QTabWidget::pane { border: 1px solid gray; }");
    //helpTab = new QWidget(this);

    //tabWidget->addTab(startTab, QIcon(":/icons/start.png"), "Start");
    //tabWidget->addTab(demoTab, QIcon(":/icons/demo.png"), "Demo");
    //tabWidget->addTab(displayTab, QIcon(":/icons/display.png"), "Display");
    //tabWidget->addTab(helpTab, QIcon(":/icons/help.png"), "Help");

    // displayTab->loadConfiguration();  // 只需调用一次加载配置
    displayTab->loadConfigurationBinaryPro("");

    // 设置简单的样式
     tabWidget->setStyleSheet("QTabWidget::pane { border: 1px solid gray; }");
}

void MainWindow::updateConfigChanged(bool changed)
{
    isConfigChanged = changed;  // 更新配置是否改变的标志
}

void MainWindow::newFile()
{
    // 如果配置没有改变，不需要弹出保存提示
    if (!isConfigChanged) {
        // 直接新建文件
        /*QMessageBox::information(this, tr("New File"), tr("Created a new temporary configuration."));*/

        // 清空当前 DisplayWidget 的所有 Pattern 数据
        if (displayTab) {
            displayTab->currentConfigPath.clear();
            displayTab->clearAllPatterns();
            displayTab->updatePatternListView();
            displayTab->patternPreview->clear();
        }
    }
    else {
        // 配置改变，先确认是否保存当前配置
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("New File"), tr("Do you want to save the current configuration?"),
            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (reply == QMessageBox::Cancel)
            return;
        if (reply == QMessageBox::No)
            if (displayTab) {
                displayTab->currentConfigPath.clear();
                displayTab->clearAllPatterns();
                displayTab->updatePatternListView();
                displayTab->patternPreview->clear();
            }
            //return;
        if (reply == QMessageBox::Yes)
            if (displayTab->currentConfigPath.isEmpty()) {
                saveAsFile();
            }
            else {
                saveFile();
            }
            
    }

    isConfigChanged = false;  // 清空文件后，将标志设置为 false
}

void MainWindow::openFile() {
    // 如果配置有更改，询问是否保存
    if (isConfigChanged) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Open File"), tr("Do you want to save the current configuration?"),
            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (reply == QMessageBox::Cancel)
            return;
        if (reply == QMessageBox::Yes)
            saveFile();
    }

    // 打开文件对话框，选择要打开的文件
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open Configuration File"), "", "DAT Files (*.dat)");

    if (filePath.isEmpty()) {
        return; 
    }

    displayTab->loadConfigurationBinaryPro(filePath);

    isConfigChanged = false;
}

void MainWindow::saveFile() {
    if (displayTab->currentConfigPath.isEmpty()) {
        // 如果路径为空，则执行另存为
        saveAsFile();
        return;
    }
    else {
        // 当前路径不为空，尝试直接保存
        bool success = displayTab->saveConfigurationBinaryPro(displayTab->currentConfigPath);

        if (success) {
            QMessageBox::information(this, tr("Save File"), tr("Configuration saved successfully."));
            isConfigChanged = false;
        }
        else {
            QMessageBox::warning(this, tr("Save File"), tr("Failed to save configuration."));
        }
    }

    isConfigChanged = false;
}

void MainWindow::saveAsFile() {
    // 默认保存文件名
    QString defaultFileName = "config.dat";

    QString filePath = QFileDialog::getSaveFileName(this, tr("Save As..."), defaultFileName, "Dat Files (*.dat)");

    if (filePath.isEmpty()) {
        return;
    }

    if (!filePath.endsWith(".dat", Qt::CaseInsensitive)) {
        filePath.append(".dat");
    }

    // 创建并打开文件
    QFile configFile(filePath);
    if (configFile.open(QIODevice::WriteOnly)) {
        configFile.close();  // 关闭文件句柄
        displayTab->saveConfigurationBinaryPro(filePath);
        QMessageBox::information(this, tr("Save as"), tr("Configuration saved successfully."));
        displayTab->loadConfigurationBinaryPro(filePath);
    }
    else {
        QMessageBox::warning(this, tr("Save as"), tr("Failed to create configuration file."));
    }

    isConfigChanged = false;
}

// 加载语言文件
void MainWindow::loadLanguage(const QString& languageCode) {
    // 先移除已有的翻译器
    qApp->removeTranslator(&appTranslator);
    qApp->removeTranslator(&qtTranslator);

     // 加载自己的翻译文件
    QString appQmFilePath = QString("./translations/%1.qm").arg(languageCode);
    if (appTranslator.load(appQmFilePath)) {
        qApp->installTranslator(&appTranslator);
    }

    // 加载 Qt 自带翻译 qt_zh_CN.qm
    QString qtQmFilePath = QLibraryInfo::path(QLibraryInfo::TranslationsPath) + QString("/qt_%1.qm").arg(languageCode);
    if (qtTranslator.load(qtQmFilePath)) {
        qApp->installTranslator(&qtTranslator);
    }

    // 更新界面文本
    retranslateUi();
}

void MainWindow::retranslateUi() {
    setWindowTitle(tr("PatternDisplay"));

    if (newAction) newAction->setText(tr("New"));
    if (openAction) openAction->setText(tr("Open"));
    if (saveAction) saveAction->setText(tr("Save"));
    if (saveAsAction) saveAsAction->setText(tr("Save As..."));

    if (settingsMenu) settingsMenu->setTitle(tr("Settings"));
    if (languageMenu) languageMenu->setTitle(tr("Language Settings"));
    if (englishAction) englishAction->setText(tr("English"));
    if (chineseAction) chineseAction->setText(tr("Chinese"));

    // 更新 tab 页标题
    if (tabWidget) {
        int index = tabWidget->indexOf(displayTab);
        if (index != -1) {
            tabWidget->setTabText(index, tr("Display"));  // 这里是你想翻译的tab名字
        }
    }

    if (displayTab) {
        displayTab->retranslateUi();
    }
}