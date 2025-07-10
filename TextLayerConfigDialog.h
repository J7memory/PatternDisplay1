#ifndef TEXTLAYERCONFIGDIALOG_H
#define TEXTLAYERCONFIGDIALOG_H

#include <QCheckBox>
#include <QColorDialog>
#include <QComboBox>
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
#include <QTextEdit>
#include <QVariant>
#include <QVBoxLayout>

#include "TextLayer.h"

class TextLayerConfigDialog : public QDialog {
    Q_OBJECT

public:
    explicit TextLayerConfigDialog(TextLayer* layer, QWidget* parent = nullptr);
    QMap<QString, QVariant> getLayerParameters() const;

private slots:
    void chooseSpinBox();
    void chooseTextColor();
    void updateAlphaLabel();
    void chooseComboBox();
    void chooseText();

    void applyChanges();
    void previewChanges();
    void cancelChanges();

    void undoChanges();

    void updateThumbnail();

private:
    QMap<QString, QVariant> originalParams;
    TextLayer* selectedLayer;

    QSpinBox* startXSpinBox;
    QSpinBox* startYSpinBox;

    QTextEdit* textTextEdit;
    QComboBox* fontComboBox;
    QSpinBox* sizeSpinBox;
    QComboBox* styleComboBox;
    
    QPushButton* textColorButton;
    QColor currentTextColor;
    QLabel* textAlphaLabel;

    QLabel* imageLabel;

    QPushButton* applyButton;
    QPushButton* previewButton;
    QPushButton* cancelButton;

    QPushButton* undoButton;

    void initializeSpinBox(QSpinBox*& spinBox, const QString& paramKey, int minValue, int maxValue);
    void initializeTextEdit(QTextEdit*& textEdit, const QString& paramKey);
    void initializeColorButton(QPushButton*& button, const QString& paramKey, QColor& color, QLabel* alphaLabel);
    void initializeAlphaLabel(QLabel*& label, QColor color);
    void initializeComboBox(QComboBox*& comboBox, const QStringList& items, const QString& paramKey);
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

#endif // TEXTLAYERCONFIGDIALOG_H
