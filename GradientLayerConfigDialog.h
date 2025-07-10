#ifndef GRADIENTLAYERCONFIGDIALOG_H
#define GRADIENTLAYERCONFIGDIALOG_H

#include <QCheckBox>
#include <QColorDialog>
#include <QComboBox>
#include <QDialog>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QImage>
#include <QLabel>
#include <QListWidget>
#include <QMap>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QSpacerItem>
#include <QSpinBox>
#include <QStack>
#include <QVariant>
#include <QVBoxLayout>

#include "GradientLayer.h"

class GradientLayerConfigDialog : public QDialog {
    Q_OBJECT

public:
    explicit GradientLayerConfigDialog(GradientLayer* layer, QWidget* parent = nullptr);
    QMap<QString, QVariant> getLayerParameters() const;

private slots:
    void chooseSpinBox();
    void chooseStartColor();
    void chooseEndColor();
    void updateAlphaLabel();

    void chooseComboBox();

    void loadSelectedGradient();
    void updateGradientList();
    void saveCurrentGradient();

    void applyChanges();
    void previewChanges();
    void cancelChanges();

    void undoChanges();



    void updateThumbnail();

private:
    QMap<QString, QVariant> originalParams;
    int currentGradientIndex;
    QList<QMap<QString, QVariant>> gradientBars;
    GradientLayer* selectedLayer;
    QListWidget* gradientList;

    QSpinBox* startXSpinBox;
    QSpinBox* startYSpinBox;
    QSpinBox* widthSpinBox;
    QSpinBox* heightSpinBox;
    QSpinBox* countsSpinBox;

    QComboBox* orientationComboBox;
    QSpinBox* stepsSpinBox;

    QPushButton* startColorButton;
    QPushButton* endColorButton;

    QColor currentStartColor;
    QColor currentEndColor;

    QLabel* sCAlphaLabel;
    QLabel* eCAlphaLabel;
    
    QLabel* imageLabel;

    QPushButton* applyButton;
    QPushButton* previewButton;
    QPushButton* cancelButton;

    QPushButton* undoButton;

    void initializeSpinBox(QSpinBox*& spinBox, const QString& paramKey, int minValue, int maxValue);
    void initializeComboBox(QComboBox*& comboBox, const QStringList& items, const QString& paramKey);
    void initializeColorButton(QPushButton*& button, const QString& paramKey, QColor& color, QLabel* alphaLabel);
    void initializeAlphaLabel(QLabel*& label, QColor color);
    void initializeButton(QPushButton*& button, const QString& text, int fixedWidth, bool autoDefault);
    void initializeImageLabel(QLabel*& lable, int thumbWidth, int thumbHeight);
    QVBoxLayout* createLayout();
    void connectSignals();

    int thumbWidth = 400;
    int thumbHeight = 400;

    QStack<QMap<QString, QVariant>> undoStack;
    const int maxUndoCount = 50;
    void pushToUndoStack(const QMap<QString, QVariant>& params);
}; 

#endif // GRADIENTLAYERCONFIGDIALOG_H
