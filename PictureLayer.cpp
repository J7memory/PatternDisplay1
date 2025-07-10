#include "PictureLayer.h"
#include <QDebug>
#include <QFileInfo>
#include <QMessageBox>
#include <QImageReader>
#include <QGuiApplication>
#include <QScreen>

PictureLayer::PictureLayer(const QString& type, bool status)
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
        {"mode", "Original"},
        {"imagePath", ""}
    };
}

void PictureLayer::setParameters(const QMap<QString, QVariant>& params) {
    for (auto it = params.begin(); it != params.end(); ++it) {
        layerParams[it.key()] = it.value();
    }

    // ��� imagePath �и��£��������¼���ͼƬ
    if (params.contains("imagePath")) {
        QString path = params.value("imagePath").toString();
        loadImage(path);
    }
}

QMap<QString, QVariant> PictureLayer::getParameters() const {
    return layerParams;
}

void PictureLayer::loadImage(const QString& path) {
    if (path.isEmpty()) {
        //QMessageBox::warning(nullptr, "Image Load Error", "Image path is empty. Please select a valid image file.");
        return;
    }

    QString suffix = QFileInfo(path).suffix().toLower();
    QByteArray format = suffix.toLatin1();

    if (!QImageReader::supportedImageFormats().contains(format)) {
        QMessageBox::warning(nullptr, "Unsupported Format",
            QString("The image format '%1' is not supported.\nPlease use PNG, JPG, BMP, etc.").arg(suffix));
        return;
    }

    if (!pixmap.load(path)) {
        pixmap = QPixmap(); // ��� pixmap
        QMessageBox::warning(nullptr, "Image Load Failed",
            QString("Failed to load image from path:\n%1").arg(path));
    }
}

QPixmap PictureLayer::getPixmap() const {
    return pixmap;
}

void PictureLayer::drawLayer(QPainter& painter, bool isThumbnail) const {
    if (!getLayerStatus()) return;
    if (pixmap.isNull()) return;

    int startX = layerParams.value("startX").toInt();
    int startY = layerParams.value("startY").toInt();
    int width = layerParams.value("width").toInt();
    int height = layerParams.value("height").toInt();
    QString mode = layerParams.value("mode").toString();

    QPixmap scaledPixmap;
    QSize originalSize = pixmap.size();
    Qt::AspectRatioMode aspectMode = Qt::IgnoreAspectRatio;  // Ĭ��Ϊԭͼ��ʾ

    if (mode == "Original") {
        scaledPixmap = pixmap;
        scaledPixmap = pixmap.scaled(originalSize, aspectMode, Qt::SmoothTransformation);
    }
    else if (mode == "Maintain Ratio") {
        // ���ֿ�߱�
        aspectMode = Qt::KeepAspectRatio;
        scaledPixmap = pixmap.scaled(width, height, aspectMode, Qt::SmoothTransformation);
    }
    else if (mode == "Ignore Ratio") {
        // ���Կ�߱ȣ���ȫ���Ŀ������
        aspectMode = Qt::IgnoreAspectRatio;
        scaledPixmap = pixmap.scaled(width, height, aspectMode, Qt::SmoothTransformation);
    }
    else if (mode == "Expand Ratio") {
        // ���ֿ�߱Ȳ���չ
        aspectMode = Qt::KeepAspectRatioByExpanding;
        scaledPixmap = pixmap.scaled(width, height, aspectMode, Qt::SmoothTransformation);
    }

    // ��ȡ����
    QRect sourceRect(0, 0, width, height);
    sourceRect = sourceRect.intersected(scaledPixmap.rect());
    QPoint targetPoint(startX, startY);
    painter.drawPixmap(targetPoint, scaledPixmap, sourceRect);
    //if (!isThumbnail) {
    //    QPoint targetPoint(startX, startY);
    //    painter.drawPixmap(targetPoint, scaledPixmap, sourceRect);
    //}
    //else {
    //    QPoint targetPoint(0, 0);
    //    painter.drawPixmap(targetPoint, scaledPixmap, sourceRect);
    //}
}


void PictureLayer::outputLayerInfo() const {
    qDebug() << "\t\tPictureLayer Information:";

    for (auto it = layerParams.begin(); it != layerParams.end(); ++it) {
        QString key = it.key();
        QVariant value = it.value();

        if (key == "mode") {
            QString modeStr = value.toString();  // ֱ������ַ���
            qDebug() << "\t\t\t" << key << ":" << modeStr;
        }
        else {
            qDebug() << "\t\t\t" << key << ":" << value.toString();
        }
    }
}


