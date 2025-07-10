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

    // 计算每个分割点的 x 和 y 坐标（线性插值）
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

    // 构造每个小矩形区域
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
    int a = color.alpha();  // 获取 Alpha 通道（透明度）

    return { r, g, b, a };
}

void updateParameterWithUndo(QMap<QString, QVariant>& parameters,
    QSpinBox* spinBox,
    const QString& paramName,
    QStack<QMap<QString, QVariant>>& undoStack,
    int maxUndoCount) {
    // 更新参数
    int newValue = spinBox->value();
    parameters[paramName] = newValue;

    // 撤销栈管理
    if (undoStack.size() >= maxUndoCount) {
        undoStack.pop();  // 如果撤销栈超出最大数量，删除最旧的元素
    }
    undoStack.push(parameters);  // 将更新后的参数推入栈中
}

void chooseColor(QColor& targetColor, QPushButton* targetButton, QWidget* parent) {
    // 使用 getColor 方法，保证未调整的分量不会改变
    QColor selectedColor = QColorDialog::getColor(targetColor, parent, "choosecolor", QColorDialog::ShowAlphaChannel);

    if (selectedColor.isValid()) {
        // 仅更新发生变化的分量
        if (selectedColor != targetColor) {
            targetColor = selectedColor;
            targetButton->setStyleSheet(QString("background-color: %1").arg(targetColor.name(QColor::HexArgb)));
        }
    }
}


// 缩略图
QPixmap generateThumbnail(const Layer* layer, int thumbWidth, int thumbHeight) {
    if (!layer) return QPixmap();

    bool originalStatus = layer->getLayerStatus();  // 保存原始的状态

    // 设置 layerStatus 为 true，确保图层被绘制
    layer->setLayerStatus(true);

    QScreen* screen = QGuiApplication::primaryScreen();
    QRect geometry = screen->geometry();
    qreal pixelRatio = screen->devicePixelRatio();

    int width = geometry.width() * pixelRatio;
    int height = geometry.height() * pixelRatio;

    if (width <= 0 || height <= 0) return QPixmap();  // 避免无效尺寸

    // 创建缩略图 QPixmap
    QPixmap thumbnail(thumbWidth, thumbHeight);
    thumbnail.fill(Qt::transparent);  // 透明背景

    // 创建 QPainter 进行绘制
    QPainter painter(&thumbnail);
    painter.setRenderHint(QPainter::Antialiasing, false);

    // 计算缩放比例（保持长宽比）
    qreal scaleX = static_cast<qreal>(thumbWidth) / width;
    qreal scaleY = static_cast<qreal>(thumbHeight) / height;
    qreal scale = qMin(scaleX, scaleY);

    // 平移并缩放，使图层居中
    painter.translate((thumbWidth - width * scale) / 2, (thumbHeight - height * scale) / 2);
    painter.scale(scale, scale);

    // 在绘制区域画浅灰色背景
    painter.fillRect(QRectF(0, 0, width, height), QColor(245, 245, 245, 245));
    painter.setClipRect(QRect(0, 0, width, height));

    // 调用 `Layer` 的 `drawLayer` 方法（多态调用）
    layer->drawLayer(painter);

    layer->setLayerStatus(originalStatus);

    return thumbnail;
}
