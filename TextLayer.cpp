#include "TextLayer.h"
#include "common.h"
#include <QGuiApplication>
#include <QScreen>
#include <QDebug>

TextLayer::TextLayer(const QString& type, bool status)
    : Layer(type, status) {

    //QScreen* screen = QGuiApplication::primaryScreen();
    //QRect geometry = screen->geometry();
    //qreal pixelRatio = screen->devicePixelRatio();

    //int width = geometry.width() * pixelRatio;
    //int height = geometry.height() * pixelRatio;

    layerParams = {
        {"startX", 0},
        {"startY", 0},
        {"text", "https://www.szhk.com.cn"},
        {"font", "Arial"},
        {"size", 80},
        {"style", "Normal"},
        {"textColor", QColor(255, 0, 0, 255)}
    };
}

void TextLayer::setParameters(const QMap<QString, QVariant>& params) {
    for (auto it = params.begin(); it != params.end(); ++it) {
        layerParams[it.key()] = it.value();
    }
}

QMap<QString, QVariant> TextLayer::getParameters() const {
    return layerParams;
}

void TextLayer::drawLayer(QPainter& painter, bool isThumbnail) const {
    if (!getLayerStatus()) return;

    int startX = layerParams.value("startX").toInt();
    int startY = layerParams.value("startY").toInt();
    QString text = layerParams.value("text").toString();
    QString fontName = layerParams.value("font").toString();
    int fontSize = layerParams.value("size").toInt();
    QString fontStyle = layerParams.value("style").toString();

    QColor textColor = layerParams.value("textColor").value<QColor>();

    painter.save();
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(textColor);
    QFont font(fontName, fontSize);

    // 根据 style 设置字体样式
    if (fontStyle.compare("Bold", Qt::CaseInsensitive) == 0) {
        font.setBold(true);
    }
    else if (fontStyle.compare("Italic", Qt::CaseInsensitive) == 0) {
        font.setItalic(true);
    }
    else if (fontStyle.compare("Underline", Qt::CaseInsensitive) == 0) {
        font.setUnderline(true);
    }
    else if (fontStyle.compare("B & I", Qt::CaseInsensitive) == 0) {
        font.setItalic(true);
        font.setBold(true);
    }
    else if (fontStyle.compare("B & U", Qt::CaseInsensitive) == 0) {
        font.setBold(true);
        font.setUnderline(true);
    }
    else if (fontStyle.compare("I & U", Qt::CaseInsensitive) == 0) {
        font.setItalic(true);
        font.setUnderline(true);
    }
    else if (fontStyle.compare("B & I & U", Qt::CaseInsensitive) == 0) {
        font.setItalic(true);
        font.setBold(true);
        font.setUnderline(true);
    }
    else if (fontStyle.compare("Normal", Qt::CaseInsensitive) == 0) {
        font.setBold(false);
        font.setItalic(false);
        font.setUnderline(false);
    }

    painter.setFont(font);

    QFontMetrics fm(painter.font());
    int textHeight = fm.ascent();  // ascent ：baseline 到字体顶部的距离
    int lineHeight = fm.height();

    //if (!isThumbnail) {
    //    painter.translate(startX, startY);
    //}

    // 多行绘制
    QStringList lines = text.split('\n', Qt::KeepEmptyParts); // 保留空行
    for (int i = 0; i < lines.size(); ++i) {
        painter.drawText(startX, startY + textHeight + i * lineHeight, lines[i]);
    }

    painter.restore();
    //painter.drawText(startX, startY, endX - startX, endY - startY, Qt::AlignCenter, text);  // 绘制文本

    /*QFontMetrics metrics(font);
    int textWidth = metrics.horizontalAdvance(text);
    int textHeight = metrics.height();
    int rows = (endY - startY) / textHeight;
    int cols = (endX - startX) / textWidth;

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            int xPos = startX + col * textWidth;
            int yPos = startY + row * textHeight;
            painter.drawText(xPos, yPos + textHeight, text); 
        }
    }*/
}

void TextLayer::outputLayerInfo() const {
    QStringList outputOrder = {
        "startX", "startY", "text", "font", "size", "style"
    };

    qDebug() << "\t\tTextLayer Information:";
    for (const QString& key : outputOrder) {
        if (layerParams.contains(key)) {
            QString valueStr = layerParams.value(key).toString();

            if (key == "textColor") {
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