#include "ColorGridWidget.h"
#include <QPainter>
#include <QMouseEvent>

ColorGridWidget::ColorGridWidget(QWidget* parent)
    : QWidget(parent), gridWidth(8), gridHeight(8) {
    colors.resize(gridWidth * gridHeight, QColor(255, 255, 255, 255));
}

void ColorGridWidget::setGridSize(int width, int height) {
    gridWidth = width;
    gridHeight = height;
    colors.resize(gridWidth * gridHeight, QColor(255, 255, 255, 255));
    update();
}

void ColorGridWidget::setPixelColors(const QList<QColor>& newColors) {
    int expectedPixelCount = gridWidth * gridHeight;
    if (newColors.size() == expectedPixelCount) {
        colors = newColors;
        update();
    }
   /* else {
        qWarning() << "Invalid color array size, expected" << gridWidth * gridHeight << "but got" << newColors.size();
    }*/

    // 调整颜色数组的大小以匹配网格大小
    QList<QColor> adjustedColors = newColors;
    if (adjustedColors.size() < expectedPixelCount) {
        while (adjustedColors.size() < expectedPixelCount) {
            adjustedColors.append(QColor(255, 255, 255, 255));
        }
    }
    else if (adjustedColors.size() > expectedPixelCount) {
        adjustedColors = adjustedColors.mid(0, expectedPixelCount);
    }

    colors = adjustedColors;
    update();
}


QList<QColor> ColorGridWidget::getPixelColors() const {
    return colors;
}

void ColorGridWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    for (int row = 0; row < gridHeight; ++row) {
        for (int col = 0; col < gridWidth; ++col) {
            int index = row * gridWidth + col;
            QRect cellRect(col * (CELL_SIZE), row * (CELL_SIZE), CELL_SIZE, CELL_SIZE);

            painter.setBrush(colors[index]);
            painter.drawRect(cellRect);
        }
    }
}

// 处理鼠标点击事件
void ColorGridWidget::mousePressEvent(QMouseEvent* event) {
    int col = event->position().x() / CELL_SIZE;
    int row = event->position().y() / CELL_SIZE;
    if (col >= 0 && col < gridWidth && row >= 0 && row < gridHeight) {
        int index = row * gridWidth + col;
        emit pixelClicked(index);
    }
}

