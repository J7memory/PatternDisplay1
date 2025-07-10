// CRectLayer.cpp
#include "CRectLayer.h"
#include "common.h"
#include <QGuiApplication>
#include <QScreen>

CRectLayer::CRectLayer(const QString& type, bool status)
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
        {"counts", 9},
        {"fill", true},
        {"color1", QColor(0, 0, 0, 255)},
        {"color2", QColor(255, 255, 255, 255)},
    };
}

void CRectLayer::setParameters(const QMap<QString, QVariant>& params) {
    for (auto it = params.begin(); it != params.end(); ++it) {
        layerParams[it.key()] = it.value();
    }
}

QMap<QString, QVariant> CRectLayer::getParameters() const {
    return layerParams;
}

void CRectLayer::drawLayer(QPainter& painter, bool isThumbnail) const {
    if (!getLayerStatus()) return;

    int startX = layerParams.value("startX").toInt();
    int startY = layerParams.value("startY").toInt();
    int width = layerParams.value("width").toInt();
    int height = layerParams.value("height").toInt();
    int counts = layerParams.value("counts").toInt();
    bool fill = layerParams.value("fill").toBool();
    QColor color1 = layerParams.value("color1").value<QColor>();
    QColor color2 = layerParams.value("color2").value<QColor>();
    
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing, false);

    //if (!isThumbnail) {
    //    painter.translate(startX, startY);
    //}

    // 中心点坐标
    int centerX = startX + width / 2;
    int centerY = startY + height / 2;

    for (int i = 0; i < counts; ++i) {
        double ratio = static_cast<double>(i) / (counts - 1);
        double scale = static_cast<double>(counts - i) / counts;

        // 计算每个颜色通道的渐变
        int r = color1.red() * (1 - ratio) + color2.red() * ratio;
        int g = color1.green() * (1 - ratio) + color2.green() * ratio;
        int b = color1.blue() * (1 - ratio) + color2.blue() * ratio;
        int a = color1.alpha() * (1 - ratio) + color2.alpha() * ratio;
        // int a = qBound(0, static_cast<int>(color1.alpha() * (1 - ratio) + color2.alpha() * ratio), 255);

        QColor rectColor(r, g, b, a);

        int rectWidth = qRound(width * scale);
        int rectHeight = qRound(height * scale);

        // 计算矩形的左上角坐标，确保矩形始终以固定中心点为中心
        int x = centerX - rectWidth / 2;
        int y = centerY - rectHeight / 2;

        if (fill) {
            painter.setBrush(rectColor);
            painter.setPen(Qt::NoPen);
            painter.drawRect(x, y, rectWidth, rectHeight);
        }
        else {
            painter.setPen(QPen(rectColor));
            painter.setBrush(Qt::NoBrush);
            painter.drawRect(x, y, rectWidth - 1, rectHeight - 1);
        }
    }

    painter.restore();
}

void CRectLayer::outputLayerInfo() const {
    QStringList outputOrder = {
        "startX", "startY", "width", "height", "counts", "color1", "color2"
    };

    qDebug() << "\t\tCRectLayer Information:";
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

    // 额外输出每个矩形的颜色和大小
    int width = layerParams.value("width").toInt();
    int height = layerParams.value("height").toInt();
    int counts = layerParams.value("counts").toInt();
    QColor color1 = layerParams.value("color1").value<QColor>();
    QColor color2 = layerParams.value("color2").value<QColor>();

    int centerX = width / 2;
    int centerY = height / 2;

    qDebug() << "\t\t\t Rectangles:";
    for (int i = 0; i < counts; ++i) {
        double ratio = static_cast<double>(i) / (counts - 1);

        // 计算渐变颜色
        int r = color1.red() * (1 - ratio) + color2.red() * ratio;
        int g = color1.green() * (1 - ratio) + color2.green() * ratio;
        int b = color1.blue() * (1 - ratio) + color2.blue() * ratio;
        int a = color1.alpha() * (1 - ratio) + color2.alpha() * ratio;
        //int a = qBound(0, static_cast<int>(color1.alpha() * (1 - ratio) + color2.alpha() * ratio), 255);

        // 计算矩形尺寸
        int rectWidth = width * (counts - i) / counts;
        int rectHeight = height * (counts - i) / counts;

        // 计算矩形居中位置
        int x = centerX - rectWidth / 2;
        int y = centerY - rectHeight / 2;

        qDebug() << "\t\t\t Rect" << i << "\t"
            << "RGBA: (" << r << "\t," << g << "\t," << b << "\t," << a << ")" << "\t"
            << "Size: (" << rectWidth << "x" << rectHeight << ")" << "\t"
            << "Start: (" << x << "x" << y << ")" << "\t"
            << "Center: (" << centerX << "x" << centerY <<")";
    }
}