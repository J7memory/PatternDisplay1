// CRectLayer.cpp
#include "CCircleLayer.h"
#include "common.h"
#include <QGuiApplication>
#include <QScreen>

CCircleLayer::CCircleLayer(const QString& type, bool status)
    : Layer(type, status) {

    QScreen* screen = QGuiApplication::primaryScreen();
    QRect geometry = screen->geometry();
    qreal pixelRatio = screen->devicePixelRatio();

    int width = geometry.width() * pixelRatio;
    int height = geometry.height() * pixelRatio;

    layerParams = {
        {"centerX", width / 2},
        {"centerY", height / 2},
        {"radiusX", width / 2},
        {"radiusY", height / 2},
        {"counts", 9},
        {"concentric", false},
        {"lineWidth", 1},
        {"color", QColor(255, 0, 0, 255)},
    };
}

void CCircleLayer::setParameters(const QMap<QString, QVariant>& params) {
    for (auto it = params.begin(); it != params.end(); ++it) {
        layerParams[it.key()] = it.value();
    }
}

QMap<QString, QVariant> CCircleLayer::getParameters() const {
    return layerParams;
}


void CCircleLayer::drawLayer(QPainter& painter, bool isThumbnail) const {
    if (!getLayerStatus()) return;

    //QScreen* screen = QGuiApplication::primaryScreen();
    //QRect geometry = screen->geometry();
    //qreal pixelRatio = screen->devicePixelRatio();

    //int width = geometry.width() * pixelRatio;
    //int height = geometry.height() * pixelRatio;

    int centerX = layerParams.value("centerX").toInt();
    int centerY = layerParams.value("centerY").toInt();
    int radiusX = layerParams.value("radiusX").toInt();
    int radiusY = layerParams.value("radiusY").toInt();
    int counts = layerParams.value("counts").toInt();
    bool concentric = layerParams.value("concentric").toBool();
    //int lineWidth = layerParams.value("lineWidth").toInt();
    int lineWidth = isThumbnail ? 5 : layerParams.value("lineWidth").toInt();
    QColor color = layerParams.value("color").value<QColor>();

    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(color, lineWidth));

    QPoint center(centerX, centerY);    // 圆心

    if (concentric) {
        // 计算半径递减步长
        double stepX = static_cast<double>(radiusX) / counts;
        double stepY = static_cast<double>(radiusY) / counts;

        for (int i = 0; i < counts; ++i) {
            double cradiusX = radiusX - i * stepX;
            double cradiusY = radiusY - i * stepY;

            double diameterX = cradiusX * 2.0;
            double diameterY = cradiusY * 2.0;

            QRectF ellipseRect(
                center.x() - cradiusX + lineWidth / 2.0,
                center.y() - cradiusY + lineWidth / 2.0,
                diameterX - lineWidth,
                diameterY - lineWidth
            );

            painter.drawEllipse(ellipseRect);
        }
    }
    else {
        // 非同心模式，绘制一个椭圆
        int x = centerX - radiusX + lineWidth / 2;
        int y = centerY - radiusY + lineWidth / 2;
        int w = radiusX * 2 - lineWidth;
        int h = radiusY * 2 - lineWidth;

        QRectF ellipseRect(x, y, w, h);
        painter.drawEllipse(ellipseRect);
    }
}

void CCircleLayer::outputLayerInfo() const {
    QStringList outputOrder = {
        "centerX", "centerY", "radiusX", "radiusY", "counts", "concentric", "lineWidth", "color"
    };

    qDebug() << "\t\tCCircleLayer Information:";
    for (const QString& key : outputOrder) {
        if (layerParams.contains(key)) {
            QString valueStr = layerParams.value(key).toString();

            if (key == "color") {
                QColor color = layerParams.value(key).value<QColor>();
                valueStr = QString("(%1, %2, %3, %4)")
                    .arg(color.red())
                    .arg(color.green())
                    .arg(color.blue())
                    .arg(color.alpha());
            }
            qDebug() << "\t\t\t" << key << ":" << valueStr;
        }
    }
}