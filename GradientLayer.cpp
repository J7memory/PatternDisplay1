// GradientLayer.cpp
#include "GradientLayer.h"
#include "common.h"
#include <QGuiApplication>
#include <QScreen>
#include <QLinearGradient>

GradientLayer::GradientLayer(const QString& type, bool status)
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
        {"counts", 4},
        {"orientation", "Horizontal"},
    };

    initializeGradientBars(layerParams["counts"].toInt());
}

void GradientLayer::setParameters(const QMap<QString, QVariant>& params) {
    for (auto it = params.begin(); it != params.end(); ++it) {
        if (it.key() == "gradientBars") {
            QList<QMap<QString, QVariant>> newGradientBars = it.value().value<QList<QMap<QString, QVariant>>>();
            if (newGradientBars.size() != layerParams["counts"].toInt()) {
                initializeGradientBars(layerParams["counts"].toInt());
            }
            else {
                gradientBars = newGradientBars;
            }
        }
        else {
            layerParams[it.key()] = it.value();
        }
    }
}

QMap<QString, QVariant> GradientLayer::getParameters() const {
    QMap<QString, QVariant> params = layerParams;
    params["gradientBars"] = QVariant::fromValue(gradientBars);
    return params;
}

void GradientLayer::initializeGradientBars(int count) {
    if (gradientBars.size() > count) {
        gradientBars = gradientBars.mid(0, count);
    }
    else {
        QList<QColor> endColors = {
            QColor(255, 255, 255, 255),
            QColor(255, 0, 0, 255),
            QColor(0, 255, 0, 255),
            QColor(0, 0, 255, 255)
        };
        QColor startColor(0, 0, 0, 255);
        int steps = 9;

        for (int i = gradientBars.size(); i < count; ++i) {
            QMap<QString, QVariant> defaultGradient;
            defaultGradient["steps"] = steps;
            defaultGradient["startColor"] = QVariant::fromValue(startColor);
            defaultGradient["endColor"] = QVariant::fromValue((i < endColors.size()) ? endColors[i] : QColor(255, 255, 255, 255));

            gradientBars.append(defaultGradient);
        }
    }

    layerParams["gradientBars"] = QVariant::fromValue(gradientBars);
}

void GradientLayer::drawLayer(QPainter& painter, bool isThumbnail) const {
    int startX = layerParams.value("startX").toInt();
    int startY = layerParams.value("startY").toInt();
    int layerWidth = layerParams["width"].toInt();
    int layerHeight = layerParams["height"].toInt();
    int counts = layerParams["counts"].toInt();
    QString orientation = layerParams["orientation"].toString();

    painter.save();
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setBrush(Qt::NoBrush);

    QList<int> stepsPerRegion;
    for (const auto& bar : gradientBars) {
        stepsPerRegion.append(bar["steps"].toInt());
    }

    QList<QRect> regions = divideGradientRegion(layerWidth, layerHeight, counts, stepsPerRegion, orientation, startX, startY);

    //if (!isThumbnail) {
    //    for (QRect& rect : regions) {
    //        rect.translate(startX, startY);
    //    }
    //}

    int rectIndex = 0; // 记录当前绘制到哪个小区域

    for (int regionIndex = 0; regionIndex < counts; ++regionIndex) {
        if (regionIndex >= gradientBars.size()) break;

        const auto& bar = gradientBars[regionIndex];
        QColor startColor = bar["startColor"].value<QColor>();
        QColor endColor = bar["endColor"].value<QColor>();
        int steps = stepsPerRegion[regionIndex];
        if (steps <= 0) continue;

        for (int stepIndex = 0; stepIndex < steps; ++stepIndex) {
            if (rectIndex >= regions.size()) {
                qWarning() << "Warning: regions size less than total steps";
                break;
            }

            qreal ratio = (steps == 1) ? 0 : static_cast<qreal>(stepIndex) / (steps - 1);
            int r = qRound(startColor.red() * (1 - ratio) + endColor.red() * ratio);
            int g = qRound(startColor.green() * (1 - ratio) + endColor.green() * ratio);
            int b = qRound(startColor.blue() * (1 - ratio) + endColor.blue() * ratio);
            int a = qRound(startColor.alpha() * (1 - ratio) + endColor.alpha() * ratio);
            QColor interpolatedColor(r, g, b, a);

            // 打印当前区域信息和颜色值
            //qDebug() << "Region" << regionIndex
            //    << "Step" << stepIndex
            //    << "Ratio" << ratio
            //    << "Color(RGBA):" << r << g << b << a
            //    << "Rect:" << regions[rectIndex];

            painter.fillRect(regions[rectIndex], interpolatedColor);

            ++rectIndex;
        }
    }

    painter.restore();
}


