// LineLayer.cpp
#include "LineLayer.h"
#include "common.h"
#include <QGuiApplication>
#include <QScreen>

LineLayer::LineLayer(const QString& type, bool status)
    : Layer(type, status) {
    QScreen* screen = QGuiApplication::primaryScreen();
    if (screen) {
        QRect geometry = screen->geometry();
        qreal pixelRatio = screen->devicePixelRatio();

        int width = geometry.width() * pixelRatio;
        int height = geometry.height() * pixelRatio;

        layerParams = {
        {"startX", 0},
        {"startY", 0},
        {"endX", width},
        {"endY", height},
        {"lineWidth", 1},
        {"lineColor", QColor(255, 0, 0, 255)},
        };
    }
}

void LineLayer::setParameters(const QMap<QString, QVariant>& params) {
    for (auto it = params.begin(); it != params.end(); ++it) {
        layerParams[it.key()] = it.value();
    }
}

QMap<QString, QVariant> LineLayer::getParameters() const {
    return layerParams;
}

void LineLayer::drawLayer(QPainter& painter, bool isThumbnail) const {
    if (!getLayerStatus()) return;

    int startX = layerParams.value("startX").toInt();
    int startY = layerParams.value("startY").toInt();
    int endX = layerParams.value("endX").toInt();
    int endY = layerParams.value("endY").toInt();
    /*int lineWidth = layerParams.value("lineWidth").toInt();*/
    int lineWidth = isThumbnail ? 5 : layerParams.value("lineWidth").toInt();
    QColor lineColor = layerParams.value("lineColor").value<QColor>();

    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setBrush(Qt::NoBrush);

    QPen pen(lineColor);
    pen.setWidth(lineWidth);
    painter.setPen(pen);
    painter.drawLine(startX, startY, endX, endY);
    //if (!isThumbnail) {
    //    painter.drawLine(startX, startY, endX, endY);
    //}
    //else {
    //    painter.drawLine(0, 0, endX, endY);
    //}

    //QPen pen(lineColor);
    //pen.setWidth(lineWidth);
    //painter.setPen(pen);

    //if (!isThumbnail) {
    //    painter.drawLine(startX, startY, endX, endY);
    //}
    //else {
    //    // 计算包围盒
    //    int minX = std::min(startX, endX);
    //    int minY = std::min(startY, endY);
    //    int maxX = std::max(startX, endX);
    //    int maxY = std::max(startY, endY);
    //    int origWidth = maxX - minX;
    //    int origHeight = maxY - minY;

    //    // 缩放至 thumbnail 区域，例如 320x230
    //    QSize previewSize(320, 230);
    //    qreal scaleX = previewSize.width() / static_cast<qreal>(origWidth + 10);
    //    qreal scaleY = previewSize.height() / static_cast<qreal>(origHeight + 10);
    //    qreal scale = std::min(scaleX, scaleY);

    //    // 平移后绘制，使图形居中
    //    painter.save();
    //    painter.translate((previewSize.width() - origWidth * scale) / 2.0 - minX * scale,
    //        (previewSize.height() - origHeight * scale) / 2.0 - minY * scale);
    //    painter.scale(scale, scale);

    //    painter.drawLine(startX, startY, endX, endY);

    //    painter.restore();
    //}
}

void LineLayer::outputLayerInfo() const {
    QStringList outputOrder = {
        "startX", "startY", "endX", "endY", "lineWidth", "lineColoe"
    };

    qDebug() << "\t\tLineLayer Information:";
    for (const QString& key : outputOrder) {
        if (layerParams.contains(key)) {
            QString valueStr = layerParams.value(key).toString();

            if (key == "backColor" || key == "lineColoe") {
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