#ifndef CRECTLAYERCONFIGDIALOG_H
#define CRECTLAYERCONFIGDIALOG_H

#include <QDialog>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QStack>
#include <QColorDialog>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QImage>
#include <QMap>
#include <QPainter>
#include <QPixmap>
#include <QSpacerItem>
#include <QVariant>
#include <QVBoxLayout>

#include "CRectLayer.h"

class CRectLayerConfigDialog : public QDialog {
    Q_OBJECT

public:
    explicit CRectLayerConfigDialog(CRectLayer* layer, QWidget* parent = nullptr);
    QMap<QString, QVariant> getLayerParameters() const;

private slots:
    void chooseSpinBox();
    void chooseColor1();
    void chooseColor2();
    void updateAlphaLabel();

    void updateCheckBox(int state);

    void applyChanges();
    void previewChanges();
    void cancelChanges();

    void undoChanges();
    
    void updateThumbnail();

private:
    QMap<QString, QVariant> originalParams;
    CRectLayer* selectedLayer;

    QSpinBox* startXSpinBox;
    QSpinBox* startYSpinBox;
    QSpinBox* widthSpinBox;
    QSpinBox* heightSpinBox;

    QSpinBox* countsSpinBox;

    QCheckBox* fillCheckBox;

    QPushButton* color1Button;
    QPushButton* color2Button;

    QColor currentColor1;
    QColor currentColor2;

    QLabel* color1AlphaLabel;
    QLabel* color2AlphaLabel;

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

#endif // CRECTLAYERCONFIGDIALOG_H