QList<QRect> GradientLayer::divideGradientRegion(int width, int height, int counts,
    const QList<int>& stepsPerRegion,
    const QString& orientation,
    int startX, int startY)const{

    QList<QRect> regions;
    if (counts <= 0 || stepsPerRegion.size() != counts) return regions;

    int mainSize = (orientation == "Horizontal") ? height : width;
    int subSize = (orientation == "Horizontal") ? width : height;

    // 计算主方向每块大小，余数均匀分配
    QVector<double> mainBoundaries(counts + 1);
    for (int i = 0; i <= counts; ++i) {
        mainBoundaries[i] = (double)mainSize * i / counts;
    }

    int mainPos = 0;
    for (int i = 0; i < counts; ++i) {
        // 主方向区块大小，四舍五入边界差值
        int mainLength = qRound(mainBoundaries[i + 1]) - qRound(mainBoundaries[i]);

        int steps = stepsPerRegion[i];
        if (steps <= 0) continue;

        // 副方向每块大小，余数均匀分配
        QVector<double> subBoundaries(steps + 1);
        for (int j = 0; j <= steps; ++j) {
            subBoundaries[j] = (double)subSize * j / steps;
        }

        int subPos = 0;
        for (int j = 0; j < steps; ++j) {
            int subLength = qRound(subBoundaries[j + 1]) - qRound(subBoundaries[j]);

            QRect region;
            if (orientation == "Horizontal") {
                region = QRect(startX + subPos, startY + mainPos, subLength, mainLength);
            }
            else {
                region = QRect(startX + mainPos, startY + subPos, mainLength, subLength);
            }
            regions.append(region);

            // 调试输出
            //qDebug() << QString("Region[%1][%2]: x=%3 y=%4 w=%5 h=%6")
            //    .arg(i).arg(j)
            //    .arg(region.x()).arg(region.y())
            //    .arg(region.width()).arg(region.height());

            subPos += subLength;
        }
        mainPos += mainLength;
    }

    return regions;
}

void GradientLayer::outputLayerInfo() const {
    QStringList outputOrder = {
         "startX", "startY", "width", "height", "counts", "orientation",
    };

    qDebug() << "\t\tMonoLayer Information:";
    for (const QString& key : outputOrder) {
        if (layerParams.contains(key)) {
            QString valueStr = layerParams.value(key).toString();
            qDebug() << "\t\t\t" << key << ":" << valueStr;
        }
    }

    qDebug() << "\t\t\tGradient Bars Info:";
    for (int i = 0; i < gradientBars.size(); ++i) {
        const auto& gradient = gradientBars[i];
        qDebug() << "\t\t\t\tGradient" << i + 1 << "Info:";

        for (auto it = gradient.begin(); it != gradient.end(); ++it) {
            const QString& key = it.key();
            const QVariant& value = it.value();

            if (key == "startColor" || key == "endColor") {
                QColor color = value.value<QColor>();
                QString colorStr = QString("(%1, %2, %3, %4)")
                    .arg(color.red())
                    .arg(color.green())
                    .arg(color.blue())
                    .arg(color.alpha());
                qDebug() << "\t\t\t\t\t" << key << "(RGBA):" << colorStr;
            }
            else {
                qDebug() << "\t\t\t\t\t" << key << ":" << value.toString();
            }
        }
    }
}