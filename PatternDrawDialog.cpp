// PatternDrawDialog.cpp
#include "PatternDrawDialog.h"
#include <QDebug>
#include <QKeyEvent>
#include <QGuiApplication>
#include <QScreen>

PatternDrawDialog::PatternDrawDialog(std::shared_ptr<Pattern> pattern, QWidget* parent)
    : QDialog(parent), pattern(pattern) {
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);  // 允许背景透明
    setStyleSheet("background-color: black;");  // 纯黑背景

    // 设置窗口大小为屏幕尺寸
    QScreen* screen = QGuiApplication::primaryScreen();
    setGeometry(screen->geometry());
}

void PatternDrawDialog::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape) {
        close();
    }
    else {
        QDialog::keyPressEvent(event);
    }
}

void PatternDrawDialog::updatePattern(std::shared_ptr<Pattern> newPattern)
{
    pattern = newPattern;
    update();  // 触发重绘
}

void PatternDrawDialog::paintEvent(QPaintEvent* event) {
    QPainter painter(this);

    // 先判断是不是空图层pattern
    if (!pattern || pattern->getLayers().isEmpty()) {
        // 画纯黑背景，保证窗口显示黑色
        painter.fillRect(rect(), Qt::black);
        return;
    }
    // 获取屏幕 DPI 缩放
    qreal dpr = devicePixelRatioF();

    // 获取pattern的宽度和高度
    int width = pattern->getPatternWidth();
    int height = pattern->getPatternHeight();

    // 创建 QImage 并清空背景
    QImage image(width, height, QImage::Format_ARGB32);
    image.fill(QColor(0, 0, 0, 255)); // 设定Pattern的默认背景为不透明的黑色

    // 在 QImage 上绘制图层
    QPainter imagePainter(&image);
    imagePainter.setRenderHint(QPainter::Antialiasing, false);

    // 遍历Pattern中的所有Layer，按顺序绘制
    for (const auto& layer : pattern->getLayers()) {
        if (layer->getLayerStatus()) {
            QImage layerImage(width, height, QImage::Format_ARGB32);
            layerImage.fill(Qt::transparent);  // 设置透明背景

            QPainter layerPainter(&layerImage);
            layer->drawLayer(layerPainter);
            layerPainter.end();

            // 设置混合模式（CompositionMode_SourceOver：覆盖/ CompositionMode_Multiply：正片叠底/ CompositionMode_Screen：滤色/ CompositionMode_Overlay：增强对比度）
            imagePainter.setCompositionMode(QPainter::CompositionMode_SourceOver);
            imagePainter.drawImage(0, 0, layerImage);  // 叠加到最终图像上
        }
    }

    imagePainter.end();

    // 将 QImage 转换为 QPixmap，并应用 DPR
    QPixmap pixmap = QPixmap::fromImage(image);
    pixmap.setDevicePixelRatio(dpr);

    // 绘制到窗口
    painter.drawPixmap(0, 0, pixmap);
}
