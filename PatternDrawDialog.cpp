// PatternDrawDialog.cpp
#include "PatternDrawDialog.h"
#include <QDebug>
#include <QKeyEvent>
#include <QGuiApplication>
#include <QScreen>

PatternDrawDialog::PatternDrawDialog(std::shared_ptr<Pattern> pattern, QWidget* parent)
    : QDialog(parent), pattern(pattern) {
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);  // ������͸��
    setStyleSheet("background-color: black;");  // ���ڱ���

    // ���ô��ڴ�СΪ��Ļ�ߴ�
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
    update();  // �����ػ�
}

void PatternDrawDialog::paintEvent(QPaintEvent* event) {
    QPainter painter(this);

    // ���ж��ǲ��ǿ�ͼ��pattern
    if (!pattern || pattern->getLayers().isEmpty()) {
        // �����ڱ�������֤������ʾ��ɫ
        painter.fillRect(rect(), Qt::black);
        return;
    }
    // ��ȡ��Ļ DPI ����
    qreal dpr = devicePixelRatioF();

    // ��ȡpattern�Ŀ�Ⱥ͸߶�
    int width = pattern->getPatternWidth();
    int height = pattern->getPatternHeight();

    // ���� QImage ����ձ���
    QImage image(width, height, QImage::Format_ARGB32);
    image.fill(QColor(0, 0, 0, 255)); // �趨Pattern��Ĭ�ϱ���Ϊ��͸���ĺ�ɫ

    // �� QImage �ϻ���ͼ��
    QPainter imagePainter(&image);
    imagePainter.setRenderHint(QPainter::Antialiasing, false);

    // ����Pattern�е�����Layer����˳�����
    for (const auto& layer : pattern->getLayers()) {
        if (layer->getLayerStatus()) {
            QImage layerImage(width, height, QImage::Format_ARGB32);
            layerImage.fill(Qt::transparent);  // ����͸������

            QPainter layerPainter(&layerImage);
            layer->drawLayer(layerPainter);
            layerPainter.end();

            // ���û��ģʽ��CompositionMode_SourceOver������/ CompositionMode_Multiply����Ƭ����/ CompositionMode_Screen����ɫ/ CompositionMode_Overlay����ǿ�Աȶȣ�
            imagePainter.setCompositionMode(QPainter::CompositionMode_SourceOver);
            imagePainter.drawImage(0, 0, layerImage);  // ���ӵ�����ͼ����
        }
    }

    imagePainter.end();

    // �� QImage ת��Ϊ QPixmap����Ӧ�� DPR
    QPixmap pixmap = QPixmap::fromImage(image);
    pixmap.setDevicePixelRatio(dpr);

    // ���Ƶ�����
    painter.drawPixmap(0, 0, pixmap);
}
