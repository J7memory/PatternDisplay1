#ifndef MONOLAYERCONFIGDIALOG_H
#define MONOLAYERCONFIGDIALOG_H

#include <QCheckBox>
#include <QColorDialog>
#include <QDialog>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QImage>
#include <QLabel>
#include <QMap>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QSpacerItem>
#include <QSpinBox>
#include <QStack>
#include <QVariant>
#include <QVBoxLayout>

#include "MonoLayer.h"

class MonoLayerConfigDialog : public QDialog {
    Q_OBJECT

public:
    explicit MonoLayerConfigDialog(MonoLayer* layer, QWidget* parent = nullptr);
    QMap<QString, QVariant> getLayerParameters() const;

private slots:
    void chooseSpinBox();
    void chooseRectColor();
    void updateAlphaLabel();

    void updateCheckBox(int state);

    void applyChanges();
    void previewChanges();
    void cancelChanges();

    void undoChanges();

    void updateThumbnail();

private:
    QMap<QString, QVariant> originalParams;
    MonoLayer* selectedLayer;

    QSpinBox* startXSpinBox;
    QSpinBox* startYSpinBox;
    QSpinBox* widthSpinBox;
    QSpinBox* heightSpinBox;
    QSpinBox* lineWidthSpinBox;

    QCheckBox* fillCheckBox;

    QPushButton* colorButton;
    QColor currentColor;
    QLabel* alphaLabel;

    QLabel* imageLabel;

    QPushButton* applyButton;
    QPushButton* previewButton;
    QPushButton* cancelButton;

    QPushButton* undoButton;

    void initializeSpinBox(QSpinBox*& spinBox, const QString& paramKey, int minValue, int maxValue);
    void initializeColorButton(QPushButton*& button, const QString& paramKey, QColor& color, QLabel* alphaLabel);
    void initializeAlphaLabel(QLabel*& label, QColor color);
    void initializeCheckBox(QCheckBox*& checkBox, const QString& label, const QString& paramKey);
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

#endif // MONOLAYERCONFIGDIALOG_H