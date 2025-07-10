// LayerDrawDialog.cpp
#include "LayerDrawDialog.h"
#include <QDebug>
#include <QKeyEvent>
#include <QGuiApplication>
#include <QScreen>

LayerDrawDialog::LayerDrawDialog(QWidget* parent)
    : QDialog(parent) {
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);  // ������͸��
    setStyleSheet("background-color: black;");  // ���ڱ���

    // ���ô��ڴ�СΪ��Ļ�ߴ�
    QScreen* screen = QGuiApplication::primaryScreen();
    setGeometry(screen->geometry());
}

 // ����ͼ���б�
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
    // ��ȡ��Ļ DPI ����
    qreal dpr = devicePixelRatioF();
    int width = this->width() * dpr;
    int height = this->height() * dpr;

    // ���� QImage ����ձ���
    QImage image(width, height, QImage::Format_ARGB32);
    image.fill(Qt::transparent);

    // �� QImage �ϻ���ͼ��
    QPainter imagePainter(&image);
    imagePainter.setRenderHint(QPainter::Antialiasing, false);
    for (const auto& layer : layers) {
        layer->setLayerStatus(true);
        layer->drawLayer(imagePainter);
    }

    // �� QImage ת��Ϊ QPixmap����Ӧ�� DPR
    QPixmap pixmap = QPixmap::fromImage(image);
    pixmap.setDevicePixelRatio(dpr);

    // ���Ƶ�����
    QPainter painter(this);
    painter.drawPixmap(0, 0, pixmap);
}