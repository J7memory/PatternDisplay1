#pragma once

#include <QtWidgets/QMainWindow>

class QTabWidget;  // ǰ�����������ٲ���Ҫ�� #include
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

    QTabWidget* tabWidget;     // ��ҳ��ؼ�
    //QWidget* startTab;               // Start ҳ��
    //QWidget* demoTab;            // Demo ҳ��
    DisplayWidget* displayTab;          // Display ҳ��
    //QWidget* helpTab;              // Help ҳ��

    // �˵���
    QMenuBar* menuBar;

    // ���ܰ�ť
    QAction* newAction;
    QAction* openAction;
    QAction* saveAction;
    QAction* saveAsAction;

    QMenu* settingsMenu;
    QMenu* languageMenu;
    QAction* englishAction;
    QAction* chineseAction;

    QTranslator appTranslator;  // �Լ���Ŀ�ķ���
    QTranslator qtTranslator;   // Qt���÷���

    void loadLanguage(const QString& languageCode);
    void retranslateUi();

private slots:
    void newFile();
    void openFile();
    void saveFile();
    void saveAsFile();

    void updateConfigChanged(bool changed);  // �������ø��ĵ��ź�
};
