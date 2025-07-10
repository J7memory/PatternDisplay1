#ifndef BITMAPLAYERCONFIGDIALOG_H
#define BITMAPLAYERCONFIGDIALOG_H

#include <QColorDialog>
#include <QDialog>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QImage>
#include <QLabel>
#include <QMap>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QSpacerItem>
#include <QSpinBox>
#include <QStack>
#include <QVariant>
#include <QVBoxLayout>

#include "BitMapLayer.h"

class ColorGridWidget;

class BitMapLayerConfigDialog : public QDialog {
    Q_OBJECT

public:
    explicit BitMapLayerConfigDialog(BitMapLayer* layer, QWidget* parent = nullptr);
    QMap<QString, QVariant> getLayerParameters() const;

private slots:
    void chooseSpinBox();
    void chooseBitMapColor();
    void updateAlphaLabel();

    void applyChanges();
    void previewChanges();
    void cancelChanges();

    void undoChanges();

    void updateThumbnail();
    
private:
    QMap<QString, QVariant> originalParams;
    BitMapLayer* selectedLayer;

    QSpinBox* startXSpinBox;
    QSpinBox* startYSpinBox;
    QSpinBox* widthSpinBox;
    QSpinBox* heightSpinBox;

    QSpinBox* bitMapWidthSpinBox;  
    QSpinBox* bitMapHeightSpinBox;

    QList<QColor> pixelColors;
    ColorGridWidget* colorGrid;

    QPushButton* colorButton;
    QColor currentColor;

    QLabel* alphaLabel;

    QPushButton* applyButton;
    QPushButton* previewButton;
    QPushButton* cancelButton;

    QPushButton* undoButton;

    void initializeSpinBox(QSpinBox*& spinBox, const QString& paramKey, int minValue, int maxValue);
    void initializeColorButton(QPushButton*& button, const QString& paramKey, QColor& color, QLabel* alphaLabel);
    void initializeAlphaLabel(QLabel*& label, QColor color);
    void initializeButton(QPushButton*& button, const QString& text, int fixedWidth, bool autoDefault);
    QVBoxLayout* createLayout();
    void connectSignals();

    void updateGridSize();
    void handlePixelClicked(int index);
    void updateControlsFromParameters();

    QStack<QMap<QString, QVariant>> undoStack;
    const int maxUndoCount = 50;
    /*void pushToUndoStack(const QMap<QString, QVariant>& params);*/
    void pushToUndoStack(const QMap<QString, QVariant>& params, const QList<QColor>& pixelColors);
};



#endif // BITMAPLAYERCONFIGDIALOG_H

