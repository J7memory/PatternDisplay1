#include "MonoLayer.h"
#include "common.h"
#include "DisplayWidget.h"
#include <QGuiApplication>
#include <QScreen>
#include <QDebug>


MonoLayer::MonoLayer(const QString& type, bool status)
    : Layer(type, status) {

    QScreen* screen = QGuiApplication::primaryScreen();
    QRect geometry = screen->geometry();
    qreal pixelRatio = screen->devicePixelRatio();

    int width = geometry.width() * pixelRatio;
    int height = geometry.height() * pixelRatio;

    layerParams = {
        {"startX", 0},
        {"startY", 0},
        {"width", width},
        {"height", height},
        {"lineWidth", 1},
        {"fill", true},
        {"color", QColor(255, 0, 0, 255)}
    };
}

void MonoLayer::setParameters(const QMap<QString, QVariant>& params) {
    for (auto it = params.begin(); it != params.end(); ++it) {
        layerParams[it.key()] = it.value();
    }
}

QMap<QString, QVariant> MonoLayer::getParameters() const {
    return layerParams;
}

void MonoLayer::drawLayer(QPainter& painter, bool isThumbnail) const {
    if (!getLayerStatus()) return;

    int startX = layerParams.value("startX").toInt();
    int startY = layerParams.value("startY").toInt();
    int width = layerParams.value("width").toInt();
    int height = layerParams.value("height").toInt();
    int lineWidth = layerParams.value("lineWidth").toInt();
    bool fill = layerParams.value("fill").toBool();
    QColor color = layerParams.value("color").value<QColor>();

    painter.save();
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setBrush(Qt::NoBrush);

    //if (!isThumbnail) {
    //    painter.translate(startX, startY);
    //}

    if (fill) {
        painter.setPen(Qt::NoPen);
        painter.setBrush(color);
        QRectF rect(startX, startY, width, height);
        painter.drawRect(rect);
    }
    else {
        QPen pen(color, lineWidth, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin);
        painter.setPen(pen);
        QRectF rect(startX + lineWidth / 2.0, startY + lineWidth / 2.0, width - lineWidth, height - lineWidth);
        painter.drawRect(rect);
    }

    painter.restore();
}

void MonoLayer::outputLayerInfo() const {
    QStringList outputOrder = {
        "startX", "startY", "width", "height", "fill", "lineWidth", "color"
    };

    qDebug() << "\t\tMonoLayer Information:";
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