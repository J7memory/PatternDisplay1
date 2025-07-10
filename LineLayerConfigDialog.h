#ifndef LINELAYERCONFIGDIALOG_H
#define LINELAYERCONFIGDIALOG_H

#include <QCheckBox>
#include <QColorDialog>
#include <QDialog>
#include <QDoubleSpinBox>
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

#include "LineLayer.h"

class LineLayerConfigDialog : public QDialog {
    Q_OBJECT

public:
    explicit LineLayerConfigDialog(LineLayer* layer, QWidget* parent = nullptr);
    QMap<QString, QVariant> getLayerParameters() const;

private slots:
    void chooseSpinBox();
    void chooseLineColor();
    void updateAlphaLabel();

    void applyChanges();
    void previewChanges();
    void cancelChanges();

    void undoChanges();

    void updateThumbnail();

private:
    QMap<QString, QVariant> originalParams;
    LineLayer* selectedLayer;

    QSpinBox* startXSpinBox;
    QSpinBox* startYSpinBox;
    QSpinBox* endXSpinBox;
    QSpinBox* endYSpinBox;
    QSpinBox* lineWidthSpinBox;

    QPushButton* lineColorButton;
    QColor currentLineColor;
    QLabel* lineColorAlphaLabel;

    QLabel* imageLabel;

    QPushButton* applyButton;
    QPushButton* previewButton;
    QPushButton* cancelButton;

    QPushButton* undoButton;

    void initializeSpinBox(QSpinBox*& spinBox, const QString& paramKey, int minValue, int maxValue);
    void initializeColorButton(QPushButton*& button, const QString& paramKey, QColor& color, QLabel* alphaLabel);
    void initializeAlphaLabel(QLabel*& label, QColor color);
    void initializeButton(QPushButton*& button, const QString& text, int fixedWidth, bool autoDefault);
    void initializeImageLabel(QLabel*& lable, int thumbWidth, int thumbHeight);
    QVBoxLayout* createLayout();
    void connectSignals();

    const int thumbWidth = 400;
    const int thumbHeight = 400;

    QStack<QMap<QString, QVariant>> undoStack;
    const int maxUndoCount = 50;
    void pushToUndoStack(const QMap<QString, QVariant>& params);
};

#endif // LINELAYERCONFIGDIALOG_H
