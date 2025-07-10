#pragma once

#include <QtWidgets/QMainWindow>

class QTabWidget;  // 前向声明，减少不必要的 #include
class QWidget;
class DisplayWidget;
#include <QTabWidget>
#include <QPushButton>
#include <QWidget>
#include <QMenuBar>
#include <QToolBar>
#include <QTranslator>
#include <QApplication>
//#include "ui_MainWindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void closeEvent(QCloseEvent* event);

private:
    bool isConfigChanged = false;

    // Ui::MainWindowClass ui;
    void createTabs();
    void createMenus();

    QTabWidget* tabWidget;     // 多页面控件
    //QWidget* startTab;               // Start 页面
    //QWidget* demoTab;            // Demo 页面
    DisplayWidget* displayTab;          // Display 页面
    //QWidget* helpTab;              // Help 页面

    // 菜单栏
    QMenuBar* menuBar;

    // 功能按钮
    QAction* newAction;
    QAction* openAction;
    QAction* saveAction;
    QAction* saveAsAction;

    QMenu* settingsMenu;
    QMenu* languageMenu;
    QAction* englishAction;
    QAction* chineseAction;

    QTranslator appTranslator;  // 自己项目的翻译
    QTranslator qtTranslator;   // Qt内置翻译

    void loadLanguage(const QString& languageCode);
    void retranslateUi();

private slots:
    void newFile();
    void openFile();
    void saveFile();
    void saveAsFile();

    void updateConfigChanged(bool changed);  // 接收配置更改的信号
};
