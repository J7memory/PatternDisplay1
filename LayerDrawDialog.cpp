// LayerDrawDialog.cpp
#include "LayerDrawDialog.h"
#include <QDebug>
#include <QKeyEvent>
#include <QGuiApplication>
#include <QScreen>

LayerDrawDialog::LayerDrawDialog(QWidget* parent)
    : QDialog(parent) {
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);  // 允许背景透明
    setStyleSheet("background-color: black;");  // 纯黑背景

    // 设置窗口大小为屏幕尺寸
    QScreen* screen = QGuiApplication::primaryScreen();
    setGeometry(screen->geometry());
}

 // 设置图层列表
void LayerDrawDialog::setLayers(const QVector<std::shared_ptr<Layer>>& layersList) {
    layers = layersList;
    update();
}

void LayerDrawDialog::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
    case Qt::Key_Escape:
        close();
        break;
    default:
        QDialog::keyPressEvent(event);
    }
}

void LayerDrawDialog::paintEvent(QPaintEvent* event) {
    // 获取屏幕 DPI 缩放
    qreal dpr = devicePixelRatioF();
    int width = this->width() * dpr;
    int height = this->height() * dpr;

    // 创建 QImage 并清空背景
    QImage image(width, height, QImage::Format_ARGB32);
    image.fill(Qt::transparent);

    // 在 QImage 上绘制图层
    QPainter imagePainter(&image);
    imagePainter.setRenderHint(QPainter::Antialiasing, false);
    for (const auto& layer : layers) {
        layer->setLayerStatus(true);
        layer->drawLayer(imagePainter);
    }

    // 将 QImage 转换为 QPixmap，并应用 DPR
    QPixmap pixmap = QPixmap::fromImage(image);
    pixmap.setDevicePixelRatio(dpr);

    // 绘制到窗口
    QPainter painter(this);
    painter.drawPixmap(0, 0, pixmap);
}