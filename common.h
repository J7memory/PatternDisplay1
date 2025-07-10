#include "iostream"
#include <QWidget>
#include <QPushBUtton>
#include <QList>
#include <QColor>
#include <QRect>
#include <QDebug>
#include "Layer.h"
#include <QSpinBox>
#include <QMap>
#include <QVariant>
#include <QStack>
#include <QListWidget>

QList<QRect> divideRegion(int width, int height, int rows, int cols);

QRect getRegionFromDivide(const QList<QRect>& regions, int regionIndex);

QVector<int> convertToRGBA(const QColor& color);

void chooseColor(QColor& targetColor, QPushButton* targetButton, QWidget* parent = nullptr);

QPixmap generateThumbnail(const Layer* layer, int thumbWidth, int thumbHeight);

void updateParameterWithUndo(QMap<QString, QVariant>& parameters,
    QSpinBox* spinBox,
    const QString& paramName,
    QStack<QMap<QString, QVariant>>& undoStack,
    int maxUndoCount);
