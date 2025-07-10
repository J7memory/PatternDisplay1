#include "iostream"
#include "common.h"
#include <QColorDialog>
#include <QPixmap>
#include <QPainter>
#include <QGuiApplication>
#include <QScreen>
#include <QList>
#include <QRect>
#include <QVector>
#include <cmath>  // std::round

QList<QRect> divideRegion(int width, int height, int rows, int cols) {
    QList<QRect> regions;

    // ����ÿ���ָ��� x �� y ���꣨���Բ�ֵ��
    QVector<int> xCoords(cols + 1);
    QVector<int> yCoords(rows + 1);

    for (int i = 0; i <= cols; ++i) {
        double ratio = static_cast<double>(i) / cols;
        xCoords[i] = static_cast<int>(std::round(ratio * width));
    }

    for (int i = 0; i <= rows; ++i) {
        double ratio = static_cast<double>(i) / rows;
        yCoords[i] = static_cast<int>(std::round(ratio * height));
    }

    // ����ÿ��С��������
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            int x = xCoords[col];
            int y = yCoords[row];
            int w = xCoords[col + 1] - xCoords[col];
            int h = yCoords[row + 1] - yCoords[row];
            regions.append(QRect(x, y, w, h));
        }
    }

    return regions;
}


QRect getRegionFromDivide(const QList<QRect>& regions, int regionIndex) {
    if (regionIndex < 0 || regionIndex >= regions.size()) {
        qDebug() << "Invalid region index";
        return QRect();
    }
    return regions[regionIndex];
}

QVector<int> convertToRGBA(const QColor& color) {
    int r = color.red();
    int g = color.green();
    int b = color.blue();
    int a = color.alpha();  // ��ȡ Alpha ͨ����͸���ȣ�

    return { r, g, b, a };
}

void updateParameterWithUndo(QMap<QString, QVariant>& parameters,
    QSpinBox* spinBox,
    const QString& paramName,
    QStack<QMap<QString, QVariant>>& undoStack,
    int maxUndoCount) {
    // ���²���
    int newValue = spinBox->value();
    parameters[paramName] = newValue;

    // ����ջ����
    if (undoStack.size() >= maxUndoCount) {
        undoStack.pop();  // �������ջ�������������ɾ����ɵ�Ԫ��
    }
    undoStack.push(parameters);  // �����º�Ĳ�������ջ��
}

void chooseColor(QColor& targetColor, QPushButton* targetButton, QWidget* parent) {
    // ʹ�� getColor ��������֤δ�����ķ�������ı�
    QColor selectedColor = QColorDialog::getColor(targetColor, parent, "choosecolor", QColorDialog::ShowAlphaChannel);

    if (selectedColor.isValid()) {
        // �����·����仯�ķ���
        if (selectedColor != targetColor) {
            targetColor = selectedColor;
            targetButton->setStyleSheet(QString("background-color: %1").arg(targetColor.name(QColor::HexArgb)));
        }
    }
}


// ����ͼ
QPixmap generateThumbnail(const Layer* layer, int thumbWidth, int thumbHeight) {
    if (!layer) return QPixmap();

    bool originalStatus = layer->getLayerStatus();  // ����ԭʼ��״̬

    // ���� layerStatus Ϊ true��ȷ��ͼ�㱻����
    layer->setLayerStatus(true);

    QScreen* screen = QGuiApplication::primaryScreen();
    QRect geometry = screen->geometry();
    qreal pixelRatio = screen->devicePixelRatio();

    int width = geometry.width() * pixelRatio;
    int height = geometry.height() * pixelRatio;

    if (width <= 0 || height <= 0) return QPixmap();  // ������Ч�ߴ�

    // ��������ͼ QPixmap
    QPixmap thumbnail(thumbWidth, thumbHeight);
    thumbnail.fill(Qt::transparent);  // ͸������

    // ���� QPainter ���л���
    QPainter painter(&thumbnail);
    painter.setRenderHint(QPainter::Antialiasing, false);

    // �������ű��������ֳ���ȣ�
    qreal scaleX = static_cast<qreal>(thumbWidth) / width;
    qreal scaleY = static_cast<qreal>(thumbHeight) / height;
    qreal scale = qMin(scaleX, scaleY);

    // ƽ�Ʋ����ţ�ʹͼ�����
    painter.translate((thumbWidth - width * scale) / 2, (thumbHeight - height * scale) / 2);
    painter.scale(scale, scale);

    // �ڻ�������ǳ��ɫ����
    painter.fillRect(QRectF(0, 0, width, height), QColor(245, 245, 245, 245));
    painter.setClipRect(QRect(0, 0, width, height));

    // ���� `Layer` �� `drawLayer` ��������̬���ã�
    layer->drawLayer(painter);

    layer->setLayerStatus(originalStatus);

    return thumbnail;
}
