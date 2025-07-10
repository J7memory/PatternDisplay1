#ifndef PICTURELAYERCONFIGDIALOG_H
#define PICTURELAYERCONFIGDIALOG_H

#include <QDialog>
#include <QMap>
#include <QVariant>
#include <QSpinBox>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QStack>

#include "PictureLayer.h"

class PictureLayerConfigDialog : public QDialog {
    Q_OBJECT

public:
    explicit PictureLayerConfigDialog(PictureLayer* layer, QWidget* parent = nullptr);
    QMap<QString, QVariant> getLayerParameters() const;

private slots:
    void chooseImage();
    void chooseSpinBox();
    void chooseComboBox();

    //void savePicture();
    void applyChanges();
    void previewChanges();
    void cancelChanges();

    void undoChanges();

    void updateThumbnail();

private:
    PictureLayer* selectedLayer;
    QMap<QString, QVariant> originalParams;

    QSpinBox* startXSpinBox;
    QSpinBox* startYSpinBox;
    QSpinBox* widthSpinBox;
    QSpinBox* heightSpinBox;

    QComboBox* modeComboBox;
    QLineEdit* imagePathLineEdit;

    QLabel* imageLabel;

    QPushButton* browseButton;
    //QPushButton* saveButton;
    QPushButton* applyButton;
    QPushButton* previewButton;
    QPushButton* cancelButton;

    QPushButton* undoButton;

    void initializeLineEdit(QLineEdit*& lineEdit, const QString& initialText, bool readOnly);
    void initializeSpinBox(QSpinBox*& spinBox, const QString& paramKey, int minValue, int maxValue);
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

#endif // PICTURELAYERCONFIGDIALOG_H

