// CheckerLayer.cpp
#include "CheckerLayer.h"
#include "common.h"
#include <QGuiApplication>
#include <QScreen>

CheckerLayer::CheckerLayer(const QString& type, bool status)
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
        {"rows", 8},
        {"cols", 8},
        {"color1", QColor(0, 0, 0, 255)},
        {"color2", QColor(255, 255, 255, 255)},
    };
}

void CheckerLayer::setParameters(const QMap<QString, QVariant>& params) {
    for (auto it = params.begin(); it != params.end(); ++it) {
        layerParams[it.key()] = it.value();
    }
}

QMap<QString, QVariant> CheckerLayer::getParameters() const {
    return layerParams;
}

//void CheckerLayer::drawLayer(QPainter& painter, bool isThumbnail = false) const {
//    if (!getLayerStatus()) return;
//
//    int startX = layerParams.value("startX").toInt();
//    int startY = layerParams.value("startY").toInt();
//    int width = layerParams.value("width").toInt(); 
//    int height = layerParams.value("height").toInt(); 
//    int rows = layerParams.value("rows").toInt();
//    int cols = layerParams.value("cols").toInt();
//    QColor color1 = layerParams.value("color1").value<QColor>();
//    QColor color2 = layerParams.value("color2").value<QColor>();
//
//    painter.setRenderHint(QPainter::Antialiasing, false);
//    painter.setBrush(Qt::NoBrush);
//    QList<QRect> regions = divideRegion(width, height, rows, cols);
//
//    for (QRect& rect : regions) {
//        if (!isThumbnail) {
//            rect.translate(startX, startY);
//        }
//        else {
//            rect.translate(0, 0);
//        }
//    }
//
//    for (int i = 0; i < regions.size(); ++i) {
//        const QRect& region = regions[i];
//        QColor fillColor = ((i / cols) % 2 == (i % cols) % 2) ? color1 : color2;
//        painter.setBrush(fillColor);
//        painter.setPen(Qt::NoPen);
//        painter.drawRect(region);
//    }
//}

void CheckerLayer::drawLayer(QPainter& painter, bool isThumbnail) const {
    if (!getLayerStatus()) return;

    int startX = layerParams.value("startX").toInt();
    int startY = layerParams.value("startY").toInt();
    int width = layerParams.value("width").toInt();
    int height = layerParams.value("height").toInt();
    int rows = layerParams.value("rows").toInt();
    int cols = layerParams.value("cols").toInt();
    QColor color1 = layerParams.value("color1").value<QColor>();
    QColor color2 = layerParams.value("color2").value<QColor>();

    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setPen(Qt::NoPen);

    QList<QRect> regions = divideRegion(width, height, rows, cols);

    //if (!isThumbnail) {
    //    for (QRect& rect : regions) {
    //        rect.translate(startX, startY);
    //    }
    //}

    for (int i = 0; i < regions.size(); ++i) {
        const QRect& region = regions[i];
        QColor fillColor = ((i / cols) % 2 == (i % cols) % 2) ? color1 : color2;
        painter.setBrush(fillColor);
        painter.drawRect(region.translated(startX, startY));
        //painter.drawRect(region);
    }
}


void CheckerLayer::outputLayerInfo() const {
    QStringList outputOrder = {
    "startX", "startY", "width", "height", "rows", "cols", "color1", "color2"
    };

    qDebug() << "\t\tCheckerLayer Information:";
    for (const QString& key : outputOrder) {
        if (layerParams.contains(key)) {
            QString valueStr = layerParams.value(key).toString();

            if (key == "color1" || key == "color2") {
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