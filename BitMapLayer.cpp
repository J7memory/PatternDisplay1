#include "BitMapLayer.h"
#include "common.h"
#include <QGuiApplication>
#include <QScreen>

BitMapLayer::BitMapLayer(const QString& type, bool status)
    : Layer(type, status) {

    QScreen* screen = QGuiApplication::primaryScreen();
    QRect geometry = screen->geometry();
    qreal pixelRatio = screen->devicePixelRatio();

    int width = geometry.width() * pixelRatio;
    int height = geometry.height() * pixelRatio;
    int bitMapWidth = 4;
    int bitMapHeight = 4;

    layerParams = {
        {"startX", 0},
        {"startY", 0},
        {"width", width},
        {"height", height},
        {"bitMapWidth", bitMapWidth},
        {"bitMapHeight", bitMapHeight},
        {"color", QColor(255, 0, 0, 255)},
    };

     pixelColors.clear();
    for (int i = 0; i < bitMapWidth * bitMapHeight; ++i) {
        pixelColors.append(QColor(255, 255, 255, 255));
    }
    layerParams["pixelColors"] = QVariant::fromValue<QList<QColor>>(pixelColors);
}

void BitMapLayer::setParameters(const QMap<QString, QVariant>& params) {
    for (auto it = params.begin(); it != params.end(); ++it) {
        if (it.key() == "pixelColors") {
            QList<QColor> colors = it.value().value<QList<QColor>>();
            layerParams[it.key()] = QVariant::fromValue(colors);
        }
        else {
            layerParams[it.key()] = it.value();
        }
    }
}

QMap<QString, QVariant> BitMapLayer::getParameters() const {
    return layerParams;
}

void BitMapLayer::drawLayer(QPainter& painter, bool isThumbnail) const {
    if (!getLayerStatus()) return;

    int startX = layerParams.value("startX").toInt();
    int startY = layerParams.value("startY").toInt();
    int width = layerParams.value("width").toInt();
    int height = layerParams.value("height").toInt();
    int bitMapWidth = layerParams.value("bitMapWidth").toInt();
    int bitMapHeight = layerParams.value("bitMapHeight").toInt();
    QColor color = layerParams.value("color").value<QColor>();
    QList<QColor> pixelColors = layerParams.value("pixelColors").value<QList<QColor>>();

    painter.save();
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setBrush(Qt::NoBrush);


    //if (!isThumbnail) {
    //    painter.translate(startX, startY);
    //}
    //else
    //{
    //    painter.translate(0, 0);
    //}
    
    // 1. 生成位图
    QImage bitmapImage(bitMapWidth, bitMapHeight, QImage::Format_ARGB32);
    for (int y = 0; y < bitMapHeight; ++y) {
        for (int x = 0; x < bitMapWidth; ++x) {
            QColor color = pixelColors[y * bitMapWidth + x];
            bitmapImage.setPixelColor(x, y, color);
        }
    }

    QPixmap bitmapPixmap = QPixmap::fromImage(bitmapImage);
    painter.drawTiledPixmap(startX, startY, width, height, bitmapPixmap);

    painter.restore();
}


void BitMapLayer::outputLayerInfo() const {
    QStringList outputOrder = {
        "startX", "startY", "width", "height", "bitMapWidth", "bitMapHeight", "color"
    };

    qDebug() << "\t\tBitMapLayer Information:";
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

    // 打印 pixelColors 信息
    QList<QColor> pixelColors = layerParams.value("pixelColors").value<QList<QColor>>();
    int bitMapWidth = layerParams.value("bitMapWidth").toInt();
    int bitMapHeight = layerParams.value("bitMapHeight").toInt();

    qDebug() << "\t\t\tPixel Colors:";
    for (int y = 0; y < bitMapHeight; ++y) {
        for (int x = 0; x < bitMapWidth; ++x) {
            QColor color = pixelColors[y * bitMapWidth + x];
            qDebug() << "\t\t\t\tpixelColors (" << x << ", " << y << ") (RGBA) : ("
                << color.red() << ", "
                << color.green() << ", "
                << color.blue() << ", "
                << color.alpha() << ")";
        }
    }
}
